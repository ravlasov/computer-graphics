from PySide2.QtCore import QTimer
from PySide2.QtOpenGL import QGLWidget
from OpenGL.GL import *
from OpenGL.GLU import *
from math import *


class OpenGLView(QGLWidget):
    iterations = 0
    frameRate = 15

    def initializeGL(self):
        glMatrixMode(GL_PROJECTION)
        gluPerspective(45.0, self.width() / self.height(), 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)
        self.__timer = QTimer()
        self.__timer.timeout.connect(self.repaint)
        self.__timer.start(1000 / self.frameRate)

    # initializeGL

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        gluPerspective(45.0, (width / height), 0.1, 100.0)

    # resizeGL

    def setIterations(self, n):
        self.iterations = n

    # setPointsAmount

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(0.0, 12.0, -95.0)
        glRotatef(5-2 * self.iterations, 0.0, 0.0, 1.0)
        startPoint = [0, 0]
        startLength = 30
        startAngle = -5
        rotateLeft = 46
        rotateRight = 42
        self.triangle(startPoint, startLength, startAngle)

        tmpP = startPoint
        tmpA = startAngle
        glBegin(GL_LINES)
        for i in range(0, self.iterations):
            glVertex2fv(self.movePoint(self.rotateLeftAroundCenter([0, 0], tmpA), tmpP))
            tmpP = self.movePoint(self.rotateLeftAroundCenter([0, -startLength * 9 / 32], tmpA), tmpP)
            glVertex2fv(tmpP)
            tmpA = tmpA + 2
        glEnd()

        for i in range(0, self.iterations):
            startPoint = [startPoint[0], startPoint[1] - min(i, 1) * startLength / 5]
            pNew = self.movePoint(self.rotateLeftAroundCenter([0 - i * startLength / 15, 0], startAngle ), startPoint)
            self.fractal(pNew, startLength * 3 / 8, startAngle + rotateLeft + i, rotateLeft, rotateRight, i)
            pNew = self.movePoint(self.rotateLeftAroundCenter([0 + i * startLength / 15, -startLength * 9 / 64],  startAngle), startPoint)
            self.fractal(pNew, startLength * 3 / 8, startAngle - rotateRight + i, rotateLeft, rotateRight, i)
        glFlush()
    # paintGL

    def fractal(self, point, length, angle, rotateLeft, rotateRight, iterations):
        self.triangle(point, length, angle)
        if iterations == 0:
            return
        tmpP = point
        tmpA = angle
        glBegin(GL_LINES)
        for i in range(0, iterations):
            glVertex2fv(self.movePoint(self.rotateLeftAroundCenter([0, 0], tmpA), tmpP))
            tmpP = self.movePoint(self.rotateLeftAroundCenter([0, -length * 9 / 32], tmpA), tmpP)
            glVertex2fv(tmpP)
            tmpA = tmpA + 2
        glEnd()
        for i in range(0, iterations):
            dif = self.rotateLeftAroundCenter([0, -min(i, 1) * length / 5], angle)
            point = [point[0] + dif[0], point[1] + dif[1]]
            pNew = self.movePoint(self.rotateLeftAroundCenter([0 - (length + i) / 15, 0], angle), point)
            self.fractal(pNew, length * 3 / 8, angle + rotateLeft + 2 * i, rotateLeft, rotateRight, iterations - 1)
            pNew = self.movePoint(self.rotateLeftAroundCenter([0 + (length + i) / 15, -length * 9 / 64], angle), point)
            self.fractal(pNew, length * 3 / 8, angle - rotateRight + 2 * i, rotateLeft, rotateRight, iterations - 1)

    def rotateLeftAroundCenter(self, point, angle):
        return [point[0] * cos(angle * pi / 180) - point[1] * sin(angle * pi / 180),
                point[0] * sin(angle * pi / 180) + point[1] * cos(angle * pi / 180)]

    def movePoint(self, point, moveTo):
        return [point[0] + moveTo[0], point[1] + moveTo[1]]

    def triangle(self, p, length, angle):
        h = length * sqrt(3) / 2
        a = self.movePoint(self.rotateLeftAroundCenter([0, h], angle), p)
        b = self.movePoint(self.rotateLeftAroundCenter([length / 2, 0], angle), p)
        c = self.movePoint(self.rotateLeftAroundCenter([-length / 2, 0], angle), p)
        glBegin(GL_LINE_LOOP)
        glVertex2fv(a)
        glVertex2fv(b)
        glVertex2fv(c)
        glEnd()



