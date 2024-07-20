#!/usr/bin/env python3

# main.py
# This is the main executable for MVP's Cup Builder.

# Python version check
# Since we use the Walrus Operator, force Python 3.8 or higher
import os
import sys

if sys.version_info < (3, 8):
    raise SystemExit('Please update your copy of Python to 3.8 or greater. Currently running on: ' + sys.version.split()[0])

# Import json5
try:
    import json5
except ImportError:
    raise SystemExit('JSON5 not found! Please install it with `python -m pip install json5`')

# If any other error occurs, let QtPy throw its own exceptions without intervention
try:
    from qtpy import QtWidgets, QtGui
    from qtpy.QtCore import Qt
except ImportError:
    raise SystemExit('QtPy not found! Please install it with `python -m pip install qtpy`')

# Local imports
from common import Tracklist
from cups import CupListHolder
from parsing import importData, checkData
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
        self.currentFile = ''

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
        self.setWindowTitle('Midnight x Variety Pack Cup Builder')

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
        file.addAction('Check Validity', self.checkData, 'CTRL+K')
        file.addAction('Save Cup Definition', self.saveData, 'CTRL+S')
        file.addAction('Save Cup Definition As', self.saveDataAs, 'CTRL+SHIFT+S')
        file.addAction(closeicon, 'Exit', self.close, 'CTRL+Q')

    def clearData(self):
        mw = self.centralWidget()

        for cupList in mw.cups.getCupLists():
            cupList.clear()

        mw.randomTracks.list.clear()
        mw.tracks.list.clear()
        mw.currentFile = ''

    def saveDataAs(self):

        # Reset current file and call function
        self.centralWidget().currentFile = ''
        self.saveData()

    def saveData(self):

        # Get file to save to
        mw = self.centralWidget()
        if file := mw.currentFile if mw.currentFile else \
            QtWidgets.QFileDialog.getSaveFileName(self, 'Save Cup File To', 'data.json5',
                                                'Cup File Data (*.json5);;')[0]:

            # Initialize vars
            mw.currentFile = file
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
                json5.dump(data, f, ensure_ascii=False, indent=4)

            # Inform the user
            QtWidgets.QMessageBox.information(self, 'Save Completed!', 'Save completed successfully!')

    def openData(self):

        # Get file to open
        if file := QtWidgets.QFileDialog.getOpenFileName(self,
                                                        'Open Cup File',
                                                        os.path.dirname(self.centralWidget().currentFile),
                                                        'Cup File Data (*.json5);;')[0]:

            # Clear data first
            self.clearData()

            # Get the parsed data
            tracks, randTracks, cups = importData(file)

            # Get main window lists
            mw = self.centralWidget()
            mw.currentFile = file
            trackWidget = mw.tracks
            randTrackWidget = mw.randomTracks

            # Fill the cup lists
            for cupList, cupWidget in zip(cups, mw.cups.getCupLists()):
                for cup in cupList:
                    newitem = cupWidget.parent().addItem(cupWidget, cup)
                    for track in cup.tracks:
                        cupWidget.parent().addItem(newitem, track)

            # Fill the track lists (we need to do this separately for exclusive random track variants)
            for track in tracks:
                trackWidget.addTrack(track)
            for randTrack in randTracks:
                randTrackWidget.addTrack(randTrack)

    def checkData(self):

        # Get the main window
        mw = self.centralWidget()
        trackWidget = mw.tracks.list
        randTrackWidget = mw.randomTracks.list
        cupLists = []

        # Collect the cups
        for cupList in mw.cups.getCupLists():
            cups = []
            for i in range(cupList.topLevelItemCount()):
                cup = cupList.topLevelItem(i)
                cupData = cup.data(0, 0x100)
                cupData.tracks = [cup.child(j).data(0, 0x100) for j in range(cup.childCount())]
                cups.append(cupData)
            cupLists.append(cups)

        # Collect the tracks
        tracks = [trackWidget.item(i).data(0x100) for i in range(trackWidget.count())]
        randTracks = [randTrackWidget.item(i).data(0x100) for i in range(randTrackWidget.count())]

        # Feed the data to the check function
        errors = checkData(cupLists, randTracks, tracks)

        # Create the dialog to show the errors
        errorDialog = QtWidgets.QDialog(self)
        errorDialog.setWindowTitle('Scan Completed!')
        lyt = QtWidgets.QVBoxLayout(errorDialog)
        if errors:
            lyt.addWidget(QtWidgets.QLabel('The following errors were found:', errorDialog))
            errorDisplay = QtWidgets.QTextEdit(errorDialog)
            errorDisplay.setReadOnly(True)
            errorDisplay.setWordWrapMode(QtGui.QTextOption.NoWrap)
            errorDisplay.setPlainText('\n'.join(errors))
            lyt.addWidget(errorDisplay)
        else:
            lyt.addWidget(QtWidgets.QLabel('No errors found!', errorDialog))

        # Add a close button
        closeBtn = QtWidgets.QPushButton('OK', errorDialog)
        closeBtn.clicked.connect(errorDialog.close)
        lyt.addWidget(closeBtn, alignment=Qt.AlignCenter)

        # Execute the dialog
        errorDialog.exec()


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
