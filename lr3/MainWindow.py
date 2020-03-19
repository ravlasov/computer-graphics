import sys

from PySide2 import QtCore
from PySide2.QtWidgets import (QLabel, QApplication, QVBoxLayout, QHBoxLayout, QDialog, QSpacerItem,
                               QSizePolicy, QSpinBox, QSlider, QFrame)
from OpenGLView import OpenGLView


class MainWindow(QDialog):
    def onSpinPointsChanged(self):
        self.OpenGLWidget.setIterations(self.iterationsSpinBox.value())
        self.iterationsSlider.setValue(self.iterationsSpinBox.value())
    # onSpinPointsChanged

    def onSliderPointsChanged(self):
        self.OpenGLWidget.setIterations(self.iterationsSpinBox.value())
        self.iterationsSpinBox.setValue(self.iterationsSlider.value())
    # onSliderPointsChanged

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setFixedSize(1200, 920)
        self.setWindowTitle("Vlasov Roman lr3")
        # OpenGL widget
        glWidgetW = 800
        glWidgetH = 900
        self.OpenGLWidget = OpenGLView()
        self.OpenGLWidget.setFixedSize(glWidgetW, glWidgetH)
        self.OpenGLWidget.setIterations(0)
        # number of iterations
        minIter = 0
        maxIter = 6
        self.iterationsLabel = QLabel("Number of iterations: ")
        self.iterationsSpinBox = QSpinBox()
        self.iterationsSpinBox.setMinimum(minIter)
        self.iterationsSpinBox.setMaximum(maxIter)
        self.iterationsSpinBox.valueChanged.connect(self.onSpinPointsChanged)
        self.iterationsSlider = QSlider(QtCore.Qt.Horizontal)
        self.iterationsSlider.setMinimum(minIter)
        self.iterationsSlider.setMaximum(maxIter)
        self.iterationsSlider.valueChanged.connect(self.onSliderPointsChanged)
        self.iterationsMinLabel = QLabel("Min: %d" % minIter)
        self.iterationsMinLabel.setFixedWidth(40)
        self.iterationsMaxLabel = QLabel("Max: %d" % maxIter)
        self.iterationsMaxLabel.setFixedWidth(60)
        line1 = QFrame()
        line1.setFrameShape(QFrame.HLine)
        layoutAmount = QHBoxLayout()
        layoutAmount.addWidget(self.iterationsMinLabel)
        layoutAmount.addWidget(self.iterationsSpinBox)
        layoutAmount.addWidget(self.iterationsMaxLabel)
        # tools panel layout
        layoutTools = QVBoxLayout()
        layoutTools.addWidget(self.iterationsLabel)
        layoutTools.addLayout(layoutAmount)
        layoutTools.addWidget(self.iterationsSlider)
        verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)
        layoutTools.addItem(verticalSpacer)
        # window layout
        layout = QHBoxLayout()
        layout.addWidget(self.OpenGLWidget)
        layout.addLayout(layoutTools)
        self.setLayout(layout)
    # __init__


if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()
    sys.exit(app.exec_())
