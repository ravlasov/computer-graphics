import sys
from random import random

from OpenGLView import OpenGLView
from PySide2 import QtCore
from PySide2.QtWidgets import (QApplication, QVBoxLayout, QHBoxLayout, QDialog, QSpacerItem,
                               QSizePolicy, QLabel, QSpinBox, QSlider, QDoubleSpinBox, QFrame)


class MainWindow(QDialog):
    points = 7
    x = []
    y = []
    z = []
    widgets = []

    def updatePoints(self):
        tmpX = []
        tmpY = []
        tmpZ = []
        for i in range(min(self.points, len(self.x))):
            tmpX.append(self.x[i])
            tmpY.append(self.y[i])
            tmpZ.append(self.z[i])
        if self.points > len(self.x):
            for i in range(self.points - len(self.x)):
                tmpX.append(random() * 2 - 1)
                tmpY.append(random() * 2 - 1)
                tmpZ.append(random() * 2 - 1)
        self.x = tmpX
        self.y = tmpY
        self.z = tmpZ

    def onSpinPointsChanged(self):
        self.points = self.pointsAmountSpinBox.value()
        self.pointsAmountSlider.setValue(self.points)
        self.updatePoints()

    def onSliderPointsChanged(self):
        self.points = self.pointsAmountSlider.value()
        self.pointsAmountSpinBox.setValue(self.points)
        self.updatePoints()

    def onCoordsChanged(self):
        self.x = []
        self.y = []
        self.z = []
        for w in self.widgets:
            self.x.append(w[0].value())
            self.y.append(w[1].value())
            self.z.append(w[2].value())
        self.OpenGLWidget.updatePoints(self.x, self.y, self.z)

    def createPointWidget(self, x, y, z):
        layout = QHBoxLayout()
        x_label = QLabel("x:")
        x_val = QDoubleSpinBox()
        x_val.setSingleStep(0.01)
        x_val.setMaximum(-100)
        x_val.setMaximum(100)
        x_val.setValue(x)
        x_val.valueChanged.connect(self.onCoordsChanged)
        y_label = QLabel("y:")
        y_val = QDoubleSpinBox()
        y_val.setMaximum(-100)
        y_val.setMaximum(100)
        y_val.setSingleStep(0.01)
        y_val.setValue(y)
        y_val.valueChanged.connect(self.onCoordsChanged)
        z_label = QLabel("z:")
        z_val = QDoubleSpinBox()
        z_val.setMaximum(-100)
        z_val.setMaximum(100)
        z_val.setSingleStep(0.01)
        z_val.setValue(z)
        z_val.valueChanged.connect(self.onCoordsChanged)
        layout.addWidget(x_label)
        layout.addWidget(x_val)
        layout.addWidget(y_label)
        layout.addWidget(y_val)
        layout.addWidget(z_label)
        layout.addWidget(z_val)
        self.widgets.append([x_val, y_val, z_val])
        return layout

    def setupLayout(self):
        self.widgets = []
        # OpenGL widget
        glWidgetW = 800
        glWidgetH = 900
        self.OpenGLWidget = OpenGLView()
        self.OpenGLWidget.setFixedSize(glWidgetW, glWidgetH)
        self.OpenGLWidget.updatePoints(self.x, self.y, self.z)
        '''# number of points
        minPoints = 3
        maxPoints = 10
        self.pointsAmountLabel = QLabel("Number of points: ")
        self.pointsAmountSpinBox = QSpinBox()
        self.pointsAmountSpinBox.setMinimum(minPoints)
        self.pointsAmountSpinBox.setMaximum(maxPoints)
        self.pointsAmountSpinBox.setValue(self.points)
        self.pointsAmountSpinBox.valueChanged.connect(self.onSpinPointsChanged)
        self.pointsAmountSlider = QSlider(QtCore.Qt.Horizontal)
        self.pointsAmountSlider.setValue(self.points)
        self.pointsAmountSlider.setMinimum(minPoints)
        self.pointsAmountSlider.setMaximum(maxPoints)
        self.pointsAmountSlider.valueChanged.connect(self.onSliderPointsChanged)
        self.pointsAmountMinLabel = QLabel("Min: %d" % minPoints)
        self.pointsAmountMinLabel.setFixedWidth(40)
        self.pointsAmountMaxLabel = QLabel("Max: %d" % maxPoints)
        self.pointsAmountMaxLabel.setFixedWidth(60)
        # points amount layout
        layoutAmount = QHBoxLayout()
        layoutAmount.addWidget(self.pointsAmountMinLabel)
        layoutAmount.addWidget(self.pointsAmountSpinBox)
        layoutAmount.addWidget(self.pointsAmountMaxLabel)
        line1 = QFrame()
        line1.setFrameShape(QFrame.HLine)'''
        # point coords layout
        pointsLayout = QVBoxLayout()
        [pointsLayout.addLayout(self.createPointWidget(self.x[i], self.y[i], self.z[i])) for i in range(self.points)]

        # tools panel layout
        layoutTools = QVBoxLayout()
        '''layoutTools.addLayout(layoutAmount)
        layoutTools.addWidget(self.pointsAmountSlider)
        layoutTools.addWidget(line1)'''
        layoutTools.addLayout(pointsLayout)
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
        self.setWindowTitle("lr4")

        self.x = [random() * 2 - 1 for _ in range(self.points)]
        self.y = [random() * 2 - 1 for _ in range(self.points)]
        self.z = [random() * 2 - 1 for _ in range(self.points)]

        self.setupLayout()
    # __init__


if __name__ == '__main__':
    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()
    sys.exit(app.exec_())
