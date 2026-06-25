Write-Host "Rozpoczynam wyszukiwanie instalacji Pythona..." 
Write-Host "--------------------------------------------------"

# 1. Sprawdzenie tradycyjnych instalacji w rejestrze systemowym
Write-Host "[1/4] Sprawdzanie rejestru systemowego (Instalatory EXE)..." 
$regPaths = @(
    "HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*",
    "HKCU:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*",
    "HKLM:\Software\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*"
)
Get-ItemProperty $regPaths -ErrorAction SilentlyContinue | 
    Where-Object { $_.DisplayName -match "Python" -and $_.DisplayName -notmatch "Launcher" } | 
    Select-Object DisplayName, DisplayVersion, InstallLocation | 
    Format-Table -AutoSize

# 2. Sprawdzenie aplikacji z Microsoft Store
Write-Host "[2/4] Sprawdzanie aplikacji ze sklepu Microsoft Store..." 
Get-AppxPackage -AllUsers *Python* -ErrorAction SilentlyContinue | 
    Select-Object Name, Version | 
    Format-Table -AutoSize

# 3. Sprawdzenie aktywnych ścieżek w zmiennej środowiskowe PATH (gdzie system widzi polecenie 'python')
Write-Host "[3/4] Sprawdzanie aktywnych programów w zmiennej PATH..." 
Get-Command python -All -ErrorAction SilentlyContinue | 
    Select-Object Name, Version, Source | 
    Format-Table -AutoSize

# 4. Sprawdzenie domyślnych folderów instalacyjnych (na wypadek instalacji 'portable' / bez rejestru)
Write-Host "[4/4] Sprawdzanie domyślnych folderów użytkownika..." 
$userPaths = @(
    "$env:LocalAppData\Programs\Python",
    "$env:AppData\Python"
)
foreach ($path in $userPaths) {
    if (Test-Path $path) {
        Write-Host "Znaleziono folder Pythona w: $path" 
        Get-ChildItem $path -Directory | Select-Object Name, FullName | Format-Table -AutoSize
    }
}

Write-Host "--------------------------------------------------"
Write-Host "Wyszukiwanie zakoNczone." 
