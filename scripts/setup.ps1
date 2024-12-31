
function DownloadFile([string]$Url, [string]$Dest, [string]$Name) {

    # Download the file
    if (!(Test-Path $Dest)) {
        Write-Host "Downloading $Name..."
        Invoke-WebRequest $Url -OutFile $Dest

        # Remove incomplete file on download failure
        if (!$?) {
            Write-Host "Failed to download $Name."
            if (Test-Path $Dest) { Remove-Item $Dest }
            Pause
            Exit 1
        }
    }
}

function InstallPythonPackage([string]$PackageName) {
    pip show $PackageName -qqq
    if (!$?) {
        Write-Host "Installing $PackageName..."
        pip install $PackageName
        if (!$?) {
            Write-Host "Failed to install $PackageName."
            Pause
            Exit 1
        }
    }
}

function HasValidDotnetVersion() {

    # Check if dotnet is installed
    Get-Command dotnet -ErrorAction SilentlyContinue | Out-Null
    if (!$?) { return 0 }

    # Get the available runtimes
    $runtimes = dotnet --list-runtimes
    $versionPattern = "Microsoft\.NETCore\.App (\d+)\.(\d+)"

    # Check if at least one installed version matches
    foreach ($line in $runtimes) {
        if ($line -match $versionPattern) {
            if ([int]$matches[1] -ge 6) {
                return 1
            }
        }
    }

    # Otherwise return false
    return 0
}

# Check for Windows
if ([Environment]::OSVersion.Platform -ne "Win32NT") {
    Write-Host "This script can only be run on Windows. Exiting..."
    Pause
    Exit 1
}

# Set project root directory as current directory
$rootdir = ($MyInvocation.MyCommand.Path | Select-String -Pattern '(.*)\\scripts\\.*.ps1').Matches.Groups[1]
Set-Location $rootdir

# Make a temporary directory for file downloads
Write-Host "Creating temporary download directory..."
New-Item tmp -ItemType Directory -Force | Out-Null
Set-Location tmp

# Set variable
$logOffRequired = 0

# Check for .NET Core dependency
Write-Host "Checking for .NET Core 6.0 or higher..."
if (!(HasValidDotnetVersion)) {

    # Download .NET installation script
    DownloadFile -Url "https://dot.net/v1/dotnet-install.ps1" -Dest "dotnet-install.ps1" -Name ".NET installation script"

    # Install .NET 6.0 runtime
    Write-Host "Installing .NET Core 6.0 runtime..."
    .\dotnet-install.ps1 -Runtime dotnet -Channel 6.0
    if (!$?) {
        Write-Host "Failed to install .NET Core."
        Pause
        Exit 1
    }

    # Add install location to PATH and schedule logout
    [Environment]::SetEnvironmentVariable("PATH", [Environment]::GetEnvironmentVariable("PATH", "Machine") + ";$Env:LocalAppData\Microsoft\dotnet", [EnvironmentVariableTarget]::Machine)
    Write-Host ".NET Core installed successfully!"
    $logOffRequired = 1
}

# Check Python version
Write-Host "Checking for Python installation..."
Get-Command python -ErrorAction SilentlyContinue | Out-Null
if ($?) {
    python -c "import sys; exit_code = 0 if sys.version_info >= (3, 11) else 1; sys.exit(exit_code)"
}

# Install if any of the two checks failed
if (!$?) {

    # Download Python
    DownloadFile -Url "https://www.python.org/ftp/python/3.12.5/python-3.12.5-amd64.exe" -Dest "python-installer.exe" -Name "Python installer"

    # Install Python
    Write-Host "Running installer..."
    .\python-installer.exe /quiet InstallAllUsers=1 PrependPath=1 Include_tcltk=0 Include_test=0
    if (!$?) {
        Write-Host "Failed to install Python."
        Pause
        Exit 1
    }

    # Schedule logout
    Write-Host "Python installed successfully!"
    $logOffRequired = 1
}

# Check for wszst installation
Write-Host "Checking for Wiimm's SZS Tools installation..."
Get-Command wszst -ErrorAction SilentlyContinue | Out-Null
if (!$?) {

    # Download wszst
    DownloadFile -Url "https://szs.wiimm.de/download/szs-v2.42a-r8989-cygwin64.zip" -Dest "wszst.zip" -Name "Wiimm's SZS Tools installer"

    # Install wszst
    Expand-Archive wszst.zip .\ -Force
    Set-Location szs-v2.42a-r8989-cygwin64
    .\windows-install.exe
    if (!$?) {
        Write-Host "Failed to install Wiimm's SZS Tools."
        Pause
        Exit 1
    }

    # Schedule logout
    Write-Host "Wiimm's SZS Tools installed successfully!"
    $logOffRequired = 1
}

# Log out to refresh environment variables
if ($logOffRequired) {
    Write-Host "Logging out is required to continue the installation. Run this script again after logging back in to finalize the setup procedure. Make sure to save any unsaved data before continuing!"
    Pause
    shutdown -L
    Exit
}

