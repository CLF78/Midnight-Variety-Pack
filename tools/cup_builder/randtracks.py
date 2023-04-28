#!/usr/bin/env python3

# randtracks.py
# Random track list and random track editor widgets.

from typing import Union

from qtpy import QtWidgets, QtGui
from qtpy.QtCore import Qt

from common import Track, RandomTrack, Language
from widgets import getMainWindow

class RandomTrackEditor(QtWidgets.QDialog):
    def __init__(self, parent, randomTrack: RandomTrack):
        super().__init__(parent)

        # Set data
        self.data = randomTrack

        # Disable the "?" button
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)
        self.setWindowTitle(f'Random Track Editor - {randomTrack.names[0]}')

        lyt = QtWidgets.QFormLayout(self)

        self.trackNames = []
        for i, j in enumerate(Language):
            row = QtWidgets.QLineEdit(randomTrack.names[i])
            row.editingFinished.connect(self.updateTrackNames)
            self.trackNames.append(row)
            lyt.addRow(f'Random Track Name - {j.value}:', row)

        self.addTrackButton = QtWidgets.QPushButton('Add Variant', self)
        self.addTrackButton.setEnabled(getMainWindow(self).tracks.list.count() != 0)
        self.addTrackButton.clicked.connect(lambda: self.addTrack(None, -1))
        lyt.addRow(self.addTrackButton)

        self.trackRows = []
        for i in randomTrack.tracks:
            self.addTrack(i[0], i[1])

    def updateTrackNames(self):
        for i, src in enumerate(self.trackNames):
            if text := src.text():
                self.data.names[i] = text
            else:
                src.setText(self.data.names[i])
        self.setWindowTitle(f'Random Track Editor - {self.trackNames[0].text()}')

    def addTrack(self, track: Union[Track, None], chance: int):
        tracklist = getMainWindow(self).tracks.list
        allTracks = [tracklist.item(i).data(0x100) for i in range(tracklist.count())]
        allTrackNames = [track.names[0] for track in allTracks]
        row = QtWidgets.QHBoxLayout()

        trackSelector = QtWidgets.QComboBox(self)
        trackSelector.addItems(allTrackNames)
        trackSelector.currentIndexChanged.connect(self.updateTracks)

        chanceLabel = QtWidgets.QLabel('Chance:', self)
        chancePicker = QtWidgets.QSpinBox(self)
        chancePicker.setRange(1, 256)
        chancePicker.valueChanged.connect(self.updateTracks)

        removeBtn = QtWidgets.QPushButton('Remove', self)
        removeBtn.clicked.connect(lambda: self.removeTrack(row))

        if track is not None:
            trackSelector.setCurrentIndex(allTracks.index(track))
            chancePicker.setValue(chance)
        else:
            self.data.tracks.append((allTracks[0], 1))

        row.addWidget(trackSelector)
        row.addWidget(chanceLabel)
        row.addWidget(chancePicker)
        row.addWidget(removeBtn)

        lyt = self.layout()
        rowCount = lyt.rowCount()
        lyt.insertRow(rowCount - 1, f'Variant {rowCount - len(Language)}:', row)
        self.trackRows.append(row)

    def removeTrack(self, trackRow):
        for i, row in enumerate(self.trackRows):
            if row == trackRow:
                self.layout().removeRow(i + len(Language))
                self.trackRows.pop(i)
                self.data.tracks.pop(i)
                break

        for i in range(len(self.trackRows)):
            rowLabel = self.layout().itemAt(i + len(Language), QtWidgets.QFormLayout.LabelRole).widget()
            rowLabel.setText(f'Variant {i + 1}:')

    def updateTracks(self):
        tracklist = getMainWindow(self).tracks.list
        allTracks = [tracklist.item(i).data(0x100) for i in range(tracklist.count())]

        for i, j in enumerate(self.trackRows):
            track = j.itemAt(0).widget().currentIndex()
            chance = j.itemAt(2).widget().value()
            self.data.tracks[i] = (allTracks[track], chance)

    def closeEvent(self, e: QtGui.QCloseEvent):
        mw = getMainWindow(self)
        trackList = mw.randomTracks.list

        # Update the names
        self.updateTrackNames()

        # Update the entry in the track list
        for i in range(trackList.count()):
            item = trackList.item(i)
            if item.data(0x100) == self.data:
                item.setText(self.data.names[0])
                break

        # Update the entries in every cup list
        for cupList in mw.cups.getCupLists():
            for i in range(cupList.topLevelItemCount()):
                cup = cupList.topLevelItem(i)
                for j in range(cup.childCount()):
                    child = cup.child(j)
                    if child.data(0, 0x100) == self.data:
                        child.setText(0, self.data.names[0])

        # Close the window
        super().closeEvent(e)


class RandomTrackList(QtWidgets.QWidget):
    def __init__(self, parent):
        super().__init__(parent)

        self.label = QtWidgets.QLabel('Random Tracks:', self)

        # Initialize track list
        self.list = QtWidgets.QListWidget(self)
        self.list.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
        self.list.setDragEnabled(True)
        self.list.itemSelectionChanged.connect(self.updateButtons)
        self.list.itemDoubleClicked.connect(self.editItem)

        # Import button
        self.addTracksBtn = QtWidgets.QPushButton('Add Random Track', self)
        self.addTracksBtn.clicked.connect(lambda: self.addTrack(RandomTrack()))

        # Remove button
        self.removeTracksBtn = QtWidgets.QPushButton('Remove Selected Tracks', self)
        self.removeTracksBtn.setEnabled(False)
        self.removeTracksBtn.clicked.connect(self.removeTracks)

        # Create layout
        lyt = QtWidgets.QGridLayout(self)
        lyt.addWidget(self.label, 0, 0, 1, 2)
        lyt.addWidget(self.list, 1, 0, 1, 2)
        lyt.addWidget(self.addTracksBtn, 2, 0)
        lyt.addWidget(self.removeTracksBtn, 2, 1)

    def addTrack(self, data):
        newitem = QtWidgets.QListWidgetItem(data.names[0], self.list)
        newitem.setData(0x100, data)
        self.list.sortItems()

    def updateButtons(self):

        # Enable the remove button if at least one track is selected
        self.removeTracksBtn.setEnabled(bool(self.list.selectedItems()))

    def removeTracks(self):
    
        # Take the selected items and reverse the list
        selected = self.list.selectedItems()
        selected.reverse()

        # Remove each item
        for item in selected:
            self.list.takeItem(self.list.row(item))
            data = item.data(0x100)

            # Remove all entries that represent this track in the cup lists
            for cupList in getMainWindow(self).cups.getCupLists():
                for i in range(cupList.topLevelItemCount()):
                    curCup = cupList.topLevelItem(i)
                    for j in range(curCup.childCount() - 1, -1, -1):
                        if curCup.child(j).data(0, 0x100) == data:
                            curCup.takeChild(j)

    def editItem(self, item):
        RandomTrackEditor(self, item.data(0x100)).exec()
        self.list.sortItems()
