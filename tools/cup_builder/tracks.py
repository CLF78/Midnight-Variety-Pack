#!/usr/bin/env python3

# tracks.py
# Track list and track editor widgets.

import os

from qtpy import QtWidgets, QtGui
from qtpy.QtCore import Qt

from common import Track, Slot, Language, hashFile
from widgets import getMainWindow

class TrackEditor(QtWidgets.QDialog):
    def __init__(self, parent, track: Track, lastdir: str):
        super().__init__(parent)

        # Set data
        self.data = track
        self.lastdir = lastdir

        # Disable the "?" button
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)
        self.setWindowTitle(f'Track Editor - {track.names[0]}')

        self.trackPickPath = QtWidgets.QLineEdit(track.path, self)
        self.trackPickPath.setReadOnly(True)
        self.trackPickPath.setPlaceholderText('(none selected)')

        self.trackPickButton = QtWidgets.QPushButton('...', self)
        self.trackPickButton.clicked.connect(self.pickTrackFile)

        trackPickFormLyt = QtWidgets.QHBoxLayout()
        trackPickFormLyt.addWidget(self.trackPickPath)
        trackPickFormLyt.addWidget(self.trackPickButton)

        self.trackSlotCombo = QtWidgets.QComboBox(self)
        self.trackSlotCombo.addItems(Slot.getAll())
        self.trackSlotCombo.setCurrentIndex(track.specialSlot.idx())
        self.trackSlotCombo.currentIndexChanged.connect(self.updateTrackSlot)

        self.musicSlotCombo = QtWidgets.QComboBox(self)
        self.musicSlotCombo.addItems(Slot.getAll())
        self.musicSlotCombo.setCurrentIndex(track.musicSlot.idx())
        self.musicSlotCombo.currentIndexChanged.connect(self.updateMusicSlot)

        lyt = QtWidgets.QFormLayout(self)
        lyt.addRow('Track File:', trackPickFormLyt)
        lyt.addRow('Track Slot:', self.trackSlotCombo)
        lyt.addRow('Music Slot:', self.musicSlotCombo)

        self.trackNames = []
        for i, j in enumerate(Language):
            row = QtWidgets.QLineEdit(track.names[i])
            row.editingFinished.connect(self.updateTrackNames)
            self.trackNames.append(row)
            lyt.addRow(f'Track Name - {j.value}:', row)

        self.trackAuthorEdit = QtWidgets.QLineEdit(track.trackAuthor, self)
        self.trackAuthorEdit.editingFinished.connect(self.updateAuthors)
        lyt.addRow('Track Author(s):', self.trackAuthorEdit)

        self.musicPickPath = QtWidgets.QLineEdit(track.musicFile, self)
        self.musicPickPath.setReadOnly(True)
        self.musicPickPath.setPlaceholderText('(none selected)')
        self.musicPickPathFast = QtWidgets.QLineEdit(track.musicFileFast, self)
        self.musicPickPathFast.setReadOnly(True)
        self.musicPickPathFast.setPlaceholderText('(none selected)')

        self.musicPickButton = QtWidgets.QPushButton('...', self)
        self.musicPickButton.clicked.connect(lambda: self.pickMusicFile(False))
        self.musicPickButtonFast = QtWidgets.QPushButton('...', self)
        self.musicPickButtonFast.clicked.connect(lambda: self.pickMusicFile(True))

        self.musicRemoveButton = QtWidgets.QPushButton('Remove', self)
        self.musicRemoveButton.clicked.connect(self.removeFastMusic)

        musicPickFormLyt = QtWidgets.QHBoxLayout()
        musicPickFormLyt.addWidget(self.musicPickPath)
        musicPickFormLyt.addWidget(self.musicPickButton)
        musicPickFormLytFast = QtWidgets.QHBoxLayout()
        musicPickFormLytFast.addWidget(self.musicPickPathFast)
        musicPickFormLytFast.addWidget(self.musicPickButtonFast)
        musicPickFormLytFast.addWidget(self.musicRemoveButton)

        self.musicAuthorEdit = QtWidgets.QLineEdit(track.musicAuthor, self)
        self.musicAuthorEdit.editingFinished.connect(self.updateAuthors)
        self.musicAuthorEditFast = QtWidgets.QLineEdit(track.musicAuthorFast, self)
        self.musicAuthorEditFast.editingFinished.connect(self.updateAuthors)
        self.musicNameEdit = QtWidgets.QLineEdit(track.musicName, self)
        self.musicNameEdit.editingFinished.connect(self.updateAuthors)
        self.musicNameEditFast = QtWidgets.QLineEdit(track.musicNameFast, self)
        self.musicNameEditFast.editingFinished.connect(self.updateAuthors)

        lyt.addRow('Music File:', musicPickFormLyt)
        lyt.addRow('Music Name:', self.musicNameEdit)
        lyt.addRow('Music Author(s):', self.musicAuthorEdit)

        lyt.addRow('Fast Music File (Optional):', musicPickFormLytFast)
        lyt.addRow('Fast Music Name (Optional):', self.musicNameEditFast)
        lyt.addRow('Fast Music Author(s) (Optional):', self.musicAuthorEditFast)

    def updateTrackSlot(self, index: int):
        self.data.specialSlot = Slot.fromIdx(index)

    def updateMusicSlot(self, index: int):
        self.data.musicSlot = Slot.fromIdx(index)

    def updateTrackNames(self):
        for i, src in enumerate(self.trackNames):
            if text := src.text():
                self.data.names[i] = text
            else:
                src.setText(self.data.names[i])
        self.setWindowTitle(f'Track Editor - {self.trackNames[0].text()}')

    def updateAuthors(self):
        if text := self.trackAuthorEdit.text():
            self.data.trackAuthor = text
        else:
            self.trackAuthorEdit.setText(self.data.trackAuthor)

        if text := self.musicAuthorEdit.text():
            self.data.musicAuthor = text
        else:
            self.musicAuthorEdit.setText(self.data.musicAuthor)

        if text := self.musicAuthorEditFast.text():
            self.data.musicAuthorFast = text
        else:
            self.musicAuthorEditFast.setText(self.data.musicAuthorFast)

        if text := self.musicNameEdit.text():
            self.data.musicName = text
        else:
            self.musicNameEdit.setText(self.data.musicName)

        if text := self.musicNameEditFast.text():
            self.data.musicNameFast = text
        else:
            self.musicNameEditFast.setText(self.data.musicNameFast)

    def pickTrackFile(self):
        if file := QtWidgets.QFileDialog.getOpenFileName(self,
                                            'Choose a Track File',
                                            os.path.dirname(self.data.path),
                                            'YAZ0 Compressed File (*.szs)')[0]:
            self.data.path = file
            self.data.trackHash = hashFile(file)
            self.trackPickPath.setText(file)

    def pickMusicFile(self, isFast: bool):
        if file := QtWidgets.QFileDialog.getOpenFileName(self,
                                            'Choose a Music File',
                                            self.lastdir,
                                            'Binary Revolution STreaM (*.brstm)')[0]:
            self.lastdir = os.path.dirname(file)

            if isFast:
                self.data.musicFileFast = file
                self.data.fastMusicHash = hashFile(file)
                self.musicPickPathFast.setText(file)
                if not self.musicNameEditFast.text():
                    self.musicNameEditFast.setText(os.path.basename(os.path.splitext(file)[0]))
            else:
                self.data.musicFile = file
                self.data.musicHash = hashFile(file)
                self.musicPickPath.setText(file)
                if not self.musicNameEdit.text():
                    self.musicNameEdit.setText(os.path.basename(os.path.splitext(file)[0]))

    def removeFastMusic(self):
        self.data.musicFileFast = ''
        self.data.fastMusicHash = ''
        self.data.musicNameFast = ''
        self.data.musicAuthorFast = ''
        self.musicPickPathFast.setText('')
        self.musicNameEditFast.setText('')
        self.musicAuthorEditFast.setText('')

    def closeEvent(self, e: QtGui.QCloseEvent):

        mw = getMainWindow(self)
        mw.lastBrstmDir = self.lastdir

        trackList = mw.tracks.list
        cupTabs = mw.cups.getCupLists()

        # Make sure all texts are saved
        self.updateTrackNames()
        self.updateAuthors()

        # Update the entry in the track list
        for i in range(trackList.count()):
            item = trackList.item(i)
            if item.data(0x100) == self.data:
                item.setText(self.data.names[0])
                break

        # Update the entries in every cup list
        for cupList in cupTabs:
            for i in range(cupList.topLevelItemCount()):
                cup = cupList.topLevelItem(i)
                for j in range(cup.childCount()):
                    child = cup.child(j)
                    if child.data(0, 0x100) == self.data:
                        child.setText(0, self.data.names[0])

        # Close the window
        super().closeEvent(e)


