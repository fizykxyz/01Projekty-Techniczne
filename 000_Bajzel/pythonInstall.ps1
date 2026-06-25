<#
.SYNOPSIS
Pobiera, instaluje i ustawia jako domyœln¹ wybran¹ wersjê Pythona w C:\Python\PythonX.Y.Z.
Instaluje w pe³ni (pip, biblioteka standardowa) i aktualizuje systemow¹ œcie¿kê PATH.
.DESCRIPTION
Skrypt wymaga uprawnieñ administratora. Pobiera listê stabilnych wersji Pythona,
umo¿liwia wybór, nastêpnie cicho instaluje z odpowiednimi flagami (SimpleInstall=1),
a na koniec modyfikuje PATH i ustawia PYTHONHOME.
#>

# Wymagane uprawnienia administratora
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
if (-not $isAdmin) {
    Write-Error "Skrypt musi byæ uruchomiony jako administrator (prawym przyciskiem -> Uruchom jako administrator)."
    exit 1
}

# Ustawienie protoko³u TLS 1.2 dla starszych systemów
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor [System.Net.SecurityProtocolType]::Tls12

# Funkcja pobiera listê dostêpnych wersji Pythona z instalatorem amd64
function Get-AvailablePythonVersions {
    $ftpBase = "https://www.python.org/ftp/python/"
    Write-Host "Pobieranie listy dostêpnych wersji z python.org ..." -ForegroundColor Cyan
    try {
        $response = Invoke-WebRequest -Uri $ftpBase -UseBasicParsing -TimeoutSec 30
    } catch {
        Write-Error "Nie uda³o siê po³¹czyæ z $ftpBase : $_"
        exit 1
    }

    $dirLinks = $response.Links | Where-Object { $_.href -match '^(\d+\.\d+\.\d+)/$' } | Select-Object -ExpandProperty href
    $versions = @()
    foreach ($dir in $dirLinks) {
        $version = $dir.TrimEnd('/')
        $installerUrl = "$ftpBase$version/python-$version-amd64.exe"
        try {
            $headRequest = [System.Net.WebRequest]::Create($installerUrl)
            $headRequest.Method = "HEAD"
            $headResponse = $headRequest.GetResponse()
            if ($headResponse.StatusCode -eq 200) {
                $versions += [PSCustomObject]@{ Version = $version; Url = $installerUrl }
            }
            $headResponse.Close()
        } catch {
            # brak instalatora amd64 – pomiñ
        }
    }
    return $versions | Sort-Object { [version]$_.Version } -Descending
}

# G³ówna czêœæ: pobranie listy i wybór wersji
$versionsList = Get-AvailablePythonVersions
if ($versionsList.Count -eq 0) {
    Write-Error "Nie znaleziono ¿adnej wersji Pythona z instalatorem Windows amd64."
    exit 1
}

Write-Host "`nDostêpne wersje Pythona do zainstalowania:`n" -ForegroundColor Green
for ($i = 0; $i -lt $versionsList.Count; $i++) {
    Write-Host "[$($i+1)] $($versionsList[$i].Version)"
}

$choice = Read-Host "`nPodaj numer wersji (1-$($versionsList.Count))"
$index = [int]$choice - 1
if ($index -lt 0 -or $index -ge $versionsList.Count) {
    Write-Error "Nieprawid³owy numer."
    exit 1
}

$selected = $versionsList[$index]
$version = $selected.Version
$installerUrl = $selected.Url
$targetDir = "C:\Python\Python$version"

# Jeœli katalog docelowy ju¿ istnieje, zapytaj czy go usun¹æ (czysta instalacja)
if (Test-Path $targetDir) {
    Write-Warning "Katalog $targetDir ju¿ istnieje."
    $remove = Read-Host "Czy usun¹æ go przed instalacj¹? (T/N) [domyœlnie N]"
    if ($remove -eq 'T' -or $remove -eq 't') {
        Remove-Item -Recurse -Force $targetDir -ErrorAction Stop
        Write-Host "Usuniêto stary katalog." -ForegroundColor Yellow
    }
}

# Pobranie instalatora
$tempInstaller = [System.IO.Path]::GetTempFileName() + ".exe"
Write-Host "`nPobieranie instalatora dla Pythona $version ..." -ForegroundColor Cyan
try {
    Invoke-WebRequest -Uri $installerUrl -OutFile $tempInstaller -UseBasicParsing
} catch {
    Write-Error "Nie uda³o siê pobraæ instalatora: $_"
    Remove-Item $tempInstaller -Force -ErrorAction SilentlyContinue
    exit 1
}

