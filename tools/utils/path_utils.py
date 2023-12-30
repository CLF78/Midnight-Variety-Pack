#!/usr/bin/env python3

# path_utils.py
# Various path utilities.

import os.path
import sys
from pathlib import Path, PureWindowsPath
from typing import Union

def unix_to_windows(path: Path) -> Union[Path, PureWindowsPath]:
    """
    Converts a UNIX path to a Windows path (without using winepath).
    """
    if sys.platform == 'win32':
        return path
    return PureWindowsPath('Z:', str(path).replace('/', '\\'))

def windows_to_unix(path: Union[Path, PureWindowsPath]) -> Path:
    """
    Converts a Windows path to a UNIX path (without using winepath).
    """
    if sys.platform == 'win32':
        return path
    return Path(str(path)[2:].replace('\\', '/'))

def escape_win_path(path: Path) -> str:
    """
    Escapes Windows paths used in command line tools executed through Wine.
    """
    return str(path).replace('\\', '\\\\')

def change_root(path: Path, oldRoot: Path, newRoot: Path) -> Path:
    """
    Changes a path to be relative to a different root, given the old root.
    """
    return Path(newRoot, path.relative_to(oldRoot))

def change_stem_suffix(path: Path, ending: str) -> Path:
    """
    Adds the given suffix to the path stem.
    """
    return path.with_stem(f'{path.stem}{ending}')

def relpath_proper(dest: Path, src: Path) -> Path:
    """
    Recreates the walk_up option added recently to Path.relative_to()
    TODO drop this and upgrade the project's minimum Python version to 3.12.
    """
    return Path(os.path.relpath(dest, src))

def add_to_set(set: list[Path], path: Path):
    """
    Adds a path to the given list if it exists.
    """
    if path.exists():
        set.append(path)
