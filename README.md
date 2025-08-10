# Midnight x Variety Pack
Midnight x Variety Pack (or MVP for short) is a discontinued custom track distribution created by NuoKart, CLF78 and Brawlboxgaming.

> [!WARNING]
> The project has been abandoned. Use it at your own risk.

## Table of Contents
- [Feature List](#feature-list)
   * [Custom Track Support](#custom-track-support)
   * [Sound / Music Customization](#sound--music-customization)
   * [Network / Online Enhancements](#network--online-enhancements)
   * [Gameplay Enhancements](#gameplay-enhancements)
      + [Drift Changes](#drift-changes)
      + [Engine Class Changes](#engine-class-changes)
      + [Offline Modes Changes](#offline-modes-changes)
      + [Item Changes](#item-changes)
      + [Other Gameplay Enhancements](#other-gameplay-enhancements)
   * [User Interface Enhancements](#user-interface-enhancements)
   * [Other Enhancements](#other-enhancements)
   * [Development / Building Enhancements](#development--building-enhancements)
- [Project Setup - Automatic (Windows Only)](#project-setup---automatic-windows-only)
- [Project Setup - Manual](#project-setup---manual)
   * [Required Tools and Dependencies](#required-tools-and-dependencies)
      + [Installation - Windows](#installation---windows)
      + [Installation - Mac](#installation---mac)
      + [Installation - Linux](#installation---linux)
- [Building](#building)
- [Customization](#customization)
   * [Cup Layout](#cup-layout)
   * [Mod Features](#mod-features)
   * [Assets](#assets)
- [Contributing](#contributing)
   * [Setting up the Development Environment](#setting-up-the-development-environment)
   * [Visual Studio Code-Specific Features](#visual-studio-code-specific-features)

## Feature List
### Custom Track Support
* **Custom Cup System**: Add up to 65534 cups and 32764 tracks (memory limits apply). No support for original Nintendo tracks.
* **Multiple Cup Lists**: Define multiple cup lists and switch between them in-game *(feature incomplete)*.
* **Battle Mode Support**: Battle mode is fully supported with 4 arenas per cup.
* **Track Variant Support**: Support for randomly-picked track variants, with customizable probabilities.
* **Cup Generator Tool**: Supporting tool included for defining cup order and bundling track files, music, and names.

### Sound / Music Customization
* **Per-Track Custom Music**: Assign unique music to each track, with optional last-lap variants. If no last-lap music is provided, the standard track music is pitched up.
* **Prevent Muting on Missing Channels**: Automatically avoids muting if additional music channels are missing.
* **Automatic BRSAR Patching**: Prevents muting custom music due to different loop points.
* **Sound Effect Expansion**: Replace sound effects with custom BRSTMs (ported from MKW-SP).
* **Toad’s Factory Music Slot Fixes**: Prevent music reset and remove hardcoded start point in Toad’s Factory music slot.

### Network / Online Enhancements
* **Wiimmfi Support**: Connect to Wiimmfi via OpenPayload, with fixes ported from the WiiLink24 WFC patcher.
* **Extended Network Protocol**: Expand any packet section to allow including custom data.
* **Track Vote Cooldown**: Blocks voting for the last 32 tracks played.
* **Custom Online Time Limit**: Configurable maximum race time for online sessions.
* **Friendless Room Creation**: Create rooms without registered friends *(from WiiLink24)*.
* **Improved Online Error Display**: Provide clearer Wiimmfi error descriptions and shows error codes on fatal crashes for debugging.

### Gameplay Enhancements

#### Drift Changes
* **Transmission Selector**: Choose inside or outside drift for each vehicle, with default drift selection for each vehicle saved separately.
* **Purple and Orange Miniturbos**: Outside drift karts can charge purple miniturbos, while outside drift bikes can charge an orange miniturbo.

#### Engine Class Changes
* **200cc / 500cc Support**: Partial support for higher CCs. *Note: fast falling or item speed patches are not implemented.*
* **Custom CC Odds**: Each cup list can have its own CC chance distribution online *(incomplete)*.
* **Independent Mirror Mode**: Mirror mode is handled separately from CC in online play.

#### Offline Modes Changes
* **Grand Prix Updates**:
    * Enforced 150cc mode, with rank calculation based purely on score.
    * Revised star rankings (3/2/1 stars, golden/silver/bronze cup, L rank)
    * Credits roll upon completion of all GPs.
* **Time Trial Modifications**: CC selection (150cc, 200cc, 500cc) added for Time Trials.
* **VS Modifications**: Support for both random and sequential track order.

#### Item Changes
* **Mega Thundercloud**: Gives the Thundercloud a Mega Mushroom effect. Includes a custom model by Atlas.
* **Prevent Replacing Bomb Damage with Spinouts Online**: Ensures that bomb damage actually throws players into the air in online play.
* **Draggable Blue Shell**: Allows players to drag the Blue Shell.
* **Disguised Fake Item Boxes**: Fake Item Boxes look like regular item boxes.

#### Other Gameplay Enhancements
* **Conditional Out of Bounds**: Allows track authors to define out-of-bounds areas based on the player's checkpoint.
* **Lap Modifier**: Allows for custom lap progression.

### User Interface Enhancements
* **Page Expansion System**: Supports adding new UI pages to the game without replacing any existing ones, offering extensive modding flexibility.
* **Message Queue**: In-race message display system for track/music credits and other in-game information. The UI is reorganized to show the queue in the bottom left corner.
* **Track Selection Anti-Repick**: Warns players if they select a track that cannot be voted, with affected tracks highlighted in red.
* **Always Display Countdown Timer**: Always show the countdown timer when matchmaking or voting online.
* **VR Screen Enhancements**: Add a change character button and a rule explanation button (incomplete).
* **Online Menu Changes**: Remove the "Worldwide" button in the online menu and add one region selector per cup list *(incomplete; visual only)*.

### Other Enhancements
* **Effect Expansion System**: Load additional BREFF/BREFT files anywhere and without sacrificing track effect slots in races.
* **Savegame Expansion**: Modular system for storing additional save data. Currently used for rankings, transmission selection, and rating for each online mode.
* **Unlock Everything Without Affecting Savegame**: Unlock all items, characters, and vehicles without modifying the original savegame data.

### Development / Building Enhancements
* **Patches and Modifications**: All patches are fully documented and explained, with proper credits provided where applicable.
* **Resource Overrides**: Support for mod-level and user-level overrides of menu/common `.szs` files without bundling original assets.
* **Enhanced Conditional BMG Escape Sequences**: Allow the use of conditional escape sequences in all languages and automatically prepend a space for text formatting.
* **Enhanced Exception Handler**: Display the exception handler with a configurable message and additional enhancements (ported from MKW-SP).
* **Modified Kamek Loader**: Implements MKW-SP-style function replacement for more advanced modding capabilities. *(Warning: hacky implementation.)*
* **Extensive Debug Logging System**: The mod includes a powerful debug logging system, providing detailed logs to assist in debugging and development.
* **Automated Build Pipeline**:
  Robust build pipeline powered by Ninja to automate several development tasks:
  * **Source File Compilation**.
  * **Menu Asset Packaging**: Automatically packages BRCTR, BRLYT, BMG, BREFF, BRRES files using WSZST.
  * **Cup Code Generation**: Generates cup code directly from a configuration file.
  * **Riivolution XML Generation**: Automatically creates Riivolution XML files.
  * **Dolphin Preset Generation**: Provides quick launch configuration for Dolphin emulator.

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