class TrackList(QtWidgets.QWidget):
    def __init__(self, parent):
        super().__init__(parent)

        # Set the last chosen path to an empty directory
        self.lastdir = ''
        self.label = QtWidgets.QLabel('Track Files:', self)

        # Initialize track list
        self.list = QtWidgets.QListWidget(self)
        self.list.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
        self.list.setDragEnabled(True)
        self.list.itemSelectionChanged.connect(self.updateButtons)
        self.list.itemDoubleClicked.connect(self.editItem)

        # Import button
        self.importTracksBtn = QtWidgets.QPushButton('Import Tracks From Directory', self)
        self.importTracksBtn.clicked.connect(self.importTracks)

        # Remove button
        self.removeTracksBtn = QtWidgets.QPushButton('Remove Selected Tracks', self)
        self.removeTracksBtn.setEnabled(False)
        self.removeTracksBtn.clicked.connect(self.removeTracks)

        # Create layout
        lyt = QtWidgets.QGridLayout(self)
        lyt.addWidget(self.label, 0, 0, 1, 2)
        lyt.addWidget(self.list, 1, 0, 1, 2)
        lyt.addWidget(self.importTracksBtn, 2, 0)
        lyt.addWidget(self.removeTracksBtn, 2, 1)

    def addTrack(self, data: Track):
        newitem = QtWidgets.QListWidgetItem(data.names[0], self.list)
        newitem.setData(0x100, data)
        self.list.sortItems()

    def importTracks(self):
    
        # Obtain import directory
        if directory := QtWidgets.QFileDialog.getExistingDirectory(self, 'Import Tracks', self.lastdir):
    
            # Set the new directory as the previous
            self.lastdir = directory
    
            # Find all SZS files recursively
            szsFiles = set()
            for root, _, files in os.walk(directory):
                for file in files:
                    if file.endswith(".szs"):
                        szsFiles.add(os.path.join(root, file))

            # Add them
            for file in szsFiles:

                # Check that the file wasn't already added to the list
                data = None
                trackHash = hashFile(file)
                for i in range(self.list.count()):
                    if self.list.item(i).data(0x100).trackHash == trackHash:
                        data = self.list.item(i).data(0x100)
                        break

                # If so, add it
                if not data:
                    data = Track(file)
                    data.trackHash = trackHash
                    self.addTrack(data)

                # Else update the track path with the new one
                else:
                    data.path = file


    def updateButtons(self):

        # Enable the remove button if at least one track is selected
        self.removeTracksBtn.setEnabled(bool(self.list.selectedItems()))

    def removeTracks(self):
    
        # Take the selected items and reverse the list
        selected = self.list.selectedItems()
        selected.reverse()

        # Get the random track and cup lists
        mw = getMainWindow(self)
        randTracksList = mw.randomTracks.list
        cupTabs = mw.cups.getCupLists()

        # Remove each item
        for item in selected:
            self.list.takeItem(self.list.row(item))
            data = item.data(0x100)

            # Remove all variant entries that contain this track
            for i in range(randTracksList.count()):
                randTrack = randTracksList.item(i).data(0x100)
                for i in range(len(randTrack.tracks) - 1, -1, -1):
                    if randTrack.tracks[i][0] == data:
                        randTrack.tracks.pop(i)

            # Remove all entries that represent this track in the cup lists
            for cupList in cupTabs:
                for i in range(cupList.topLevelItemCount()):
                    curCup = cupList.topLevelItem(i)
                    for j in range(curCup.childCount() - 1, -1, -1):
                        if curCup.child(j).data(0, 0x100) == data:
                            curCup.takeChild(j)

    def editItem(self, item: QtWidgets.QListWidgetItem):
        TrackEditor(self, item.data(0x100), getMainWindow(self).lastBrstmDir).exec()
        self.list.sortItems()
