from math import *
from random import random

import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
from PySide2 import QtCore
from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QOpenGLWidget
from lr4.SplineInterpolate import *
from lr4.BesierCurve import *


class OpenGLView(QOpenGLWidget):
    frameRate = 30
    visionAngleHorizontal = 90.0

    def initializeGL(self):
        glMatrixMode(GL_PROJECTION)
        gluPerspective(self.visionAngleHorizontal, self.width() / self.height(), 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)

        self.__timer = QTimer()
        self.__timer.timeout.connect(self.repaint)
        self.__timer.start(1000 / self.frameRate)

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        gluPerspective(self.visionAngleHorizontal, (width / height), 0.1, 100.0)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glEnable(GL_DEPTH_TEST)
        glDepthFunc(GL_LESS)
        
        glDisable(GL_DEPTH_TEST)
        glFlush()

