from math import *
from OpenGL.GL import *
from OpenGL.GLU import *
from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QOpenGLWidget


class OpenGLView(QOpenGLWidget):
    frameRate = 30
    visionAngleHorizontal = 90.0
    mousePosX = 0
    mousePosY = 0
    angleHorizontal = 0
    angleVertical = 0

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
        glTranslatef(0.0, 0.0, -1)
        gluLookAt(0.0, 0.0, 1.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0)
        glRotatef(self.angleVertical, 1.0, 0.0, 0.0)
        glRotatef(self.angleHorizontal, 0.0, 1.0, 0.0)
        glFlush()

    def rotateLeftAroundOY(self, point, angle):
        return [
            point[0] * cos(angle * pi / 180) + point[2] * sin(angle * pi / 180),
            point[1],
            - point[0] * sin(angle * pi / 180) + point[2] * cos(angle * pi / 180)
        ]

    def mousePressEvent(self, event):
        self.mousePosX = event.pos().x()
        self.mousePosY = event.pos().y()

    def mouseMoveEvent(self, event):
        self.angleHorizontal += (event.x() - self.mousePosX)
        self.angleVertical += (event.y() - self.mousePosY)
        self.mousePosX = event.x()
        self.mousePosY = event.y()
