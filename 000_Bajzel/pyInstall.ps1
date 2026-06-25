# Zdefiniuj funkcjê do pobierania listy wersji
function Get-AvailablePythonVersions {
    $ftpUrl = "https://www.python.org/ftp/python/"
    $response = Invoke-WebRequest -Uri $ftpUrl -UseBasicParsing
    # Wyci¹gnij linki do katalogów
    $links = $response.Links | Where-Object { $_.href -match '^[0-9]+\.[0-9]+\.[0-9]+/$' } | Select-Object -ExpandProperty href
    $versions = @()
    foreach ($link in $links) {
        $version = $link.TrimEnd('/')
        $installerUrl = "$ftpUrl$version/python-$version-amd64.exe"
        # SprawdŸ, czy plik istnieje (HEAD request)
        try {
            $request = [System.Net.WebRequest]::Create($installerUrl)
            $request.Method = "HEAD"
            $response2 = $request.GetResponse()
            if ($response2.StatusCode -eq 200) {
                $versions += [PSCustomObject]@{Version = $version; Url = $installerUrl}
            }
            $response2.Close()
        } catch {
            # Brak instalatora dla tej wersji (np. tylko Ÿród³a)
        }
    }
    # Sortuj wersje malej¹co (wed³ug Version)
    $versions = $versions | Sort-Object { [version]$_.Version } -Descending
    return $versions
}

# Wyœwietl listê
$versions = Get-AvailablePythonVersions
if ($versions.Count -eq 0) {
    Write-Error "Nie znaleziono ¿adnych wersji Pythona z instalatorami."
    exit 1
}
Write-Host "Dostêpne wersje Pythona:"
for ($i=0; $i -lt $versions.Count; $i++) {
    Write-Host "$($i+1). $($versions[$i].Version)"
}
$choice = Read-Host "Wybierz numer wersji do instalacji"
$index = [int]$choice - 1
if ($index -lt 0 -or $index -ge $versions.Count) {
    Write-Error "Nieprawid³owy wybór."
    exit 1
}
$selected = $versions[$index]
$installerUrl = $selected.Url
$version = $selected.Version
$targetDir = "C:\Python\Python$version"  # np. C:\Python\Python3.14.0
# Pobierz instalator do tymczasowego pliku
$tempInstaller = [System.IO.Path]::GetTempFileName() + ".exe"
Write-Host "Pobieranie instalatora z $installerUrl ..."
Invoke-WebRequest -Uri $installerUrl -OutFile $tempInstaller
# Uruchom instalator
$arguments = "/quiet InstallAllUsers=1 TargetDir=`"$targetDir`" PrependPath=0 AssociateFiles=0"
Write-Host "Instalowanie Pythona $version do $targetDir ..."
$process = Start-Process -FilePath $tempInstaller -ArgumentList $arguments -Wait -PassThru -NoNewWindow
if ($process.ExitCode -eq 0) {
    Write-Host "Instalacja zakoñczona pomyœlnie."
    # Opcjonalnie: dodanie do PATH? Nie robimy.
    # Dodatkowo: mo¿na dodaæ skrót lub zrobiæ coœ jeszcze.
} else {
    Write-Error "Instalacja nie powiod³a siê. Kod b³êdu: $($process.ExitCode)"
}
# Usuñ tymczasowy plik
Remove-Item $tempInstaller -Force