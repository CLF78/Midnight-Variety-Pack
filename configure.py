#!/usr/bin/env python3

# configure.py
# Generates the Ninja build file.

###########
# Imports #
###########

import io
import shutil
import sys
from pathlib import Path
from typing import Any

from tools.cw_wrapper.path_utils import PathConverter, escapeWinPath, changePathRoot, changePathEnding

#####################
# Dependency Checks #
#####################

if sys.version_info < (3, 8):
    raise SystemExit('Please update your copy of Python to 3.8 or greater. Currently running on: ' + sys.version.split()[0])

if not shutil.which('wszst'):
    raise SystemExit("Wiimm's SZS Tools are not installed! Make sure they're installed and present on PATH!")

try:
    import json5
except Exception:
    raise SystemExit('JSON5 not found! Please install it with `python -m pip install json5`')

############################
# Helper Functions/Classes #
############################

def escapeNinjaPath(path: str) -> str:
    return path.replace(':', '$:')

class NinjaWriter():
    def __init__(self):
        self.buffer = io.StringIO()

    def writeNewline(self, count: int = 1):
        self.buffer.write('\n' * count)

    def writeVariable(self, name: str, value: Any):
        if isinstance(value, list):
            self.buffer.write(f'{name} =')
            for val in value:
                self.buffer.write(f' $\n  {val}')
            self.writeNewline()
        else:
            self.buffer.write(f'{name} = {value}\n')

    def writeRule(self, ruleName: str, **kwargs):
        self.buffer.write(f'rule {ruleName}\n')
        for key, value in kwargs.items():
            self.buffer.write(f'  {key} = {value}\n')
        self.writeNewline()

    def writeBuildCommand(self, command: str, output: Any, input: Any, **kwargs):
        if isinstance(output, list):
            output = ' '.join(map(str, output))
        if isinstance(input, list):
            input = ' '.join(map(str, input))
        self.buffer.write(f'build {escapeNinjaPath(str(output))}: {command} {escapeNinjaPath(str(input))}\n')
        for key, value in kwargs.items():
            self.buffer.write(f'  {key} = {value}\n')
        self.writeNewline()

    @staticmethod
    def addPath(files: list[Path], newFile: Path):
        if newFile.is_file():
            files.append(newFile)


class AssetManager():
    def __init__(self, writer: NinjaWriter):
        self.outputs = {}
        self.writer = writer

    def addFile(self, src: Path, dest: Path):
        if src not in self.outputs:
            self.outputs[src] = dest

            if src.suffix == '.json5':
                self.writer.writeBuildCommand('wuj5',
                                            dest,
                                            src,
                                            in_short=src.name)
            else:
                self.writer.writeBuildCommand('copy_file',
                                            dest,
                                            src,
                                            in_short=src.name)
        else:
            src = self.outputs[src]
            self.writer.writeBuildCommand('copy_file',
                                        dest,
                                        src,
                                        in_short=src.name)

    def addDir(self, src: Path, dest: Path):
        self.writer.writeBuildCommand('copy_dir',
                                    dest,
                                    src,
                                    in_short=src.name)

#################
# Initial Setup #
#################

ROOT_DIR = Path(__file__).parent
NINJA_FILE = Path(ROOT_DIR, 'build.ninja')
converter = PathConverter(ROOT_DIR)
writer = NinjaWriter()
uiAssetManager = AssetManager(writer)

###############
# Directories #
###############

# Source directories
ASSETS_DIR = Path(ROOT_DIR, 'assets')
BMG_SRC_DIR = Path(ASSETS_DIR, 'message')
CODE_DIR = Path(ROOT_DIR, 'game')
INCLUDE_DIR = Path(ROOT_DIR, 'include')
LOADER_DIR = Path(ROOT_DIR, 'loader')
TOOL_DIR = Path(ROOT_DIR, 'tools')

# Intermediate output directories
AUTO_GEN_CODE_DIR = Path(CODE_DIR, 'cupsystem')
BUILD_DIR = Path(ROOT_DIR, 'build')
BMG_OUT_DIR = Path(BUILD_DIR, 'messages')
BMG_MERGED_OUT_DIR = Path(BUILD_DIR, 'messages', 'merged')
CODE_OUT_DIR = Path(BUILD_DIR, 'code')
CUP_ICONS_OUT_DIR = Path(BUILD_DIR, 'cupicons')
LOADER_OUT_DIR = Path(BUILD_DIR, 'loader')
UI_ASSETS_DIR = Path(BUILD_DIR, 'ui')

