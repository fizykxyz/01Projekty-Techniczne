<#
.SYNOPSIS
Ustawia aktywn¹ wersjê Pythona spoœród zainstalowanych w C:\Python\Python*
Poprzez modyfikacjê zmiennej œrodowiskowej PATH (systemowej).
.DESCRIPTION
Skrypt skanuje katalog C:\Python w poszukiwaniu podfolderów o nazwie Python*,
sprawdza obecnoœæ python.exe i pip.exe, wyœwietla listê wersji do wyboru,
a nastêpnie aktualizuje systemow¹ zmienn¹ PATH (wymaga uprawnieñ administratora)
dodaj¹c œcie¿ki do wybranej wersji (python.exe i pip.exe) oraz usuwaj¹c stare
œcie¿ki prowadz¹ce do innych instalacji Pythona w C:\Python.
Po zmianie PATH jest odœwie¿any w bie¿¹cej sesji.
.NOTES
Wymaga uruchomienia jako administrator.
Autor: Asystent
#>


<#
.SYNOPSIS
Ustawia aktywn¹ wersjê Pythona spoœród zainstalowanych w C:\Python\Python*
Dodatkowo weryfikuje poprawnoœæ instalacji (modu³ encodings).
#>

$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
if (-not $isAdmin) {
    Write-Error "Uruchom jako administrator."
    exit 1
}

$basePath = "C:\Python"
$pythonFolders = Get-ChildItem -Path $basePath -Directory | Where-Object { $_.Name -like "Python*" }
$validVersions = @()

foreach ($folder in $pythonFolders) {
    $pythonExe = Join-Path $folder.FullName "python.exe"
    if (Test-Path $pythonExe) {
        $versionOutput = & $pythonExe --version 2>&1
        if ($versionOutput -match "Python (\d+\.\d+\.\d+)") {
            $version = $matches[1]
        } else { $version = "nieznana" }
        $validVersions += [PSCustomObject]@{
            Name      = $folder.Name
            FullPath  = $folder.FullName
            PythonExe = $pythonExe
            Version   = $version
        }
    }
}

if ($validVersions.Count -eq 0) { Write-Error "Brak Pythona w $basePath"; exit 1 }

Write-Host "Znalezione wersje:`n" -ForegroundColor Green
for ($i = 0; $i -lt $validVersions.Count; $i++) {
    Write-Host "[$($i+1)] $($validVersions[$i].Name) (wersja $($validVersions[$i].Version))"
}

$choice = Read-Host "`nWybierz numer"
$index = [int]$choice - 1
if ($index -lt 0 -or $index -ge $validVersions.Count) { Write-Error "Nieprawid³owy numer"; exit 1 }

$selected = $validVersions[$index]

# Test poprawnoœci instalacji
Write-Host "`nSprawdzanie poprawnoœci instalacji..." -ForegroundColor Cyan
$testResult = & $selected.PythonExe -c "import encodings" 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "Instalacja Pythona w $($selected.FullPath) jest NIEKOMPLETNA!" -ForegroundColor Red
    Write-Host "B³¹d: $testResult" -ForegroundColor Red
    Write-Host "`nRozwi¹zanie: Zainstaluj tê wersjê ponownie, u¿ywaj¹c instalatora w trybie interaktywnym (bez /quiet) lub z flag¹ SimpleInstall=1." -ForegroundColor Yellow
    exit 1
}
Write-Host "Instalacja wygl¹da poprawnie." -ForegroundColor Green

# Reszta skryptu (aktualizacja PATH) jak poprzednio...
# [wstaw tutaj kod aktualizacji PATH – ten sam co wczeœniej]










# Sprawdzenie uprawnieñ administratora
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
if (-not $isAdmin) {
    Write-Error "Ten skrypt wymaga uprawnieñ administratora, aby zmieniæ systemow¹ zmienn¹ PATH."
    Write-Host "Uruchom PowerShell jako administrator (prawym przyciskiem myszy -> Uruchom jako administrator)." -ForegroundColor Yellow
    exit 1
}

# Œcie¿ka bazowa, gdzie szukamy instalacji Pythona (zgodnie z poprzednim skryptem)
$basePath = "C:\Python"

# ZnajdŸ wszystkie podfoldery pasuj¹ce do C:\Python\Python*
$pythonFolders = Get-ChildItem -Path $basePath -Directory | Where-Object { $_.Name -like "Python*" }

if ($pythonFolders.Count -eq 0) {
    Write-Error "Nie znaleziono ¿adnego folderu z Pythonem w $basePath\Python*"
    exit 1
}

