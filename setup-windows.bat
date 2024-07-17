@echo off

:: Check for Admin Privileges
ECHO Checking for admin privileges...
openfiles >nul 2>&1
IF NOT %ErrorLevel% EQU 0 (
    ECHO Please run this script with admin privileges.
    pause
    EXIT 1
)

:: Check if cURL is present
ECHO Checking for cURL...
WHERE /q curl
IF NOT %ErrorLevel% EQU 0 (
    ECHO cURL is not installed on this version of Windows. Please refer to the manual installation guide.
    pause
    EXIT 1
)

:: Check .NET version
ECHO Checking for .NET Core...
WHERE /q dotnet
IF NOT %ErrorLevel% EQU 0 (
    ECHO .NET Core 6.0 or higher is required to run Kamek. Please install it before proceeding.
    pause
    EXIT 1
)

:: Set project root directory as CWD
SET "ROOTDIR=%~dp0"
CD /D "%ROOTDIR%"

:: Make temporary directory for file downloads
ECHO Creating temporary download directory...
SET "TMPDIR=tmp"
MD "%TMPDIR%" >nul 2>&1
CD /D "%TMPDIR%"

:: Set variable
SET "LOGOFFREQUIRED=0"

:: Check Python version
ECHO Checking for Python installation...
python -c "import sys; exit_code = 0 if sys.version_info >= (3, 8) else 1; sys.exit(exit_code)" >nul 2>&1
IF NOT %ErrorLevel% EQU 0 (
    :: Download Python
    ECHO Python missing or outdated! Downloading installer...
    IF NOT EXIST "python-installer.exe" (
        curl -L -o "python-installer.exe" "https://www.python.org/ftp/python/3.12.1/python-3.12.1-amd64.exe"
        IF NOT %ErrorLevel% EQU 1 (
            ECHO Failed to download Python installer.
            pause
            EXIT 1
        )
    )

    :: Install it
    ECHO Running installer...
    python-installer.exe /quiet InstallAllUsers=1 PrependPath=1 Include_tcltk=0 Include_test=0
    IF NOT %ErrorLevel% EQU 0 (
        ECHO Failed to install Python.
        pause
        EXIT 1
    )

    :: Schedule logout
    ECHO Python installed successfully!
    SET "LOGOFFREQUIRED=1"
)

:: Check for WSZST installation
ECHO Checking for Wiimm's SZS Tools installation...
WHERE /q wszst
IF NOT %ErrorLevel% EQU 0 (
    :: Download WSZST
    IF NOT EXIST "wszst.zip" (
        curl -L -o "wszst.zip" "https://szs.wiimm.de/download/szs-v2.39a-r8904-cygwin64.zip"
        IF NOT %ErrorLevel% EQU 1 (
            ECHO Failed to download WSZST Installer.
            pause
            EXIT 1
        )
    )

    :: Install it
    tar -xkf wszst.zip
    CD szs-v2.39a-r8904-cygwin64
    windows-install.exe
    ECHO Wiimm's SZS Tools installed successfully!
    SET "LOGOFFREQUIRED=1"
)

:: Log out to refresh environment variables
IF %LOGOFFREQUIRED% EQU 1 (
    ECHO Logging out is required to continue the installation. Run this script again after logging back in to finalize the setup procedure.
    pause
    shutdown -L
    exit
)

:: Check for CodeWarrior installation (both in local repository and PATH)
ECHO Checking for CodeWarrior installation...
IF EXIST "..\tools\cw\mwcceppc.exe" (
    GOTO cwInstalled
)

WHERE /q mwcceppc
IF %ErrorLevel% EQU 0 (
    GOTO cwInstalled
)

:: Download CodeWarrior
ECHO CodeWarrior not installed! Proceeding with installation...
IF NOT EXIST "CW55xx_v2_10_SE.exe" (
    curl -L -o "CW55xx_v2_10_SE.exe" "https://nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe"
    IF NOT %ErrorLevel% EQU 1 (
        ECHO Failed to download CodeWarrior installer.
        pause
        EXIT 1
    )
)

IF NOT EXIST "isxunpack.exe" (
    curl -L -o "isxunpack.exe" "http://www.compdigitec.com/labs/files/isxunpack.exe"
    IF NOT %ErrorLevel% EQU 0 (
        ECHO Failed to download ISXUnpack tool.
        pause
        EXIT 1
    )
)

:: Unpack the installer
ECHO Unpacking CodeWarrior files...
echo. | isxunpack.exe CW55xx_v2_10_SE.exe >nul 2>&1

