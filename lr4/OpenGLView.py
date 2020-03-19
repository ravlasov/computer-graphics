from math import *
from random import random

import numpy
from OpenGL.GL import *
from OpenGL.GLU import *
from PySide2 import QtCore
from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QOpenGLWidget
from lr4.SplineInterpolate import *


class OpenGLView(QOpenGLWidget):
    frameRate = 30
    visionAngleHorizontal = 90.0
    mousePosX = 0
    mousePosY = 0
    angleHorizontal = 45
    angleVertical = 45
    distance = 1

    x = []
    y = []
    z = []
    actualX = []
    actualY = []
    actualZ = []

    def updatePoints(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.recount()

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
        glBegin(GL_LINE_STRIP)
        glColor3f(1, 1, 1)
        for i in range(len(self.actualX)):
            glVertex3f(self.actualX[i], self.actualY[i], self.actualZ[i])
        glEnd()
        glPointSize(10)
        glBegin(GL_POINTS)
        for i in range(len(self.x)):
            glVertex3f(self.x[i], self.y[i], self.z[i])
        glEnd()
        
        glDisable(GL_DEPTH_TEST)
        glFlush()

    def recount(self):
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
        if event.buttons() & QtCore.Qt.MouseButton.LeftButton:
            self.x[1] += (event.x() - self.mousePosX) / 500
            self.y[1] += (event.y() - self.mousePosY) / 500
            self.recount()
        elif event.buttons() & QtCore.Qt.MouseButton.RightButton:
            self.angleHorizontal += (event.x() - self.mousePosX) / 2
            self.angleVertical += (event.y() - self.mousePosY) / 2
        self.mousePosX = event.x()
        self.mousePosY = event.y()

    def wheelEvent(self, event):
        if event.buttons() & QtCore.Qt.MouseButton.LeftButton:
            self.z[1] += event.delta() / 1000
            self.recount()
        elif self.distance + (event.delta() / 1000) > 0:
            self.distance += (event.delta() / 1000)