# Check for CodeWarrior installation (both in local repository and PATH)
Write-Host "Checking for CodeWarrior installation..."
Get-Command mwcceppc -ErrorAction SilentlyContinue | Out-Null
if (!$? -and !(Test-Path "..\tools\cw\mwcceppc.exe")) {

    # Download CodeWarrior and ISXUnpack tool
    DownloadFile -Url "https://nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe" -Dest "CW55xx_v2_10_SE.exe" -Name "CodeWarrior installer"
    DownloadFile -Url "http://www.compdigitec.com/labs/files/isxunpack.exe" -Dest "isxunpack.exe" -Name "ISXUnpack tool"

    Write-Host "Unpacking CodeWarrior files..."
    Write-Output "" | .\isxunpack.exe CW55xx_v2_10_SE.exe | Out-Null

    # Extract the files
    New-Item out -ItemType Directory -Force
    expand.exe Data1.cab "out" -F:lmgr11.dll | Out-Null
    expand.exe Data1.cab "out" -F:_44CD8EA541F44E56801091477F3DC9AA | Out-Null
    expand.exe Data1.cab "out" -F:_4C8ADA37887647F5955B4FB0F716277F | Out-Null
    Move-Item "out\lmgr11.dll" "..\tools\cw\lmgr11.dll" -Force | Out-Null
    Move-Item "out\_44CD8EA541F44E56801091477F3DC9AA" "..\tools\cw\license.dat" -Force | Out-Null
    Move-Item "out\_4C8ADA37887647F5955B4FB0F716277F" "..\tools\cw\mwcceppc.exe" -Force | Out-Null

    # Remove temporary directory
    Remove-Item out -Recurse -Force
    Write-Host "CodeWarrior installed successfully!"
}

# Check for Kamek installation
Write-Host "Checking for Kamek installation..."
Get-Command Kamek -ErrorAction SilentlyContinue | Out-Null
if (!$? -and !(Test-Path "..\tools\kamek\Kamek.exe")) {

    # Download Kamek
    DownloadFile -Url "https://github.com/Treeki/Kamek/releases/download/2024-04-10_prerelease/kamek_2024-04-10_win-x64.zip" -Dest "kamek.zip" -Name "Kamek"

    # Install Kamek
    Write-Host "Installing Kamek..."
    Expand-Archive kamek.zip "..\tools\kamek" -Force
    if (!$?) {
        Write-Host "Failed to unpack Kamek archive."
        Pause
        Exit 1
    }

    Write-Host "Kamek installed successfully!"
}

# Check and eventually install ninja
Write-Host "Checking for ninja installation..."
Get-Command ninja -ErrorAction SilentlyContinue | Out-Null
if (!$?) {
    InstallPythonPackage -Package "ninja"
}

# Check and eventually install json5
Write-Host "Checking for json5 installation..."
InstallPythonPackage -Package "json5"

# Check and eventually install qtpy
"Checking for qtpy installation..."
InstallPythonPackage -Package "qtpy"

# Check for any of the qtpy dependencies
Write-Host "Checking for qtpy dependencies..."
pip show PyQt5 PySide2 PyQt6 PySide6 -qqq
if (!$?) {
    Write-Host "No Qt packages found. Installing PyQt5..."
    InstallPythonPackage -Package "PyQt5"
}

# Check for wuj5 installation
Write-Host "Checking for wuj5 installation..."
if (!(Test-Path "..\tools\wuj5\wuj5.py")) {

    # Download wuj5 and install it
    DownloadFile -Url "https://github.com/stblr/wuj5/archive/refs/heads/main.zip" -Dest "wuj5.zip" -Name "wuj5"
    Expand-Archive wuj5.zip .\ -Force
    xcopy wuj5-main ..\tools\wuj5\ /E /Y /Q
    Write-Host "wuj5 installed successfully!"
}

# Check for wuj5 installation
Write-Host "Checking for breffconv installation..."
if (!(Test-Path "..\tools\breffconv\breff_converter.py")) {

    # Download breffconv and install it
    DownloadFile -Url "https://github.com/CLF78/Actually-Working-BREFF-Converter/archive/refs/heads/master.zip" -Dest "breffconv.zip" -Name "breffconv"
    Expand-Archive breffconv.zip .\ -Force
    xcopy breffconv-master ..\tools\breffconv\ /E /Y /Q
    Write-Host "breffconv installed successfully!"
}

# Remove temporary directory
Set-Location "..\"
Remove-Item tmp -Recurse -Force

# Prompt the user to install extra development dependencies
do {
    $answer = Read-Host "Would you like to install additional development dependencies? (Default = No) [Y] Yes [N] No"
    $answer = $answer.ToUpper()
} while ($answer -ne "Y" -and $answer -ne "N" -and $answer -ne "")

if ($answer -eq "Y") {

    # Install clang-tidy
    Write-Host "Checking for clang-tidy..."
    Get-Command clang-tidy -ErrorAction SilentlyContinue | Out-Null
    if (!$?) {
        InstallPythonPackage -Package clang-tidy
    }

    # Install clang-format
    Write-Host "Checking for clang-format..."
    Get-Command clang-format -ErrorAction SilentlyContinue | Out-Null
    if (!$?) {
        InstallPythonPackage -Package clang-format
    }
}

# Installation finished
Write-Host "All done!"
Pause
