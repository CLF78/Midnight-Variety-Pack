# Midnight x Variety Pack
Midnight x Variety Pack (or MVP for short) is a WIP custom track distribution created by NuoKart, CLF78 and Brawlboxgaming.

## Table of Contents
- [Project Setup - Automatic (Windows Only)](#project-setup---automatic-windows-only)
- [Project Setup - Manual](#project-setup---manual)
    - [Required Tools and Dependencies](#required-tools-and-dependencies)
    - [Installation - Windows](#installation---windows)
    - [Installation - Mac](#installation---mac)
    - [Installation - Linux](#installation---linux)
- [Building](#building)
- [Customization](#customization)
  - [Cup Layout](#cup-layout)
  - [Mod Features](#mod-features)
  - [Assets](#assets)
- [Contributing](#contributing)
  - [Setting up the Development Environment](#setting-up-the-development-environment)
  - [Visual Studio Code-Specific Features](#visual-studio-code-specific-features)

## Project Setup - Automatic (Windows Only)
- Clone this repository to a folder of choice (make sure the path to the folder has no spaces in it!);
- Run `setup-windows.bat` (this will download and install all the project's required dependencies, so an internet connection is required)
- The script will automatically log out of the current session to refresh the environment variables. You can run it again to continue the installation process.

## Project Setup - Manual
### Required Tools and Dependencies
- [CodeWarrior for MPC55xx/MPC56xx v2.10 Special Edition](https://nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe) ([mirror](https://cache.nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe))
- [Kamek](https://github.com/Treeki/Kamek/releases) (use the latest build as it introduces/fixes features that the mod depends on)
- [.NET Core](https://dotnet.microsoft.com/en-us/download/dotnet) (version 6.0 or later)
- [Python](https://www.python.org/downloads/) (version 3.11 or later)
- [Wiimm's SZS Tools](https://szs.wiimm.de/download.html)
- [Actually Working BREFF Converter](https://github.com/CLF78/Actually-Working-BREFF-Converter)
- [wuj5](https://github.com/stblr/wuj5)

The following Python packages are also required:
- [json5](https://pypi.org/project/json5/)
- [orjson](https://pypi.org/project/orjson/)
- [ninja](https://pypi.org/project/ninja/)

These additional packages are required for running the Cup Builder tool:
- One of the four Qt Python bindings:
    - [PyQt5](https://pypi.org/project/PyQt5/) (recommended)
    - [PySide2](https://pypi.org/project/PySide2/)
    - [PyQt6](https://pypi.org/project/PyQt6/)
    - [PySide6](https://pypi.org/project/PySide6/)
- [qtpy](https://pypi.org/project/QtPy/)

The following dependencies are additionally required on Linux:
- [7-Zip](https://www.7-zip.org/) (or any equivalent package that provides the `7z` tool)
- [WINE](https://wiki.winehq.org/Download)

#### Installation - Windows
- Clone this repository to a folder of choice (make sure the path to the folder has no spaces in it!);
- Install Python and the Python packages ([through `pip`](https://pip.pypa.io/en/stable/getting-started/));
- Install .NET Runtime 6.0 if not present, which is required for running Kamek;
- Run the CodeWarrior installer;
    - Only the `Command Line Tools` component is needed for compilation; this can be set by choosing the `Custom` install configuration;
    - Remember where the program was installed to, as it will be needed later!
- Install Wiimm's SZS Tools by executing `windows-install.exe` (it's recommended to restart the device after this step);
- Go to the CodeWarrior installation directory and copy the contents of the `PowerPC_EABI_Tools/Command_Line_Tools` folder to the `cw` folder inside the `tools` folder;
    - Copy `license.dat` from the main CodeWarrior directory to the `cw` folder as well;
- Download Kamek and copy it to the `kamek` folder (inside `tools`);
- Download wuj5 and copy it to the `wuj5` folder (unless already present);
- Download Actually Working BREFF Converter and copy it to the `breffconv` folder (unless already present);

#### Installation - Mac
I'm not covering this. Figure it out yourself.

#### Installation - Linux
- Clone this repository to a folder of choice (make sure the path to the folder has no spaces in it!);
- Install Python and the Python packages ([through `pip`](https://pip.pypa.io/en/stable/getting-started/));
- Install dotnet, which is required for running Kamek;
- Install WINE, which is required for running CodeWarrior;
- Since the CodeWarrior installer will fail on WINE, the files will need to be extracted manually:
    - To do so, first make sure the cabextract package is installed;
    - Place the CodeWarrior installer in the same folder as `extractCW.sh`, then run the aforementioned script;
    - All the files will be placed to the correct folder automatically.
- Install Wiimm's SZS Tools;
- Download Kamek and copy it to the `kamek` folder inside `tools`;
- Download wuj5 and copy it to the `wuj5` folder (unless already present);
- Download Actually Working BREFF Converter and copy it to the `breffconv` folder (unless already present);

## Building
- Run `configure.py` to create the Ninja build script;
    - Pass `--clean` to remove the existing `build` and `out` folders;
    - Pass `--loglevel=XXXX` to regulate the logs generated by the custom code (use `--help` to get the possible values);
    - Pass `--game-path=PATH` to generate a Game Mod Preset for Dolphin Emulator, using the given Mario Kart Wii copy;
    - This script needs to be run every time a source code/asset file is added/removed.
- Run `ninja` to build the distribution.
    - Ninja needs to be run as administrator on Windows to ensure successful compilation.
    - The output will be placed in the `out` folder.
    - This needs to be run every time a source code/asset file is added/updated/removed. Ninja will not re-run build steps unaffected by the changes.

## Customization
The distribution can be modified for your own purposes in multiple ways.

### Cup Layout
The cup icons/layout and the tracks within can be changed using the Cup Builder Tool.
- Ensure the additional dependencies for the tool are installed;
- Run `tools/cup_builder/main.py` to open the tool;
- Open `assets/course/data.json5` to access MVP's own track roster.

A few usage notes:
- The file's validity can be checked with the corresponding menu option.
    - This is run again during the build process, which will fail should errors be detected.
- If the cup file is moved to a different directory, the file will have to be updated to ensure the paths still match;
- Ninja will re-run the cup data exporter if either the track roster file or the assets linked to it are updated. In the latter case, Ninja will only detect the change if the updated asset's modification date is newer.

### Mod Features
The various features in the mod (including the aforementioned custom cups) can be toggled and further configured by editing `src/common/mvp/Config.h`. While most features are free standing, some are dependent on others; checks are included to prevent most breakages.

### Assets
Most of the assets in the mod (excluding tracks and music due to file size reasons) are available in (mostly) decoded form in the `assets` folder. They can be changed, added or removed, provided the `configure.py` script is updated to match.

## Contributing
Thanks for considering contributing to the project! The development team currently employs a specific set of tools to ensure consistency and cross-platform support.

### Setting up the Development Environment
The following dependencies are not strictly necessary, but will be used to analyze any contributed code, therefore their installation is recommended:
- [clang-tidy](https://pypi.org/project/clang-tidy/) to detect bugs and ensure code cleanliness;
- [clang-format](https://pypi.org/project/clang-format/) to ensure consistent code formatting.

These tools are part of the [LLVM](https://llvm.org/) project. If you'd rather avoid bloating your system with the full installation, the tools can be installed individually using the linked Python packages.

### Visual Studio Code-Specific Features
Since the authors work on the project using [Visual Studio Code](https://code.visualstudio.com/), various dedicated integrations are included to improve the development experience:
- Several [tasks](https://code.visualstudio.com/Docs/editor/tasks) are provided to build the project and perform code checks. These require the [Command Variable](https://marketplace.visualstudio.com/items?itemName=rioj7.command-variable) extension to work correctly;
- The [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) extension is used for code completion (Intellisense) and to run `clang-tidy` in the background. Various definitions have been added to the code to avoid tripping `clangd` on nonexistent errors.
    - [Microsoft's C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) must be disabled to avoid Intellisense conflicts.
- The [clang-format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) extension is used to call `clang-format` automatically whenever a code file is saved.
