# Mario Kart Midnight 2
Mario Kart Midnight 2 is a WIP custom track distribution created by NuoKart, ZPL and CLF78.

## Building Source Code

### Premises
* The guides below assume a certain level of savviness. Don't know how to do something? Look it up.
* Don't like how the folder structure is laid out? Change it.

### Requirements
- [CodeWarrior for MPC55xx/MPC56xx v2.10 Special Edition](https://nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe)
- [Kamek](https://github.com/Treeki/Kamek/releases) (pick according to your OS)

### Installation - Windows
* Clone the GitHub repo to a folder of choice;
* Install `.NET 5.0` or `.NET 6.0` if not already present, as it is required by Kamek;
* Add Kamek to your `PATH` variable (restarting your device may be necessary in order for the change to be detected);
    - If this isn't possible, then edit the build scripts to match Kamek's path.
* Run the CodeWarrior installer;
    - Only the `Command Line Tools` component is needed for compilation; you can set this in the custom install configuration.
* Go to the cloned GitHub repo and make a folder named `cw`;
* Go to the CodeWarrior installation directory and copy the contents of the `PowerPC_EABI_Tools\Command_Line_Tools` folder to the `cw` folder;
* Copy `license.dat` and `lmgr11.dll` to the `cw` folder as well;
* Run `build.py` to build the mod's code or loader. Make sure to check the [notes](#Notes) section.

### Installation - Mac
I'm not covering this. Figure it out yourself.

### Installation - Linux
* Clone the GitHub repo to a folder of choice;
* CodeWarrior is a Windows application, therefore `wine` needs to be installed in order to run it;
* Kamek requires `dotnet` to be installed, so get that as well;
* Add Kamek to your `PATH` variable (sourcing may be necessary in order for the change to be detected);
* Do NOT run the CodeWarrior installer in WINE, as the installer will fail;
* Due to the above, you'll need to extract the needed files manually. To do so, first make sure the `cabextract` package is installed;
* Place the CodeWarrior installer in the same folder as `extractCW.sh`, then run the script;
* Run `build.py` to build the mod's code or loader.

### Notes
* The scripts automatically place the compiled binaries into the `assets/game` folder. Change that if you don't like it;
* The `game` build script requires a list of regions to compile the code to. Pass ALL to compile the code for every region. The list is as follows:
    - P = EUR;
    - E = USA;
    - J = JAP;
    - K = KOR;

## Codebase
- The codebase uses C++ and PowerPC Assembly. C++ is preferred for full function replacements and for any kind of complex logic;
- No assumption about the use of registers by C++ code is made other than the ABI. If necessary, assembly wrappers are employed to restore/backup registers;
- The headers attempt to keep a consistent naming scheme and respect the original game's names, definitions and structures. However, all class members are marked as public for ease of use;
- As code is progressively added/updated, bugs may accidentally be introduced due to incompetence.
