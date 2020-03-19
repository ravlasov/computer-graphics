import sys

from PySide2 import QtCore
from PySide2.QtWidgets import (QLabel, QComboBox, QApplication, QVBoxLayout, QHBoxLayout, QDialog, QSpacerItem,
                               QSizePolicy, QSpinBox, QSlider, QFrame)
from OpenGLView import OpenGLView


class MainWindow(QDialog):
    def onModeIndexChanged(self):
        self.OpenGLWidget.setMode(self.modeComboBox.currentIndex())
    # onModeIndexChanged

    def onAlphaChanged(self):
        self.OpenGLWidget.setAlphaTest(self.alphaComboBox.currentIndex(), self.alphaSlider.value() / 100)
    # onAlphaChanged

    def onBlendChange(self):
        self.OpenGLWidget.setBlendTest(self.blendSComboBox.currentIndex(), self.blendDComboBox.currentIndex())
    # onBlendChange
    
    def onSpinPointsChanged(self):
        self.OpenGLWidget.setPointsAmount(self.amountSpinBox.value())
        self.amountSlider.setValue(self.amountSpinBox.value())
    # onSpinPointsChanged

    def onSliderPointsChanged(self):
        self.OpenGLWidget.setPointsAmount(self.amountSpinBox.value())
        self.amountSpinBox.setValue(self.amountSlider.value())
    # onSliderPointsChanged

    def onScissorChanged(self):
        self.OpenGLWidget.setScissor(self.scissorX.value(), self.scissorY.value(), self.scissorW.value(),
                                     self.scissorH.value())
    # onScissorChanged

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setFixedSize(1200, 620)
        self.setWindowTitle("Vlasov Roman lr1-2")
        # OpenGL widget
        glWidgetW = 800
        glWidgetH = 600
        self.OpenGLWidget = OpenGLView()
        self.OpenGLWidget.setFixedSize(glWidgetW, glWidgetH)
        self.OpenGLWidget.setMode(0)
        # mode
        self.modeLabel = QLabel("Mode: ")
        self.modeComboBox = QComboBox()
        modes = {
            0: "GL_POINTS",
            1: "GL_LINES",
            2: "GL_LINE_STRIP",
            3: "GL_LINE_LOOP",
            4: "GL_TRIANGLES",
            5: "GL_TRIANGLE_STRIP",
            6: "GL_TRIANGLE_FAN",
            7: "GL_QUADS",
            8: "GL_QUAD_STRIP",
            9: "GL_POLYGON"
        }
        for mode in modes:
            self.modeComboBox.addItem(modes.get(mode))
        self.modeComboBox.currentIndexChanged.connect(self.onModeIndexChanged)
        # number of points
        self.amountLabel = QLabel("Number of points: ")
        self.amountSpinBox = QSpinBox()
        self.amountSpinBox.setMinimum(4)
        self.amountSpinBox.setMaximum(100)
        self.amountSpinBox.valueChanged.connect(self.onSpinPointsChanged)
        self.amountSlider = QSlider(QtCore.Qt.Horizontal)
        self.amountSlider.setMinimum(4)
        self.amountSlider.setMaximum(100)
        self.amountSlider.valueChanged.connect(self.onSliderPointsChanged)
        self.amountMinLabel = QLabel("Min: 4")
        self.amountMinLabel.setFixedWidth(40)
        self.amountMaxLabel = QLabel("Max: 100")
        self.amountMaxLabel.setFixedWidth(60)
        line1 = QFrame()
        line1.setFrameShape(QFrame.HLine)
        layoutAmount = QHBoxLayout()
        layoutAmount.addWidget(self.amountMinLabel)
        layoutAmount.addWidget(self.amountSpinBox)
        layoutAmount.addWidget(self.amountMaxLabel)
        # GL_SCISSOR_TEST
        self.scissorLabel = QLabel("GL_SCISSOR_TEST")
        self.xLabel = QLabel("x")
        self.yLabel = QLabel("y")
        self.wLabel = QLabel("width")
        self.hLabel = QLabel("height")
        line2 = QFrame()
        line2.setFrameShape(QFrame.HLine)
        self.scissorX = QSlider(QtCore.Qt.Horizontal)
        self.scissorY = QSlider(QtCore.Qt.Horizontal)
        self.scissorW = QSlider(QtCore.Qt.Horizontal)
        self.scissorH = QSlider(QtCore.Qt.Horizontal)
        self.scissorX.setMinimum(0)
        self.scissorY.setMinimum(0)
        self.scissorW.setMinimum(0)
        self.scissorH.setMinimum(0)
        self.scissorX.setMaximum(glWidgetW)
        self.scissorY.setMaximum(glWidgetH)
        self.scissorW.setMaximum(glWidgetW)
        self.scissorH.setMaximum(glWidgetH)
        self.scissorX.valueChanged.connect(self.onScissorChanged)
        self.scissorY.valueChanged.connect(self.onScissorChanged)
        self.scissorW.valueChanged.connect(self.onScissorChanged)
        self.scissorH.valueChanged.connect(self.onScissorChanged)
        self.scissorH.setValue(self.scissorH.maximum())
        self.scissorW.setValue(self.scissorW.maximum())
        # GL_ALPHA_TEST
        self.alphaTestLabel = QLabel("GL_ALPHA_TEST")
        self.alphaLabel = QLabel("Function: ")
        self.alphaValueLabel = QLabel("Value: ")
        line3 = QFrame()
        line3.setFrameShape(QFrame.HLine)
        self.alphaComboBox = QComboBox()
        alpha = {
            0: "GL_NEVER",
            1: "GL_LESS",
            2: "GL_EQUAL",
            3: "GL_LEQUAL",
            4: "GL_GREATER",
            5: "GL_NOTEQUAL",
            6: "GL_GEQUAL",
            7: "GL_ALWAYS"
        }
        for mode in alpha:
            self.alphaComboBox.addItem(alpha.get(mode))
        self.alphaComboBox.setCurrentIndex(7)
        self.alphaSlider = QSlider(QtCore.Qt.Horizontal)
        self.alphaSlider.setMinimum(0)
        self.alphaSlider.setMaximum(100)
        self.alphaComboBox.currentIndexChanged.connect(self.onAlphaChanged)
        self.alphaSlider.valueChanged.connect(self.onAlphaChanged)
        # GL_BLEND
        self.blendTestLabel = QLabel("GL_BLEND")
        self.sfactorLabel = QLabel("sfactor: ")
        self.dfactorLabel = QLabel("dfactor: ")
        line4 = QFrame()
        line4.setFrameShape(QFrame.HLine)
        self.blendSComboBox = QComboBox()
        self.blendDComboBox = QComboBox()
        sfactor = {
            0: "GL_ZERO",
            1: "GL_ONE",
            2: "GL_DST_COLOR",
            3: "GL_ONE_MINUS_DST_COLOR",
            4: "GL_SRC_ALPHA",
            5: "GL_ONE_MINUS_SRC_ALPHA",
            6: "GL_DST_ALPHA",
            7: "GL_ONE_MINUS_DST_ALPHA",
            8: "GL_SRC_ALPHA_SATURATE"
        }
        dfactor = {
            0: "GL_ZERO",
            1: "GL_ONE",
            2: "GL_SRC_COLOR",
            3: "GL_ONE_MINUS_SRC_COLOR",
            4: "GL_SRC_ALPHA",
            5: "GL_ONE_MINUS_SRC_ALPHA",
            6: "GL_DST_ALPHA",
            7: "GL_ONE_MINUS_DST_ALPHA"
        }
        for mode in sfactor:
            self.blendSComboBox.addItem(sfactor.get(mode))
        for mode in dfactor:
            self.blendDComboBox.addItem(dfactor.get(mode))
        self.blendSComboBox.currentIndexChanged.connect(self.onBlendChange)
        self.blendDComboBox.currentIndexChanged.connect(self.onBlendChange)
        self.blendSComboBox.setCurrentIndex(1)
        # tools panel layout
        layoutTools = QVBoxLayout()
        layoutTools.addWidget(self.amountLabel)
        layoutTools.addLayout(layoutAmount)
        layoutTools.addWidget(self.amountSlider)
        layoutTools.addWidget(line1)
        layoutTools.addWidget(self.modeLabel)
        layoutTools.addWidget(self.modeComboBox)
        layoutTools.addWidget(line2)
        layoutTools.addWidget(self.scissorLabel)
        layoutTools.addWidget(self.xLabel)
        layoutTools.addWidget(self.scissorX)
        layoutTools.addWidget(self.yLabel)
        layoutTools.addWidget(self.scissorY)
        layoutTools.addWidget(self.wLabel)
        layoutTools.addWidget(self.scissorW)
        layoutTools.addWidget(self.hLabel)
        layoutTools.addWidget(self.scissorH)
        layoutTools.addWidget(line3)
        layoutTools.addWidget(self.alphaTestLabel)
        layoutTools.addWidget(self.alphaLabel)
        layoutTools.addWidget(self.alphaComboBox)
        layoutTools.addWidget(self.alphaValueLabel)
        layoutTools.addWidget(self.alphaSlider)
        layoutTools.addWidget(line4)
        layoutTools.addWidget(self.blendTestLabel)
        layoutTools.addWidget(self.sfactorLabel)
        layoutTools.addWidget(self.blendSComboBox)
        layoutTools.addWidget(self.dfactorLabel)
        layoutTools.addWidget(self.blendDComboBox)
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