# Final output directories
OUT_DIR = Path(ROOT_DIR, 'out', 'mkm')
KAMEK_OUT_DIR = Path(OUT_DIR, 'Code')
MUSIC_OUT_DIR = Path(OUT_DIR, 'Music')
TRACKS_OUT_DIR = Path(OUT_DIR, 'Tracks')
UI_ASSETS_PACKED_DIR = Path(OUT_DIR, 'UI')

###################
# Other Constants #
###################

CFLAGS = [
    '-I-',
    f'-i {escapeWinPath(converter.u2w(CODE_DIR))}',
    f'-i {escapeWinPath(converter.u2w(INCLUDE_DIR))}',
    f'-i {escapeWinPath(converter.u2w(LOADER_DIR))}',
    '-Cpp_exceptions off',
    '-enum int',
    '-fp fmadd',
    '-gccext on',
    '-maxerrors 1',
    '-msext on',
    '-nostdinc',
    '-O4',
    '-once',
    '-pragma "cpp_extensions on"',
    '-pragma "cpp1x on"',
    '-pragma "gprfloatcopy on"',
    '-pragma "no_static_dtors on"',
    '-rostr',
    '-RTTI off',
    '-schedule on',
    '-sdata 0',
    '-sdata2 0',
    '-use_lmw_stmw on']

BMG_NAMES = {
    'Common': ['Award', 'MenuMulti', 'MenuSingle', 'Race'], #TODO Globe, MenuOther
    'Menu': ['Award', 'MenuMulti', 'MenuSingle', 'Race'], #TODO Globe, MenuOther
    'Race': ['Award', 'Race']}

LOADER_HOOK_ADDR = 0x80004010
LOCALES = ['E', 'F', 'G', 'I', 'J', 'K', 'M', 'Q', 'S', 'U']
REGIONS = ['P', 'E', 'J', 'K']

#########
# Tools #
#########

BMG_MERGER = Path(TOOL_DIR, 'bmg_merge', 'merge.py')
CC = Path(file) if (file := shutil.which('mwcceppc.exe')) else Path(TOOL_DIR, 'cw', 'mwcceppc.exe')
CUP_BUILDER = Path(TOOL_DIR, 'cup_builder', 'exporter.py')
CW_WRAPPER = Path(TOOL_DIR, 'cw_wrapper', 'mwcceppc_wine_wrapper.py')
CW_WRAPPER_WIN = Path(TOOL_DIR, 'cw_wrapper', 'mwcceppc_windows_wrapper.py')
KAMEK = Path(file) if (file := shutil.which('Kamek')) else Path(TOOL_DIR, 'kamek', 'Kamek')
WUJ5 = Path(TOOL_DIR, 'wuj5', 'wuj5.py')

# Ensure that CW and Kamek are installed
if not CC.is_file():
    raise SystemExit('CodeWarrior is not installed! Make sure it is either on PATH or in `tools/cw`!')
if not KAMEK.is_file():
    raise SystemExit('Kamek is not installed! Make sure it is either on PATH or in `tools/kamek`!')

#########
# Files #
#########

AUTO_GEN_CODE_FILES = [Path(AUTO_GEN_CODE_DIR, file) for file in ['CupData.cpp', 'CupCounts.h']]
AUTO_GEN_BMG_FILES = [Path(BMG_OUT_DIR, f'CupData{locale}.bmg.json5') for locale in LOCALES]
CUP_FILE = Path(ASSETS_DIR, 'cups.json5')
LOADER_OUT_FILE = Path(OUT_DIR, 'Loader.bin')
LOADER_OUT_XML = Path(BUILD_DIR, 'Loader.xml')
PORT_FILE = Path(ROOT_DIR, 'versions-mkw.txt')
SYMBOL_FILE = Path(ROOT_DIR, 'externals-mkw.txt')

###################
# Write Variables #
###################

writer.writeVariable('builddir', BUILD_DIR)
writer.writeNewline()

