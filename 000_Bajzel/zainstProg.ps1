Write-Host "Rozpoczynam zbieranie danych o zainstalowanych aplikacjach. To mo¿e chwilê potrwaæ..." -ForegroundColor Cyan
Write-Host "----------------------------------------------------------------------------------"

# Œcie¿ki rejestru do sprawdzenia
$regPaths = @(
    "HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*",
    "HKLM:\Software\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*",
    "HKCU:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*"
)

# Pobranie unikalnych aplikacji maj¹cych nazwê wyœwietlan¹
$apps = Get-ItemProperty $regPaths -ErrorAction SilentlyContinue | 
        Where-Object { $_.DisplayName -and $_.SystemComponent -ne 1 } | 
        Select-Object DisplayName, InstallDate, InstallLocation -Unique

$results = @()

foreach ($app in $apps) {
    $folderSizeMB = "Nieznany"
    
    # Próba obliczenia rozmiaru folderu, jeœli œcie¿ka instalacji istnieje
    if ($app.InstallLocation -and (Test-Path $app.InstallLocation)) {
        try {
            $sizeInBytes = (Get-ChildItem -Path $app.InstallLocation -Recurse -File -ErrorAction SilentlyContinue | 
                            Measure-Object -Property Length -Sum).Sum
            if ($sizeInBytes) {
                $folderSizeMB = [math]::Round($sizeInBytes / 1MB, 2)
            } else {
                $folderSizeMB = 0
            }
        } catch {
            $folderSizeMB = "Brak dostêpu"
        }
    }

    # Formatowanie daty (czêsto w rejestrze zapisana jako RRRRMMDD)
    $formattedDate = $app.InstallDate
    if ($app.InstallDate -match "^\d{8}$") {
        $formattedDate = [datetime]::ParseExact($app.InstallDate, "yyyyMMdd", $null).ToString("yyyy-MM-dd")
    }

    # Tworzenie obiektu wyjœciowego
    $results += [PSCustomObject]@{
        "Nazwa Programu"      = $app.DisplayName
        "Data Instalacji"     = if ($formattedDate) { $formattedDate } else { "Brak danych" }
        "Katalog Instalacji"  = if ($app.InstallLocation) { $app.InstallLocation } else { "Brak danych" }
        "Rozmiar Folderu (MB)" = $folderSizeMB
    }
}

# Wyœwietlenie wyników w postaci czytelnej, dopasowanej tabeli
$results | Sort-Object "Nazwa Programu" | Format-Table -AutoSize

Write-Host "----------------------------------------------------------------------------------"
Write-Host "Wyszukiwanie zakoñczone. Znaleziono $($results.Count) aplikacji." -ForegroundColor Cyan
