<#
.SYNOPSIS
Doinstalowuje pip do wybranej instalacji Pythona znajduj¹cej siê w C:\Python\Python*
i aktualizuje systemow¹ zmienn¹ PATH o katalog Scripts.
.DESCRIPTION
Skrypt skanuje katalog C:\Python w poszukiwaniu podfolderów Python* z python.exe.
Dla ka¿dej wersji sprawdza, czy pip.exe istnieje w podfolderze Scripts.
Nastêpnie pozwala wybraæ wersjê bez pip i instaluje pip za pomoc¹ ensurepip lub get-pip.py.
Na koniec dodaje œcie¿kê Scripts do systemowego PATH (jeœli brak) i odœwie¿a œrodowisko.
Wymaga uprawnieñ administratora.
#>

# Sprawdzenie uprawnieñ administratora
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
if (-not $isAdmin) {
    Write-Error "Skrypt musi byæ uruchomiony jako administrator (prawym przyciskiem -> Uruchom jako administrator)."
    exit 1
}

# Œcie¿ka bazowa instalacji (zgodna z poprzednimi skryptami)
$basePath = "C:\Python"

# ZnajdŸ wszystkie podfoldery pasuj¹ce do C:\Python\Python*
$pythonFolders = Get-ChildItem -Path $basePath -Directory | Where-Object { $_.Name -like "Python*" }

if ($pythonFolders.Count -eq 0) {
    Write-Error "Nie znaleziono ¿adnego folderu z Pythonem w $basePath\Python*"
    exit 1
}

# Zebranie informacji o wersjach i obecnoœci pip
$versionsWithPip = @()
$versionsWithoutPip = @()

foreach ($folder in $pythonFolders) {
    $pythonExe = Join-Path $folder.FullName "python.exe"
    if (-not (Test-Path $pythonExe)) { continue }
    
    $pipExe = Join-Path $folder.FullName "Scripts\pip.exe"
    $hasPip = Test-Path $pipExe
    
    # Pobranie wersji Pythona
    try {
        $versionOutput = & $pythonExe --version 2>&1
        if ($versionOutput -match "Python (\d+\.\d+\.\d+)") {
            $pythonVersion = $matches[1]
        } else {
            $pythonVersion = "nieznana"
        }
    } catch {
        $pythonVersion = "nieznana"
    }
    
    $info = [PSCustomObject]@{
        Name         = $folder.Name
        FullPath     = $folder.FullName
        PythonExe    = $pythonExe
        PipExe       = $pipExe
        HasPip       = $hasPip
        PythonVersion = $pythonVersion
    }
    
    if ($hasPip) {
        $versionsWithPip += $info
    } else {
        $versionsWithoutPip += $info
    }
}

# Wyœwietlenie informacji o wersjach z pip
if ($versionsWithPip.Count -gt 0) {
    Write-Host "`nWersje Pythona, które ju¿ maj¹ pip:" -ForegroundColor Green
    foreach ($v in $versionsWithPip) {
        Write-Host "  - $($v.Name) (wersja $($v.PythonVersion)) -> pip ju¿ dostêpny"
    }
}

# Sprawdzenie, czy s¹ wersje bez pip
if ($versionsWithoutPip.Count -eq 0) {
    Write-Host "`nWszystkie znalezione wersje Pythona maj¹ ju¿ pip. Nic nie trzeba robiæ." -ForegroundColor Green
    exit 0
}

# Wyœwietlenie wersji do wyboru
Write-Host "`nWersje Pythona wymagaj¹ce instalacji pip:" -ForegroundColor Yellow
for ($i = 0; $i -lt $versionsWithoutPip.Count; $i++) {
    Write-Host "[$($i+1)] $($versionsWithoutPip[$i].Name) (wersja $($versionsWithoutPip[$i].PythonVersion))"
}