writer.writeVariable('cc', f'{sys.executable} {CW_WRAPPER_WIN if sys.platform == "win32" else CW_WRAPPER} {CC}')
writer.writeVariable('cflags', CFLAGS)
writer.writeNewline()
writer.writeVariable('kamek', KAMEK)
writer.writeVariable('port_file', PORT_FILE)
writer.writeVariable('symbol_file', SYMBOL_FILE)
writer.writeNewline()

###############
# Write Rules #
###############

writer.writeRule('cup_builder',
                command=f'{sys.executable} {CUP_BUILDER} $in $bmgDir $szsDir $brstmDir $iconDir $codeDir',
                description='Run Cup Exporter')

writer.writeRule('cw',
                command='$cc $cflags -c -o $out_conv -MDfile $out.d $in_conv',
                depfile='$out.d',
                description='Compile $in_short ($region)')

writer.writeRule('kmdynamic',
                command='$kamek $in -dynamic -versions=$port_file -externals=$symbol_file -output-kamek=$out -select-version=$selectversion',
                description='Link Code ($selectversion)')

writer.writeRule('kmstatic',
                command='$kamek $in -static=$loadaddr -externals=$symbol_file -output-code=$out -output-riiv=$out_riiv',
                description='Link Loader')

writer.writeRule('bmg_merge',
                command=f'{sys.executable} {BMG_MERGER} $in -o $out',
                description='Merge $out_short Messages ($region)')

writer.writeRule('wuj5',
                command=f'{sys.executable} {WUJ5} encode $in -o $out',
                description='Encode $in_short with WUJ5')

if sys.platform == 'win32':
    writer.writeRule('copy_file',
                    command='cmd /c mklink /h $out $in',
                    description='Copy $in_short')
    writer.writeRule('copy_dir',
                    command='cmd /c mklink /d $out $in',
                    description='Copy $in_short')
else:
    writer.writeRule('copy_file',
                    command='ln -f -T $in $out',
                    description='Copy $in_short')
    writer.writeRule('copy_dir',
                    command='ln -sf -T $in $out',
                    description='Copy $in_short')

writer.writeRule('pack_files',
                command='wszst c -q -D $out/%N.szs -o --szs --pt-dir --links --compr 10 $in_dir/*.d',
                description='Pack Files with WSZST')

########################
# Write Build Commands #
########################

writer.writeBuildCommand('cup_builder',
                        AUTO_GEN_CODE_FILES + AUTO_GEN_BMG_FILES + [CUP_ICONS_OUT_DIR],
                        CUP_FILE,
                        bmgDir=BMG_OUT_DIR,
                        szsDir=TRACKS_OUT_DIR,
                        brstmDir=MUSIC_OUT_DIR,
                        iconDir=CUP_ICONS_OUT_DIR,
                        codeDir=AUTO_GEN_CODE_DIR)

# Initialize containers for code output
inputs = set(CODE_DIR.rglob('*cpp'))
inputs.add(Path(AUTO_GEN_CODE_DIR, 'CupData.cpp'))
outputsByRegion = {key: [] for key in REGIONS}
outputs = set()

# Parse each file
for file in sorted(inputs):

    # Obtain the configuration data
    configFile = file.with_suffix('.json5')
    if configFile.is_file():
        with configFile.open(encoding='utf-8') as f:
            configData = set(json5.load(f).get('regions', []))
    else:
        configData = set()

    # Parse each region
    for region in REGIONS:

        # Get the destination path
        output = changePathRoot(file, CODE_DIR, CODE_OUT_DIR)
        output = changePathEnding(output, region if region in configData else '')
        output = output.with_suffix('.o')

        # Add it to the outputs for the current region
        outputsByRegion[region].append(output)

        # Write the corresponding build command and make sure it's not duplicated
        if output not in outputs:
            writer.writeBuildCommand('cw',
                                    output,
                                    file,
                                    out_conv=escapeWinPath(converter.u2w(output)),
                                    in_conv=escapeWinPath(converter.u2w(file)),
                                    in_short=file.relative_to(CODE_DIR),
                                    region=region if region in configData else 'All')
            outputs.add(output)

# Initialize containers for loader code output
loaderInputs = LOADER_DIR.rglob('*.cpp')
loaderOutputs = []

# Parse each file
for file in sorted(loaderInputs):

    # Get the destination path
    output = changePathRoot(file, LOADER_DIR, LOADER_OUT_DIR).with_suffix('.o')
    loaderOutputs.append(output)

    # Write the corresponding build command
    writer.writeBuildCommand('cw',
                            output,
                            file,
                            out_conv=escapeWinPath(converter.u2w(output)),
                            in_conv=escapeWinPath(converter.u2w(file)),
                            in_short=file.relative_to(LOADER_DIR),
                            region='All')

