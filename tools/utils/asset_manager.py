#!/usr/bin/env python3

# asset_manager.py
# Manages asset encoding command generation, preventing duplicate encoding runs.

from pathlib import Path
from typing import Union

from .ninja_syntax_ex import Writer as NinjaWriter
from .path_utils import change_root

class AssetManager():
    def __init__(self, inputFiles: dict, srcDir: Path, buildDir: Path, finalPath: Path):
        self.inputs = inputFiles
        self.srcDir = srcDir
        self.buildDir = buildDir
        self.finalPath = finalPath

    def writeCommand(self, writer: NinjaWriter, source: Path, dest: Path, isCopy: bool = False):

        # Copy directories with a dedicated command (ignore isCopy)
        # Use the timestamp file as an implicit input to ensure the file is rebuilt after changes
        if not source.suffix:
            timestampFile = Path(source, '.extracted')
            writer.build('copy_dir', dest, [], implicit_inputs=[timestampFile], in_dir=source, in_short=source.name)

        # Encode BREFF files with breffconv
        elif source.suffixes == ['.breff', '.d'] and not isCopy:
            writer.build('breffconv', dest, source.glob('*.json'), in_dir=source, out_short=dest.name)

        # Encode JSON5 files with wuj5
        elif source.suffix == '.json5' and not isCopy:
            writer.build('wuj5', dest, source, in_short=source.name)

        # Encode PNG files with wimgt, using the other suffixes as encoding settings
        elif source.suffix == '.png' and not isCopy:
            format = source.suffixes[0][1:].upper()
            encoding = source.suffixes[1][1:].upper() if len(source.suffixes) > 2 else 'RGB5A3'
            writer.build('wimgt', dest, source, in_short=source.name, encode=f'{format}.{encoding}')

        # Copy directly if it's another format or it's already encoded by an existing command
        else:
            writer.build('copy_file', dest, source, in_short=source.name)

    def getDestPath(self, source: Path, dest: Union[None, str, Path]) -> Path:

        # Get final filename+extension
        source = AssetManager.getDestFilename(source)

        # Destination is empty, simply change the root directory
        if dest is None:
            return change_root(source, self.srcDir, self.buildDir)

        # Destination is a string, change the filename only
        elif isinstance(dest, str):
            return change_root(source, self.srcDir, self.buildDir).with_stem(dest)

        # Destination is a full path, concatenate the build directory and the given path
        else:
            return Path(self.buildDir, dest)

    @staticmethod
    def getDestFilename(source: Path) -> Path:

        # For JSON5 and PNG files, drop all suffixes except the first one
        if source.suffix == '.json5' or source.suffix == '.png':
            for _ in range(len(source.suffixes) - 1):
                source = source.with_suffix('')

        # For other formats, leave them as is
        return source


class GlobalAssetManager():
    def __init__(self, writer: NinjaWriter):
        self.writer = writer
        self.managers: list[AssetManager] = []
        self.allOutputs = {}

    def addManager(self, inputFiles: dict, srcDir: Path, buildDir: Path, finalPath: Path):
        self.managers.append(AssetManager(inputFiles, srcDir, buildDir, finalPath))

    def writeCommands(self):
        for manager in self.managers:

            # Create a list of the packed files, these will be the input to the pack command
            packInputs = []

            # Parse all inputs for this manager
            for source, dest in manager.inputs.items():

                # If the asset isn't already encoded, write the cmd for it and add it to the global output dict
                if source not in self.allOutputs:
                    dest = manager.getDestPath(source, dest)
                    manager.writeCommand(self.writer, source, dest)
                    self.allOutputs[source] = dest

                # Else just copy the existing output over
                else:
                    dest = manager.getDestPath(source, dest)
                    source = self.allOutputs[source] if source.suffix else source
                    manager.writeCommand(self.writer, source, dest, True)

                # Add the output to the pack command
                packInputs.append(dest)

                # Add the .extracted file if it's a directory as well
                if not dest.suffix:
                    packInputs.append(Path(source, '.extracted'))

            # Do not write a pack command if the destination is a directory
            if manager.finalPath.suffix:
                compr = '10' if manager.finalPath.suffix == '.szs' else 'UNCOMPRESSED'
                self.writer.build('wszst', manager.finalPath, packInputs, in_dir=manager.buildDir,
                                  in_short=manager.finalPath.name, compress=compr)
