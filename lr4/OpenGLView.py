from OpenGL.GL import *
from OpenGL.GLU import *
from PySide2.QtCore import QTimer
from PySide2.QtOpenGL import QGLWidget


class OpenGLView(QGLWidget):
    frameRate = 30

    def initializeGL(self):
        glMatrixMode(GL_PROJECTION)
        gluPerspective(45.0, self.width() / self.height(), 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)
        self.__timer = QTimer()
        self.__timer.timeout.connect(self.repaint)
        self.__timer.start(1000 / self.frameRate)

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        gluPerspective(45.0, (width / height), 0.1, 100.0)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(0.0, 12.0, -2.0)
        glBegin(GL_POINTS)
        glVertex2f(0, 0)
        glEnd()
        glFlush()
