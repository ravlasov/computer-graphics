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
    mousePosX = 0
    mousePosY = 0
    angleHorizontal = -45
    angleVertical = 45
    distance = 1

    mode = True
    x = []
    y = []
    z = []
    actualX = []
    actualY = []
    actualZ = []
    matrix = []

    def updatePoints(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.recount()

    def setMode(self, mode):
        self.mode = mode

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
        self.setView()
        self.drawAxes()

        glLineWidth(3)
        glColor3f(1, 1, 1)
        glBegin(GL_LINE_STRIP)
        if self.mode:
            for i in range(len(self.actualX)):
                glVertex3f(self.actualX[i], self.actualY[i], self.actualZ[i])
        else:
            for i in range(len(self.matrix)):
                glVertex3fv(self.matrix[i])
        glEnd()
        glPointSize(10)
        for i in range(len(self.x)):
            glPushMatrix()
            glTranslatef(self.x[i], self.y[i], self.z[i])
            gluSphere(gluNewQuadric(), 0.02, 32, 32)
            glPopMatrix()
        if not self.mode:
            glEnable(GL_LINE_STIPPLE)
            glLineStipple(1, 0x00FF)
            glLineWidth(1)
            glBegin(GL_LINE_STRIP)
            glColor(1, 0, 1)
            for i in range(len(self.x)):
                glVertex3f(self.x[i], self.y[i], self.z[i])
            glEnd()
            glDisable(GL_LINE_STIPPLE)
        glDisable(GL_DEPTH_TEST)
        glFlush()

    def recount(self):
        # cubic spline interpolation
        self.actualX = []
        self.actualY = []
        self.actualZ = []
        t = [i * 1000 for i in range(len(self.x))]
        splineX = BuildSpline(t, self.x, len(self.x))
        splineY = BuildSpline(t, self.y, len(self.x))
        splineZ = BuildSpline(t, self.z, len(self.x))
        for i in range(t[0], t[len(t) - 1]):
            self.actualX.append(Interpolate(splineX, i))
            self.actualY.append(Interpolate(splineY, i))
            self.actualZ.append(Interpolate(splineZ, i))

        # Besier curve
        self.matrix = np.array([self.x, self.y, self.z]).transpose()
        self.matrix = calculateBesierCurve(self.matrix)


    def setView(self):
        glTranslatef(0.0, 0.0, -1)
        gluLookAt(0.0, 0.0, 1.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0)
        glScalef(self.distance, self.distance, self.distance)
        glRotatef(self.angleVertical, 1.0, 0.0, 0.0)
        glRotatef(self.angleHorizontal, 0.0, 1.0, 0.0)

    def drawAxes(self):
        glBegin(GL_LINES)
        glColor3f(1.0, 0.0, 0.0)
        glVertex3f(-1.0, 0.0, 0.0)
        glVertex3f(1.0, 0.0, 0.0)
        glColor3f(0.0, 1.0, 0.0)
        glVertex3f(0.0, -1.0, 0.0)
        glVertex3f(0.0, 1.0, 0.0)
        glColor3f(0.0, 0.0, 1.0)
        glVertex3f(0.0, 0.0, -1.0)
        glVertex3f(0.0, 0.0, 1.0)
        glEnd()
        # OX
        glPushMatrix()
        glRotatef(90, 0, 1, 0)
        glTranslatef(0, 0, 0.8)
        glColor3f(1.0, 0.0, 0.0)
        gluCylinder(gluNewQuadric(), 0.05, 0, 0.2, 32, 32)
        glPopMatrix()
        # OY
        glPushMatrix()
        glRotatef(-90, 1, 0, 0)
        glTranslatef(0, 0, 0.8)
        glColor3f(0.0, 1.0, 0.0)
        gluCylinder(gluNewQuadric(), 0.05, 0, 0.2, 32, 32)
        glPopMatrix()
        # OZ
        glPushMatrix()
        glTranslatef(0, 0, 0.8)
        glColor3f(0.0, 0.0, 1.0)
        gluCylinder(gluNewQuadric(), 0.05, 0, 0.2, 32, 32)
        glPopMatrix()

    def mousePressEvent(self, event):
        self.mousePosX = event.pos().x()
        self.mousePosY = event.pos().y()

    def mouseMoveEvent(self, event):
        if event.buttons() & QtCore.Qt.MouseButton.RightButton:
            self.angleHorizontal += (event.x() - self.mousePosX) / 2
            self.angleVertical += (event.y() - self.mousePosY) / 2
        self.mousePosX = event.x()
        self.mousePosY = event.y()

    def wheelEvent(self, event):
        if self.distance + (event.delta() / 1000) > 0:
            self.distance += (event.delta() / 1000)