# Write the Kamek linking rules
# Code commands
for region, outputs in outputsByRegion.items():
    writer.writeBuildCommand('kmdynamic',
                            Path(KAMEK_OUT_DIR, f'code{region}.bin'),
                            sorted(outputs) + ['|', '$port_file', '$symbol_file'],
                            selectversion=region)

# Loader command
writer.writeBuildCommand('kmstatic',
                        LOADER_OUT_FILE,
                        sorted(loaderOutputs) + ['|', '$symbol_file'],
                        out_riiv=LOADER_OUT_XML,
                        loadaddr=hex(LOADER_HOOK_ADDR))

# Initialize UI asset list
uiAssets = {
	'AwardMKM': {
        Path(CUP_ICONS_OUT_DIR): Path('cups')
	},

	'MenuSingleMKM': {
		Path(ASSETS_DIR, 'cuparrows', 'CupSelectCupArrowLeft.brctr.json5'): Path('button', 'ctrl', 'CupSelectCupArrowLeft.brctr'),
		Path(ASSETS_DIR, 'cuparrows', 'CupSelectCupArrowRight.brctr.json5'): Path('button', 'ctrl', 'CupSelectCupArrowRight.brctr'),
        Path(CUP_ICONS_OUT_DIR): Path('cups')
	},

	'MenuMultiMKM': {
		Path(ASSETS_DIR, 'cuparrows', 'CupSelectCupArrowLeft.brctr.json5'): Path('button', 'ctrl', 'CupSelectCupArrowLeft.brctr'),
		Path(ASSETS_DIR, 'cuparrows', 'CupSelectCupArrowRight.brctr.json5'): Path('button', 'ctrl', 'CupSelectCupArrowRight.brctr'),
        Path(CUP_ICONS_OUT_DIR): Path('cups')
	},

	'RaceMKM': {
        Path(CUP_ICONS_OUT_DIR): Path('cups')
	}
}

# Merge and add localized messages to the UI asset list
for locale, bmgFile in zip(LOCALES, AUTO_GEN_BMG_FILES):
    for file, dests in BMG_NAMES.items():
        baseFile = Path(BMG_SRC_DIR, file)
        destFile = Path(BMG_MERGED_OUT_DIR, locale, f'{file}.bmg.json5')

        inputs = []
        NinjaWriter.addPath(inputs, changePathEnding(baseFile, f'_{locale}.bmg.json5'))
        NinjaWriter.addPath(inputs, changePathEnding(baseFile, 'Wiimmfi.bmg.json5'))
        NinjaWriter.addPath(inputs, changePathEnding(baseFile, f'Wiimmfi_{locale}.bmg.json5'))
        NinjaWriter.addPath(inputs, changePathEnding(baseFile, 'MKM.bmg.json5'))
        NinjaWriter.addPath(inputs, changePathEnding(baseFile, f'MKM_{locale}.bmg.json5'))
        if file == 'Common':
            inputs.append(bmgFile)

        if len(inputs) > 1:
            writer.writeBuildCommand('bmg_merge',
                                    destFile,
                                    inputs,
                                    out_short=destFile.stem,
                                    region=locale)

            newDict = {destFile: Path('message', f'{file}.bmg')}
            for dest in dests:
                destKey = f'{dest}MKM_{locale}'
                if destKey not in uiAssets:
                    uiAssets[destKey] = {}
                uiAssets[destKey] |= newDict

# Write the UI asset commands
for file, subFiles in uiAssets.items():
    for src, dest in subFiles.items():
        dest = Path(UI_ASSETS_DIR, f'{file}.d', dest)
        if not src.suffix:
            uiAssetManager.addDir(src, dest)
        else:
            uiAssetManager.addFile(src, dest)

# Pack the UI assets
writer.writeBuildCommand('pack_files',
                        UI_ASSETS_PACKED_DIR,
                        ' '.join(map(str, uiAssetManager.outputs.values())),
                        in_dir=UI_ASSETS_DIR)

# Write the file out
with NINJA_FILE.open('w', encoding='utf-8') as f:
    f.write(writer.buffer.getvalue())