# Instalacja w trybie cichym z odpowiednimi flagami:
# SimpleInstall=1 – uproszczona instalacja (kopiuje wszystkie pliki, pomija rejestracjê MSI)
# InstallAllUsers=1 – dla wszystkich u¿ytkowników (wymaga admin)
# TargetDir – nasz katalog
# PrependPath=0 – nie modyfikuj PATH (zrobimy to sami)
# Include_pip=1 – instaluj pip
# Include_tcltk=1 – instaluj tkinter
# Include_doc=1 – dokumentacja
# AssociateFiles=0 – nie kojarz .py
# Shortcuts=0 – nie twórz skrótów
# Include_launcher=0 – nie instaluj launcher (opcjonalnie, mo¿na te¿ 1)
$arguments = "/quiet InstallAllUsers=1 TargetDir=`"$targetDir`" PrependPath=0 Include_pip=1 Include_tcltk=1 Include_doc=1 AssociateFiles=0 Shortcuts=0 Include_launcher=0 SimpleInstall=1"

Write-Host "Instalowanie Pythona $version do $targetDir ..." -ForegroundColor Cyan
$process = Start-Process -FilePath $tempInstaller -ArgumentList $arguments -Wait -PassThru -NoNewWindow

Remove-Item $tempInstaller -Force -ErrorAction SilentlyContinue

if ($process.ExitCode -ne 0) {
    Write-Error "Instalacja nie powiod³a siê. Kod b³êdu: $($process.ExitCode)"
    Write-Host "Spróbuj uruchomiæ instalator rêcznie: $installerUrl" -ForegroundColor Yellow
    exit $process.ExitCode
}

# Sprawdzenie poprawnoœci instalacji (modu³ encodings)
Write-Host "`nWeryfikacja poprawnoœci instalacji..." -ForegroundColor Cyan
$pythonExe = Join-Path $targetDir "python.exe"
$testResult = & $pythonExe -c "import encodings; print('OK')" 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Error "Instalacja jest niekompletna (b³¹d encodings). Spróbuj zainstalowaæ rêcznie u¿ywaj¹c instalatora bez flagi /quiet."
    Write-Host "B³¹d: $testResult" -ForegroundColor Red
    exit 1
}
Write-Host "Instalacja przebieg³a pomyœlnie i Python dzia³a poprawnie." -ForegroundColor Green

# Ustawienie systemowej zmiennej PYTHONHOME
Write-Host "`nUstawianie systemowej zmiennej PYTHONHOME ..." -ForegroundColor Cyan
[Environment]::SetEnvironmentVariable("PYTHONHOME", $targetDir, "Machine")
Write-Host "PYTHONHOME = $targetDir" -ForegroundColor Green

# Modyfikacja systemowej zmiennej PATH
Write-Host "Aktualizacja systemowej zmiennej PATH ..." -ForegroundColor Cyan
$oldPath = [Environment]::GetEnvironmentVariable("Path", "Machine")

# Usuwamy wszystkie wpisy zawieraj¹ce C:\Python\Python* (zarówno g³ówny, jak i Scripts)
$patternToRemove = 'C:\\Python\\Python[^;]*'
$newPath = $oldPath -split ';' | Where-Object { $_ -notmatch $patternToRemove } | Where-Object { $_ -ne "" } | Join-String -Separator ';'

# Dodajemy nowe œcie¿ki na pocz¹tek (priorytet)
$scriptsPath = Join-Path $targetDir "Scripts"
$pathsToAdd = @($targetDir)
if (Test-Path $scriptsPath) {
    $pathsToAdd += $scriptsPath
} else {
    Write-Warning "Nie znaleziono katalogu Scripts (pip mo¿e byæ niedostêpny)."
}
$updatedPath = ($pathsToAdd + ($newPath -split ';' | Where-Object { $_ -ne "" })) -join ';'

[Environment]::SetEnvironmentVariable("Path", $updatedPath, "Machine")
Write-Host "Œcie¿ka PATH zaktualizowana." -ForegroundColor Green

# Odœwie¿enie œrodowiska w bie¿¹cej sesji
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
$env:PYTHONHOME = $targetDir

# Test koñcowy
Write-Host "`nTestowanie w nowej sesji (python i pip):" -ForegroundColor Cyan
try {
    $pythonVer = & python --version 2>&1
    Write-Host "python --version : $pythonVer" -ForegroundColor Green
} catch {
    Write-Host "python nie jest dostêpny. Mo¿e wymagaæ restartu konsoli." -ForegroundColor Red
}
try {
    $pipVer = & pip --version 2>&1
    Write-Host "pip --version    : $pipVer" -ForegroundColor Green
} catch {
    Write-Host "pip nie jest dostêpny. Upewnij siê, ¿e Scripts istnieje i zawiera pip.exe." -ForegroundColor Red
}

Write-Host "`nInstalacja zakoñczona. Mo¿esz teraz u¿ywaæ 'python' i 'pip' z dowolnego folderu." -ForegroundColor Green
Write-Host "Jeœli nie dzia³a w bie¿¹cej konsoli, otwórz nowe okno PowerShell." -ForegroundColor Yellow