:: Move necessary files
SET "CWOUTDIR=out"
MD "%CWOUTDIR%" >nul 2>&1
expand Data1.cab "%CWOUTDIR%" -F:lmgr11.dll >nul 2>&1
expand Data1.cab "%CWOUTDIR%" -F:_44CD8EA541F44E56801091477F3DC9AA >nul 2>&1
expand Data1.cab "%CWOUTDIR%" -F:_4C8ADA37887647F5955B4FB0F716277F >nul 2>&1
move "%CWOUTDIR%\lmgr11.dll" "..\tools\cw" >nul 2>&1
move "%CWOUTDIR%\_44CD8EA541F44E56801091477F3DC9AA" "..\tools\cw\license.dat" >nul 2>&1
move "%CWOUTDIR%\_4C8ADA37887647F5955B4FB0F716277F" "..\tools\cw\mwcceppc.exe" >nul 2>&1

:: Remove temporary directory
ECHO CodeWarrior installed successfully!
rmdir /s /q "%CWOUTDIR%"

:: Check for Kamek installation (both in local repository and PATH)
:cwInstalled
ECHO Checking for Kamek installation...
IF EXIST "..\tools\kamek\Kamek.exe" (
    GOTO kamekInstalled
)

WHERE Kamek >nul 2>&1
IF %ErrorLevel% EQU 0 (
    GOTO kamekInstalled
)

:: Download Kamek
ECHO Kamek not installed! Proceeding with installation...
MD ..\tools\kamek
IF NOT EXIST "kamek.zip" (
    curl -L -o "kamek.zip" "https://github.com/Treeki/Kamek/releases/download/2024-04-10_prerelease/kamek_2024-04-10_win-x64.zip"
    IF NOT %ErrorLevel% EQU 1 (
        ECHO Failed to download Kamek archive.
        pause
        EXIT 1
    )
)

:: Install Kamek
ECHO Installing Kamek...
tar -xkf kamek.zip -C "..\tools\kamek"
IF NOT %ErrorLevel% EQU 0 (
    ECHO Failed to unpack Kamek archive.
    pause
    EXIT 1
)

ECHO Kamek installed successfully!

:: Check and eventually install ninja, json5 and qtpy
:kamekInstalled
ECHO Checking for ninja installation...
pip show ninja >nul 2>&1
IF NOT %ErrorLevel% EQU 0 (
    ECHO Installing ninja...
    pip install ninja
)

ECHO Checking for json5 installation...
pip show json5 >nul 2>&1
IF NOT %ErrorLevel% EQU 0 (
    ECHO Installing json5...
    pip install json5
)

ECHO Checking for qtpy installation...
pip show qtpy >nul 2>&1
IF NOT %ErrorLevel% EQU 0 (
    ECHO Installing qtpy...
    pip install qtpy
)

:: Check for any of the qtpy dependencies
ECHO Checking qtpy dependencies...
pip show PyQt5 >nul 2>&1
IF %ErrorLevel% EQU 0 (
    ECHO PyQt5 detected!
    GOTO qtDepsInstalled
)

pip show PySide2 >nul 2>&1
IF %ErrorLevel% EQU 0 (
    ECHO PySide2 detected!
    GOTO qtDepsInstalled
)

pip show PyQt6 >nul 2>&1
IF %ErrorLevel% EQU 0 (
    ECHO PyQt6 detected!
    GOTO qtDepsInstalled
)

pip show PySide6 >nul 2>&1
IF %ErrorLevel% EQU 0 (
    ECHO PySide6 detected!
    GOTO qtDepsInstalled
)

ECHO No Qt libraries found. Installing PyQt5...
pip install PyQt5

:: Check for wuj5 installation
:qtDepsInstalled
ECHO Checking wuj5 installation...
IF NOT EXIST ..\tools\wuj5\wuj5.py (
    :: Download wuj5
    IF NOT EXIST "wuj5.zip" (
        curl -L -o "wuj5.zip" "https://github.com/stblr/wuj5/archive/refs/heads/main.zip"
        IF NOT %ErrorLevel% EQU 1 (
            ECHO Failed to download wuj5.
            pause
            EXIT 1
        )
    )

    :: Install wuj5
    tar -xkf wuj5.zip
    xcopy wuj5-main ..\tools\wuj5 /E /Y /Q >nul 2>&1
    ECHO wuj5 installed successfully!
)

:: Remove temporary directory
ECHO Removing temporary installation files...
CD ..
rmdir /s /q "%TMPDIR%"

:: Installation complete!
ECHO All done!
pause
