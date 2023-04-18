#!/usr/bin/env python3

# path_utils.py
# Various path utilities.

import os.path
import subprocess
import sys
from pathlib import Path, PosixPath, PureWindowsPath
from typing import Union

class PathConverter():
    def __init__(self, rootDir: Union[Path, PosixPath, PureWindowsPath]):
        if sys.platform == 'win32':
            self.linuxDir = self.windowsDir = rootDir

        elif isinstance(rootDir, PosixPath):
            self.linuxDir = rootDir
            self.windowsDir = PureWindowsPath(subprocess.check_output(['winepath', '-w', rootDir],
                                                                    encoding='utf-8',
                                                                    stderr=subprocess.DEVNULL).rstrip('\n'))
        else:
            self.windowsDir = rootDir
            self.linuxDir = Path(subprocess.check_output(['winepath', '-u', rootDir],
                                                        encoding='utf-8',
                                                        stderr=subprocess.DEVNULL).rstrip('\n'))

    def u2w(self, path: Path) -> PureWindowsPath:
        if sys.platform == 'win32':
            return PureWindowsPath(path)
        return PureWindowsPath(self.windowsDir, path.relative_to(self.linuxDir))

    def w2u(self, path: PureWindowsPath) -> Path:
        if sys.platform == 'win32':
            return Path(path)
        return Path(self.linuxDir, path.relative_to(self.windowsDir))


def escapeWinPath(path: PureWindowsPath) -> str:
    return str(path).replace('\\', '\\\\')

def changePathRoot(file: Path, oldRoot: Path, newRoot: Path) -> Path:
    return Path(newRoot, file.relative_to(oldRoot))

def changePathEnding(file: Path, ending: str) -> Path:
    return file.with_stem(f'{file.stem}{ending}')

def properRelPath(dest: Path, src: Path) -> Path:
    return Path(os.path.relpath(dest, src))
