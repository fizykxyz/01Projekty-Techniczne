# 1. Usuwanie wersji zainstalowanych przez tradycyjne instalatory exe
$pythons = Get-Package | Where-Object { $_.Name -match "Python" -and $_.Name -notmatch "Launcher" }
foreach ($py in $pythons) {
    Write-Host "Odinstalowywanie: $($py.Name)"
    Uninstall-Package -InputObject $py -Force
}

# 2. Usuwanie wersji ze sklepu Microsoft Store
Get-AppxPackage *Python* | Remove-AppxPackage -AllUsers

# 3. Czyszczenie pozostałości w folderach użytkownika
Remove-Item -Recurse -Force "$env:LocalAppData\Programs\Python" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "$env:AppData\Python" -ErrorAction SilentlyContinue
