#!/usr/bin/env python3

# path_utils.py
# Various path utilities.

import ntpath, posixpath
import os.path
import subprocess
import sys

class PathConverter():
    def __init__(self, rootDir: str, isWinPath: bool):
        if sys.platform == 'win32':
            self.linuxDir = self.windowsDir = rootDir

        elif not isWinPath:
            self.linuxDir = rootDir
            self.windowsDir = subprocess.check_output(['winepath', '-w', rootDir],
                                                            encoding='utf-8',
                                                            stderr=subprocess.DEVNULL).rstrip('\n')
        else:
            self.windowsDir = rootDir
            self.linuxDir = subprocess.check_output(['winepath', '-u', rootDir],
                                                    encoding='utf-8',
                                                    stderr=subprocess.DEVNULL).rstrip('\n')

    def u2w(self, path: str):
        if sys.platform == 'win32':
            return path.replace('\\', '\\\\')

        relpath = posixpath.relpath(path, self.linuxDir)
        return ntpath.join(self.windowsDir, relpath).replace('/', '\\').replace('\\', '\\\\')

    def w2u(self, path: str):
        if sys.platform == 'win32':
            return path.replace('\\\\', '\\')

        relpath = ntpath.relpath(path, self.windowsDir)
        return posixpath.join(self.linuxDir, relpath).replace('\\\\', '\\').replace('\\', '/')


def changePathRoot(file: str, oldRoot: str, newRoot: str) -> str:
    return os.path.join(newRoot, os.path.relpath(file, oldRoot))

def changeFileExtension(file: str, extension: str, suffix: str = '') -> str:
    return f'{os.path.splitext(file)[0]}{suffix}{extension}'

def getAllFilesWithExt(dir: str, ext: str) -> list[str]:
    ret = []
    for root, _, files in os.walk(dir):
        for file in files:
            if file.endswith(ext):
                ret.append(os.path.join(root, file))
    return ret
