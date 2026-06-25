<#
.SYNOPSIS
Naprawia œrodowisko Python (PATH, PYTHONHOME) – usuwa b³êdy "Failed to import encodings".
Wymaga uprawnieñ administratora.
#>

$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
if (-not $isAdmin) {
    Write-Error "Uruchom jako administrator (prawym przyciskiem -> Uruchom jako administrator)."
    exit 1
}

$basePath = "C:\Python"
$pythonFolders = Get-ChildItem -Path $basePath -Directory | Where-Object { $_.Name -like "Python*" }

if ($pythonFolders.Count -eq 0) {
    Write-Error "Nie znaleziono ¿adnego folderu C:\Python\Python*"
    exit 1
}

$validPythons = @()
$invalidPythons = @()

Write-Host "`nSprawdzanie poprawnoœci instalacji Pythona w C:\Python\Python* ..." -ForegroundColor Cyan
foreach ($folder in $pythonFolders) {
    $pythonExe = Join-Path $folder.FullName "python.exe"
    if (-not (Test-Path $pythonExe)) {
        $invalidPythons += $folder.Name
        continue
    }
    # Test importu encodings
    $test = & $pythonExe -c "import encodings; print('OK')" 2>&1
    if ($LASTEXITCODE -eq 0) {
        $version = & $pythonExe --version 2>&1
        $validPythons += [PSCustomObject]@{
            Name      = $folder.Name
            FullPath  = $folder.FullName
            PythonExe = $pythonExe
            Version   = $version
        }
        Write-Host "  [OK] $($folder.Name) – dzia³a poprawnie" -ForegroundColor Green
    } else {
        $invalidPythons += $folder.Name
        Write-Host "  [FAIL] $($folder.Name) – b³¹d encodings (niekompletna instalacja)" -ForegroundColor Red
    }
}

if ($validPythons.Count -eq 0) {
    Write-Error "BRAK sprawnej instalacji Pythona. Zainstaluj ponownie (najlepiej rêcznie, bez /quiet)."
    exit 1
}

# Wyœwietlenie dzia³aj¹cych wersji
Write-Host "`nDzia³aj¹ce wersje Pythona:" -ForegroundColor Green
for ($i = 0; $i -lt $validPythons.Count; $i++) {
    Write-Host "[$($i+1)] $($validPythons[$i].Name)  $($validPythons[$i].Version)"
}

$choice = Read-Host "`nWybierz numer wersji, która ma byæ domyœlna (1-$($validPythons.Count))"
$index = [int]$choice - 1
if ($index -lt 0 -or $index -ge $validPythons.Count) {
    Write-Error "Nieprawid³owy numer."
    exit 1
}

$selected = $validPythons[$index]
$pythonDir = $selected.FullPath
$scriptsDir = Join-Path $pythonDir "Scripts"

Write-Host "`nNaprawiam œrodowisko dla: $($selected.Name)" -ForegroundColor Cyan

# 1. Usuwamy istniej¹c¹ zmienn¹ PYTHONHOME (systemow¹) – czêsto b³êdna.
Write-Host "Usuwanie starej (lub ustawianie poprawnej) PYTHONHOME ..." -ForegroundColor Cyan
try {
    [Environment]::SetEnvironmentVariable("PYTHONHOME", $pythonDir, "Machine")
    Write-Host "PYTHONHOME = $pythonDir" -ForegroundColor Green
} catch {
    Write-Warning "Nie uda³o siê ustawiæ PYTHONHOME: $_"
}

# 2. Oczyszczamy PATH z wszystkich wpisów prowadz¹cych do starych instalacji Pythona
$oldPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$patternPython = 'C:\\Python\\Python[^;]*'
$newPath = $oldPath -split ';' | Where-Object { $_ -notmatch $patternPython } | Where-Object { $_ -ne "" } | Join-String -Separator ';'

# 3. Dodajemy na pocz¹tek nowe œcie¿ki (g³ówny katalog i Scripts) – to wa¿ne, aby Python.exe znalaz³ swoje biblioteki
$pathsToAdd = @($pythonDir)
if (Test-Path $scriptsDir) {
    $pathsToAdd += $scriptsDir
} else {
    Write-Warning "Nie znaleziono katalogu Scripts – pip mo¿e byæ niedostêpny."
}
$updatedPath = ($pathsToAdd + ($newPath -split ';' | Where-Object { $_ -ne "" })) -join ';'

[Environment]::SetEnvironmentVariable("Path", $updatedPath, "Machine")
Write-Host "Œcie¿ka PATH zaktualizowana." -ForegroundColor Green

# 4. Odœwie¿enie zmiennych w bie¿¹cej sesji
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
$env:PYTHONHOME = $pythonDir

# 5. Test koñcowy
Write-Host "`nTestowanie po naprawie:" -ForegroundColor Cyan
try {
    $pythonVer = & python --version 2>&1
    Write-Host "python --version : $pythonVer" -ForegroundColor Green
} catch {
    Write-Host "python nadal nie dzia³a. Uruchom NOWE okno PowerShell (administrator) i spróbuj ponownie." -ForegroundColor Red
}
try {
    $pipVer = & pip --version 2>&1
    Write-Host "pip --version    : $pipVer" -ForegroundColor Green
} catch {
    Write-Host "pip nie jest dostêpny. Byæ mo¿e brak plików w Scripts." -ForegroundColor Yellow
}

Write-Host "`nJeœli nadal wystêpuje b³¹d encodings, otwórz NOW¥ konsolê (PowerShell lub CMD) jako zwyk³y u¿ytkownik i wpisz 'python'." -ForegroundColor Yellow