# Dla ka¿dego folderu sprawdŸ, czy zawiera python.exe i pobierz wersjê
$validVersions = @()
foreach ($folder in $pythonFolders) {
    $pythonExe = Join-Path $folder.FullName "python.exe"
    $pipExe = Join-Path $folder.FullName "Scripts\pip.exe"  # pip zwykle w Scripts
    if (Test-Path $pythonExe) {
        # Pobierz wersjê Pythona
        try {
            $versionOutput = & $pythonExe --version 2>&1
            if ($versionOutput -match "Python (\d+\.\d+\.\d+)") {
                $version = $matches[1]
            } else {
                $version = "nieznana"
            }
        } catch {
            $version = "nieznana"
        }
        $validVersions += [PSCustomObject]@{
            Name      = $folder.Name
            FullPath  = $folder.FullName
            PythonExe = $pythonExe
            PipExe    = $pipExe
            Version   = $version
        }
    }
}

if ($validVersions.Count -eq 0) {
    Write-Error "Nie znaleziono ¿adnej poprawnej instalacji Pythona (brak python.exe)."
    exit 1
}

# Wyœwietlenie menu
Write-Host "`nZnalezione wersje Pythona w $basePath :`n" -ForegroundColor Green
for ($i = 0; $i -lt $validVersions.Count; $i++) {
    Write-Host "[$($i+1)] $($validVersions[$i].Name)  (wersja $($validVersions[$i].Version))"
}

# Pobranie wyboru u¿ytkownika
$choice = Read-Host "`nPodaj numer wersji, która ma byæ aktywna globalnie (1-$($validVersions.Count))"
$index = [int]$choice - 1
if ($index -lt 0 -or $index -ge $validVersions.Count) {
    Write-Error "Nieprawid³owy numer."
    exit 1
}

$selected = $validVersions[$index]
$newPythonPath = $selected.FullPath
$newScriptsPath = Join-Path $newPythonPath "Scripts"

# SprawdŸ, czy katalog Scripts istnieje (pip.exe tam powinien byæ)
if (-not (Test-Path $newScriptsPath)) {
    Write-Warning "Nie znaleziono katalogu Scripts w $newPythonPath. Pip mo¿e nie byæ dostêpny."
    $newScriptsPath = $null
} elseif (-not (Test-Path (Join-Path $newScriptsPath "pip.exe"))) {
    Write-Warning "W katalogu $newScriptsPath nie znaleziono pip.exe. Pip nie bêdzie dodany do PATH."
}

# Pobranie aktualnej systemowej zmiennej PATH
$oldPath = [Environment]::GetEnvironmentVariable("Path", "Machine")

# Usuñ wszystkie istniej¹ce wpisy prowadz¹ce do C:\Python\Python* (zarówno g³ówny folder, jak i Scripts)
$patternToRemove = 'C:\\Python\\Python[^;]*'
$newPath = $oldPath -split ';' | Where-Object { $_ -notmatch $patternToRemove } | Where-Object { $_ -ne "" } | Join-String -Separator ';'

# Dodaj nowe œcie¿ki na pocz¹tek (aby mia³y priorytet)
$pathsToAdd = @($newPythonPath)
if ($newScriptsPath) {
    $pathsToAdd += $newScriptsPath
}
$updatedPath = ($pathsToAdd + ($newPath -split ';' | Where-Object { $_ -ne "" })) -join ';'

# Zapisz now¹ PATH do rejestru (systemowa)
[Environment]::SetEnvironmentVariable("Path", $updatedPath, "Machine")
Write-Host "`nSystemowa zmienna PATH zosta³a zaktualizowana." -ForegroundColor Cyan

# Odœwie¿enie PATH w bie¿¹cej sesji
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Weryfikacja
Write-Host "`nSprawdzam dostêpnoœæ python i pip:" -ForegroundColor Cyan
try {
    $pythonVer = & python --version 2>&1
    Write-Host "python --version : $pythonVer" -ForegroundColor Green
} catch {
    Write-Host "python nie jest dostêpny po zmianie PATH" -ForegroundColor Red
}
try {
    $pipVer = & pip --version 2>&1
    Write-Host "pip --version    : $pipVer" -ForegroundColor Green
} catch {
    Write-Host "pip nie jest dostêpny (mo¿e brak lub problem z PATH)" -ForegroundColor Red
}

Write-Host "`nAktywna wersja Pythona: $($selected.Name) (œcie¿ka: $newPythonPath)" -ForegroundColor Green
Write-Host "Mo¿esz teraz u¿ywaæ 'python' i 'pip' z dowolnego folderu." -ForegroundColor Yellow