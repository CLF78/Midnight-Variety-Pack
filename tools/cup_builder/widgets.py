#!/usr/bin/env python3

# widgets.py
# Various Qt-related code.

from common import Cup

from qtpy import QtCore, QtGui, QtWidgets
from qtpy.QtCore import Qt

def getMainWindow(self: QtCore.QObject) -> QtCore.QObject:
    """
    Gets the main window from the current widget.
    """
    parent = self.parent()
    if parent is None:
        return self.centralWidget()
    return getMainWindow(parent)


class ModdedTreeWidget(QtWidgets.QTreeWidget):
    def __init__(self, parent):
        super().__init__(parent)

        # Hide header and enable reordering
        self.setDragDropMode(QtWidgets.QAbstractItemView.InternalMove)
        self.setHeaderHidden(True)

    def dragEnterEvent(self, e: QtGui.QDragEnterEvent):
        """
        Forces the widget to accept external drops, which would otherwise be rejected due to the InternalMove flag.
        """
        src = e.source()
        if isinstance(src, QtWidgets.QAbstractItemView):
            e.accept()

    def dropEvent(self, e: QtGui.QDropEvent):
        """
        Override dropping behaviour
        """
        src = e.source()

        # Check if the source is another one of the two widgets
        if isinstance(src, QtWidgets.QListWidget):

            # Check that the items are being dropped on an item
            item = self.itemAt(e.pos())
            if item:

                # If the item has a parent, make sure it's a child of it
                if item.parent():
                    item = item.parent()

                # Add each item
                for source in src.selectedItems():
                    self.parent().addItem(item, source.data(0x100))

        # Handle internal moving
        elif src is self:
            sourceItem = src.selectedItems()[0]
            isCup = isinstance(sourceItem.data(0, 0x100), Cup)

            # Run the action
            super().dropEvent(e)

            # Move the item back if it violates the rules
            # Cups cannot be placed inside other cups
            if isCup:
                if parent := sourceItem.parent():
                    parent.takeChild(parent.indexOfChild(sourceItem))
                    idx = self.indexOfTopLevelItem(parent)
                    self.insertTopLevelItem(idx, sourceItem)

            # Tracks cannot be placed outside cups
            elif not sourceItem.parent():
                idx = self.indexOfTopLevelItem(sourceItem)
                self.takeTopLevelItem(idx)
                item = self.topLevelItem(idx)
                if not item:
                    item = self.topLevelItem(idx - 1)
                item.addChild(sourceItem)
