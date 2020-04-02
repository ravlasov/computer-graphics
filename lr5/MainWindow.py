import sys
from random import random

from OpenGLView import OpenGLView
from PySide2 import QtCore
from PySide2.QtWidgets import (QApplication, QVBoxLayout, QHBoxLayout, QDialog, QSpacerItem,
                               QSizePolicy, QLabel, QSpinBox, QSlider, QDoubleSpinBox, QFrame, QRadioButton)


class MainWindow(QDialog):

    def setupLayout(self):
        # OpenGL widget
        glWidgetW = 800
        glWidgetH = 900
        self.OpenGLWidget = OpenGLView()
        self.OpenGLWidget.setFixedSize(glWidgetW, glWidgetH)
        # tools panel layout
        layoutTools = QVBoxLayout()
        verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)
        layoutTools.addItem(verticalSpacer)
        # window layout
        layout = QHBoxLayout()
        layout.addWidget(self.OpenGLWidget)
        layout.addLayout(layoutTools)
        self.setLayout(layout)

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setFixedSize(1200, 920)
        self.setWindowTitle("lr5")
        self.setupLayout()
    # __init__


if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()
    sys.exit(app.exec_())
