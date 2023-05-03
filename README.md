# Mario Kart Midnight 2
Mario Kart Midnight 2 is a WIP custom track distribution created by NuoKart and CLF78.

## Building the Distribution

### Required Tools and Dependencies
- [CodeWarrior for MPC55xx/MPC56xx v2.10 Special Edition](https://nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe) ([mirror](https://cache.nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe))
- [Kamek](https://github.com/Treeki/Kamek/releases)
- [Python](https://www.python.org/downloads/) (version 3.8 or later)
- [Wiimm's SZS Tools](https://szs.wiimm.de/download.html)
- [wuj5](https://github.com/stblr/wuj5)

The following Python packages are also required:
- [json5](https://pypi.org/project/json5/)
- [ninja](https://pypi.org/project/ninja/)

These additional packages are required for running the Cup Builder tool:
- One of the four Qt Python bindings:
    - [PyQt5](https://pypi.org/project/PyQt5/) (recommended)
    - [PySide2](https://pypi.org/project/PySide2/)
    - [PyQt6](https://pypi.org/project/PyQt6/)
    - [PySide6](https://pypi.org/project/PySide6/)
- [qtpy](https://pypi.org/project/QtPy/)

### Installation - Windows
- Clone this repository to a folder of choice (make sure the path to the folder has no spaces in it!);
- Install Python and the Python packages ([through `pip`](https://pip.pypa.io/en/stable/getting-started/));
- Install [.NET Runtime 6.0](https://dotnet.microsoft.com/en-us/download/dotnet/6.0) if not present, which is required for running Kamek;
- Run the CodeWarrior installer;
    - Only the `Command Line Tools` component is needed for compilation; this can be set by choosing the `Custom` install configuration;
    - Remember where the program was installed to, as it will be needed later!
- Install Wiimm's SZS Tools by executing `windows-install.exe` (it's recommended to restart the device after this step);
- Go to the cloned repository, go to the `tools` folder and create the following directories:
    - `cw`;
    - `kamek`;
- Go to the CodeWarrior installation directory and copy the contents of the `PowerPC_EABI_Tools/Command_Line_Tools` folder to the `cw` folder;
    - Copy `license.dat` from the main CodeWarrior directory to the `cw` folder as well;
- Download Kamek and copy it to the `kamek` folder;
- Download wuj5 and copy it to the `wuj5` folder;

### Installation - Mac
I'm not covering this. Figure it out yourself.

### Installation - Linux
- Clone this repository to a folder of choice (make sure the path to the folder has no spaces in it!);
- Install Python and the Python packages ([through `pip`](https://pip.pypa.io/en/stable/getting-started/));
- Install [.NET Runtime 6.0](https://dotnet.microsoft.com/en-us/download/dotnet/6.0), which is required for running Kamek;
- Install [WINE](https://wiki.winehq.org/Download), which is required for running CodeWarrior;
- Since the CodeWarrior installer will fail on WINE, the files will need to be extracted manually:
    - To do so, first make sure the [cabextract](https://www.cabextract.org.uk/) package is installed;
    - Place the CodeWarrior installer in the same folder as `extractCW.sh`, then run the aforementioned script;
    - All the files will be placed to the correct folder automatically.
- Install Wiimm's SZS Tools;
- Go to the cloned repository, go to the `tools` folder and create a new directory named `kamek`;
- Download Kamek and copy it to the `kamek` folder;
- Download wuj5 and copy it to the `wuj5` folder;

### Building
- Run `configure.py` to create the Ninja build script;
    - The script accepts a `--clean` argument to remove the existing `build` and `out` folders;
    - This needs to be run every time a source code/asset file is added/removed.
- Run `ninja` to build the distribution.
    - The output will be placed in the `out` folder.
    - This needs to be run every time a source code/asset file is added/updated/removed. Ninja will not re-run build steps unaffected by the changes.

## Customization
The distribution can be modified for your own purposes in multiple ways.

### Cup Layout
The cup icons/layout and the tracks within can be changed using the Cup Builder Tool.
- Ensure the additional dependencies for the tool are installed;
- Run `tools/cup_builder/main.py` to open the tool;
- Open `assets/cups.json5` to access Midnight's own track roster.

A few usage notes:
- The file's validity can be checked with the corresponding menu option.
    - This is run again during the build process, which will fail should errors be detected.
- If the cup file is moved to a different directory, the file will have to be updated to ensure the paths still match;
- The cup file stores track, music and icon hashes internally so that Ninja can detect changes. If one of those elements is updated, the cup file will need to be opened and resaved so the internal hashes match the new files;

### Mod Features
The various features in the mod (including the aforementioned custom cups) can be toggled and further configured by editing `include/config.h`. While most features are free standing, some are dependent on others; checks are included to prevent most breakages.

### Assets
Most of the assets in the mod (excluding tracks and music due to file size reasons) are available in decoded form in the `assets` folder. They can be changed, added or removed, provided the `configure.py` script is updated to match.
