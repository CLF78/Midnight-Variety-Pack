#!/usr/bin/env python3

# main.py
# This is the main executable for MKM's Cup Builder.

# Python version check
# Since we use the Walrus Operator, force Python 3.8 or higher
import sys
if sys.version_info < (3, 8):
    raise Exception('Please update your copy of Python to 3.8 or greater. Currently running on: ' + sys.version.split()[0])

import json

# If any other error occurs, let QtPy throw its own exceptions without intervention
try:
    from qtpy import QtWidgets
    from qtpy.QtCore import Qt
except ImportError:
    raise Exception('QtPy is not installed in this Python environment. Go online and download it.')

# Local imports
from common import Tracklist, Cup, RandomTrack, Track
from cups import CupListHolder
from randtracks import RandomTrackList
from tracks import TrackList

class MainWidget(QtWidgets.QWidget):
    """
    The main widget, containing the actual program interface.
    """
    def __init__(self, parent):
        super().__init__(parent)

        lyt = QtWidgets.QHBoxLayout(self)
        splitter = QtWidgets.QSplitter()
        lyt.addWidget(splitter)

        self.cups = CupListHolder(self)
        self.randomTracks = RandomTrackList(self)
        self.tracks = TrackList(self)

        self.lastBrstmDir = ''
        self.lastPngDir = ''

        splitter.addWidget(self.cups)
        splitter.addWidget(self.randomTracks)
        splitter.addWidget(self.tracks)


class MainWindow(QtWidgets.QMainWindow):
    """
    The main window. Exciting stuff.
    """
    def __init__(self):
        super().__init__()

        # Create the menubar
        self.createMenubar()

        # Set the main widget
        self.setCentralWidget(MainWidget(self))

        # Set window title
        self.setWindowTitle('Mario Kart Midnight Cup Builder')

        # Show the window
        self.show()

    def createMenubar(self):
        """
        Sets up the menubar, unsurprisingly.
        """
        bar = self.menuBar()

        # File Menu
        file = bar.addMenu('File')
        closeicon = self.style().standardIcon(QtWidgets.QStyle.StandardPixmap.SP_DialogCancelButton)
        file.addAction('New Cup Definition', self.clearData, 'CTRL+N')
        file.addAction('Open Cup Definition', self.openData, 'CTRL+O')
        file.addAction('Save Cup Definition', self.saveData, 'CTRL+S')
        file.addAction(closeicon, 'Exit', self.close, 'CTRL+Q')

    def clearData(self):
        mw = self.centralWidget()

        for cupList in mw.cups.getCupLists():
            cupList.clear()

        mw.randomTracks.list.clear()
        mw.tracks.list.clear()

    def saveData(self):

        # Get file to save to
        if file := QtWidgets.QFileDialog.getSaveFileName(self, 'Save Cup File To', 'cups.json',
                                                     'Cup File Data (*.json);;')[0]:

            # Initialize vars
            mw = self.centralWidget()
            data = {}

            # Encode the data
            for trackListName, cupList in zip(Tracklist.getAll(), mw.cups.getCupLists()):
                data[trackListName] = []
                for i in range(cupList.topLevelItemCount()):
                    item = cupList.topLevelItem(i)
                    itemData = item.data(0, 0x100)
                    itemData.tracks = [item.child(j).data(0, 0x100) for j in range(item.childCount())]
                    data[trackListName].append(itemData.asDict(file))

            # Write the file
            with open(file, 'w', encoding='utf8') as f:
                json.dump(data, f, ensure_ascii=False, indent=4)

    def openData(self):

        # Get file to open
        if file := QtWidgets.QFileDialog.getOpenFileName(self, 'Open Cup File', '',
                                                     'Cup File Data (*.json);;')[0]:

            # Clear data first
            self.clearData()

            # Open the file
            with open(file, encoding='utf-8') as f:
                data = json.load(f)

            # Keep track of loaded tracks to make sure no duplicates occur
            tracks = []
            randTracks = []

            # Load main window lists
            mw = self.centralWidget()
            trackWidget = mw.tracks
            randTrackWidget = mw.randomTracks

            # Parse the dict
            for key, cupWidget in zip(Tracklist.getAll(), mw.cups.getCupLists()):

                # Get cup list with failsafe
                cupList = data.get(key, [])

                # Initialize cup list
                for cup in cupList:

                    # Convert the cup list from dict
                    newCup = Cup.fromDict(cup, file)

                    # Add the cup entry to the tree
                    newitem = cupWidget.parent().addItem(cupWidget, newCup)

                    # Parse the tracks
                    for track in newCup.tracks:

                        # For tracks, check the existing path and add it if no identical path is found
                        if isinstance(track, Track):
                            found = False
                            for existingTrack in tracks:
                                if track.path == existingTrack.path:
                                    found = True
                                    track = existingTrack
                                    break

                            if not found:
                                tracks.append(track)
                                trackWidget.addTrack(track)
                        
                        # For random tracks, perform track replacement and
                        # check if the variants match an existing random track
                        elif isinstance(track, RandomTrack):

                            # Perform track replacement first as it is required for the comparison
                            for i, j in enumerate(track.tracks):
                                found = False
                                for existingTrack in tracks:
                                    if j[0].path == existingTrack.path:
                                        track.tracks[i][0] = existingTrack
                                        found = True
                                        break

                                # Add any track exclusive to this variant
                                if not found:
                                    tracks.append(track.tracks[i][0])
                                    trackWidget.addTrack(track.tracks[i][0])

                            # Deduplicate the random variants
                            found = False
                            for existingTrack in randTracks:
                                if track.tracks == existingTrack.tracks:
                                    found = True
                                    track = existingTrack
                                    break

                            if not found:
                                randTracks.append(track)
                                randTrackWidget.addTrack(track)
                    
                        # Add the item to the tree
                        cupWidget.parent().addItem(newitem, track)


def main():

    # Start the application
    app = QtWidgets.QApplication([])

    # Run the app
    mw = MainWindow()
    ret = app.exec()

    # Quit the process
    sys.exit(ret)


if __name__ == '__main__':
    main()