$choice = Read-Host "`nPodaj numer wersji, dla której chcesz doinstalowaæ pip (1-$($versionsWithoutPip.Count))"
$index = [int]$choice - 1
if ($index -lt 0 -or $index -ge $versionsWithoutPip.Count) {
    Write-Error "Nieprawid³owy numer."
    exit 1
}

$selected = $versionsWithoutPip[$index]
$pythonExe = $selected.PythonExe
$scriptsDir = Join-Path $selected.FullPath "Scripts"

Write-Host "`nInstalowanie pip dla Pythona: $($selected.Name)" -ForegroundColor Cyan

# Metoda 1: ensurepip (wbudowane w Python 3.4+)
Write-Host "Próba u¿ycia 'python -m ensurepip' ..." -ForegroundColor Cyan
$result = & $pythonExe -m ensurepip --upgrade 2>&1
if ($LASTEXITCODE -eq 0) {
    Write-Host "Pip zosta³ pomyœlnie zainstalowany przez ensurepip." -ForegroundColor Green
} else {
    Write-Host "ensurepip nie zadzia³a³o (kod b³êdu: $LASTEXITCODE). Pobieram get-pip.py ..." -ForegroundColor Yellow
    # Metoda 2: get-pip.py
    $getPipUrl = "https://bootstrap.pypa.io/get-pip.py"
    $tempGetPip = Join-Path $env:TEMP "get-pip.py"
    try {
        Invoke-WebRequest -Uri $getPipUrl -OutFile $tempGetPip -UseBasicParsing
        & $pythonExe $tempGetPip --no-warn-script-location
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Pip zosta³ pomyœlnie zainstalowany przez get-pip.py." -ForegroundColor Green
        } else {
            Write-Error "Nie uda³o siê zainstalowaæ pip za pomoc¹ get-pip.py."
            Remove-Item $tempGetPip -Force -ErrorAction SilentlyContinue
            exit 1
        }
        Remove-Item $tempGetPip -Force -ErrorAction SilentlyContinue
    } catch {
        Write-Error "Nie uda³o siê pobraæ get-pip.py: $_"
        exit 1
    }
}

# Weryfikacja, czy pip faktycznie istnieje
$pipExe = Join-Path $scriptsDir "pip.exe"
if (-not (Test-Path $pipExe)) {
    Write-Error "Pip nie zosta³ znaleziony w $scriptsDir mimo próby instalacji."
    exit 1
}
Write-Host "Pip zosta³ pomyœlnie zainstalowany: $pipExe" -ForegroundColor Green

# Dodanie katalogu Scripts do systemowej zmiennej PATH (jeœli jeszcze nie istnieje)
$currentMachinePath = [Environment]::GetEnvironmentVariable("Path", "Machine")
$paths = $currentMachinePath -split ';' | Where-Object { $_ -ne "" }
if ($paths -notcontains $scriptsDir) {
    Write-Host "Dodawanie $scriptsDir do systemowej zmiennej PATH ..." -ForegroundColor Cyan
    $updatedPath = "$scriptsDir;$currentMachinePath"
    [Environment]::SetEnvironmentVariable("Path", $updatedPath, "Machine")
    Write-Host "Œcie¿ka dodana." -ForegroundColor Green
} else {
    Write-Host "Œcie¿ka $scriptsDir ju¿ znajduje siê w PATH." -ForegroundColor Green
}

# Odœwie¿enie zmiennej PATH w bie¿¹cej sesji
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Test koñcowy
Write-Host "`nTestowanie pip:" -ForegroundColor Cyan
try {
    $pipVersion = & pip --version 2>&1
    Write-Host "pip --version : $pipVersion" -ForegroundColor Green
} catch {
    Write-Host "Nie mo¿na uruchomiæ pip. SprawdŸ, czy katalog Scripts zosta³ dodany do PATH i uruchom now¹ konsolê." -ForegroundColor Red
}

Write-Host "`nInstalacja pip zakoñczona. Mo¿esz teraz u¿ywaæ 'pip' z dowolnego folderu." -ForegroundColor Green