#!/usr/bin/env python3

# cups.py
# Cup list and cup editor widgets.

import os.path

from qtpy import QtWidgets, QtGui
from qtpy.QtCore import Qt

from common import Track, Tracklist, RandomTrack, Cup, Language
from randtracks import RandomTrackEditor
from tracks import TrackEditor
from widgets import getMainWindow, ModdedTreeWidget

class CupEditor(QtWidgets.QDialog):
    def __init__(self, parent, cup: Cup, lastdir: str):
        super().__init__(parent)

        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)
        self.setWindowTitle(f'Cup Editor - {cup.names[0]}')

        self.lastdir = lastdir
        self.data = cup

        self.cupPath = QtWidgets.QLineEdit(cup.iconFile, self)
        self.cupPath.setReadOnly(True)
        self.cupPath.setPlaceholderText('(none selected)')

        self.cupPathPickBtn = QtWidgets.QPushButton('...', self)
        self.cupPathPickBtn.clicked.connect(self.pickIconFile)

        cupPathLyt = QtWidgets.QHBoxLayout()
        cupPathLyt.addWidget(self.cupPath)
        cupPathLyt.addWidget(self.cupPathPickBtn)

        form = QtWidgets.QFormLayout()
        form.addRow('Cup Icon Path:', cupPathLyt)

        self.cupNames = []
        for i, j in enumerate(Language):
            row = QtWidgets.QLineEdit(cup.names[i])
            row.editingFinished.connect(self.updateCupNames)
            self.cupNames.append(row)
            form.addRow(f'Cup Name - {j.value}:', row)

        lyt = QtWidgets.QVBoxLayout(self)
        lyt.addLayout(form)

        self.cupPic = QtWidgets.QLabel(self)
        self.cupPic.setAlignment(Qt.AlignCenter)
        if os.path.isfile(cup.iconFile):
            img = QtGui.QPixmap(cup.iconFile)
            self.cupPic.setPixmap(img)
            lyt.insertWidget(0, self.cupPic)

    def pickIconFile(self):
        if file := QtWidgets.QFileDialog.getOpenFileName(self,
                                            'Choose an Icon File',
                                            self.lastdir,
                                            'Portable Network Graphics (*.png)')[0]:
            self.lastdir = os.path.dirname(file)

            img = QtGui.QPixmap(file)
            self.cupPic.setPixmap(img)
            self.layout().insertWidget(0, self.cupPic)
            self.data.iconFile = file
            self.cupPath.setText(file)

    def updateCupNames(self):
        for i, src in enumerate(self.cupNames):
            if text := src.text():
                self.data.names[i] = text
            else:
                src.setText(self.data.names[i])
        self.setWindowTitle(f'Cup Editor - {self.cupNames[0].text()}')

    def closeEvent(self, e: QtGui.QCloseEvent):
        mw = getMainWindow(self)
        mw.lastPngDir = self.lastdir
        cupList = mw.cups.tabs.currentWidget().list

        # Update cup names
        self.updateCupNames()

        # Update the entry in the cup list
        for i in range(cupList.topLevelItemCount()):
            cup = cupList.topLevelItem(i)
            if cup.data(0, 0x100) == self.data:
                cup.setText(0, self.data.names[0])
                break

        # Close the window
        super().closeEvent(e)


class CupList(QtWidgets.QWidget):
    def __init__(self, parent, trackCount):
        super().__init__(parent)

        self.trackCount = trackCount

        self.list = ModdedTreeWidget(self)
        self.list.itemDoubleClicked.connect(self.editItem)
        self.list.itemSelectionChanged.connect(self.updateButtons)

        self.addCupButton = QtWidgets.QPushButton('Add New Cup', self)
        self.addCupButton.clicked.connect(lambda: self.addItem(self.list, Cup()))

        self.reorderButton = QtWidgets.QPushButton('Reorder Tracks', self)
        self.reorderButton.setEnabled(False)
        self.reorderButton.clicked.connect(self.reorderTracks)

        self.removeButton = QtWidgets.QPushButton('Remove Selected Item', self)
        self.removeButton.setEnabled(False)
        self.removeButton.clicked.connect(self.removeItem)

        lyt = QtWidgets.QGridLayout(self)
        lyt.addWidget(self.list, 0, 0, 1, 3)
        lyt.addWidget(self.addCupButton, 1, 0)
        lyt.addWidget(self.reorderButton, 1, 1)
        lyt.addWidget(self.removeButton, 1, 2)

    def addItem(self, parent, data) -> QtWidgets.QTreeWidgetItem:
        newitem = QtWidgets.QTreeWidgetItem(parent, [data.names[0]])

        if isinstance(parent, QtWidgets.QTreeWidget):
            newitem.setFlags(newitem.flags() | Qt.ItemIsDropEnabled)
        else:
            newitem.setFlags(newitem.flags() ^ Qt.ItemIsDropEnabled)

        newitem.setData(0, 0x100, data)
        self.updateButtons()
        return newitem

    def editItem(self, item):
        data = item.data(0, 0x100)
        if isinstance(data, Cup):
            CupEditor(self, data, getMainWindow(self).lastPngDir).exec()
        elif isinstance(data, RandomTrack):
            RandomTrackEditor(self, data).exec()
        elif isinstance(data, Track):
            TrackEditor(self, data, getMainWindow(self).lastBrstmDir).exec()
        else:
            print('Failed to edit!!')

    def updateButtons(self):

        # Enable the remove button if at least one track is selected
        self.removeButton.setEnabled(bool(self.list.selectedItems()))
        self.reorderButton.setEnabled(bool(self.list.topLevelItemCount()))

    def removeItem(self):
        for item in self.list.selectedItems():
            if parent := item.parent():
                parent.takeChild(parent.indexOfChild(item))
            else:
                self.list.takeTopLevelItem(self.list.indexOfTopLevelItem(item))
        self.updateButtons()

    def reorderTracks(self):

        # Initialize track list
        tracks = []

        # Remove all tracks from the cups
        cupCount = self.list.topLevelItemCount()
        for i in range(cupCount):
            tracks += self.list.topLevelItem(i).takeChildren()

        # Sort the list
        tracks.sort(key=lambda x: x.text(0))
        trackCount = len(tracks)

        # If there are less cups than necessary, add new cups
        requiredCups = ((trackCount + (self.trackCount - 1)) // self.trackCount) - cupCount
        for _ in range(requiredCups):
            self.addItem(self.list, Cup())

        # Fill the cups and expand them
        for i, j in enumerate(tracks):
            cup = self.list.topLevelItem(i // self.trackCount)
            cup.setExpanded(True)
            cup.addChild(j)


class CupListHolder(QtWidgets.QWidget):
    def __init__(self, parent):
        super().__init__(parent)

        self.label = QtWidgets.QLabel('Cup Layouts:', self)
        self.tabs = QtWidgets.QTabWidget(self)

        for i in Tracklist:
            tab = CupList(self.tabs, Tracklist.getTrackCount(i))
            self.tabs.addTab(tab, i.value)

        lyt = QtWidgets.QVBoxLayout(self)
        lyt.addWidget(self.label)
        lyt.addWidget(self.tabs)

    def getCupLists(self) -> list[QtWidgets.QListWidget]:
        return [self.tabs.widget(i).list for i in range(self.tabs.count())]
