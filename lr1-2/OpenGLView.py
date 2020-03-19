from PySide2.QtCore import QTimer
from PySide2.QtOpenGL import QGLWidget
from OpenGL.GL import *
from OpenGL.GLU import *
from math import *


class OpenGLView(QGLWidget):
    frameRate = 60
    mode = 0
    alphaTest = 7
    alphaVal = 0
    sfactor = 1
    dfactor = 0
    sides = 4
    scissor = [0, 0, 0, 0]

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

    def setMode(self, newMode):
        self.mode = newMode
    # setMode

    def setPointsAmount(self, n):
        self.sides = n
    # setPointsAmount

    def setScissor(self, x, y, w, h):
        self.scissor = [x, y, w, h]
    # setScissor

    def setAlphaTest(self, a, v):
        self.alphaTest = a
        self.alphaVal = v
    # setAlphaTest

    def setBlendTest(self, s, d):
        self.sfactor = s
        self.dfactor = d
    # setBlendTest

    def paintGL(self):
        modes = {
            0: GL_POINTS,
            1: GL_LINES,
            2: GL_LINE_STRIP,
            3: GL_LINE_LOOP,
            4: GL_TRIANGLES,
            5: GL_TRIANGLE_STRIP,
            6: GL_TRIANGLE_FAN,
            7: GL_QUADS,
            8: GL_QUAD_STRIP,
            9: GL_POLYGON
        }
        colors = {
            0: (0, 0, 1, 0.2),
            1: (0, 1, 1, 0.3),
            2: (1, 0, 1, 0.4),
            3: (1, 0, 0, 0.5),
            4: (0, 1, 0, 0.6),
            5: (0.5, 0.5, 0, 0.7),
            6: (0, 0, 0.5, 0.8),
            7: (0, 0.5, 0.5, 1.0)
        }
        alpha = {
            0: GL_NEVER,
            1: GL_LESS,
            2: GL_EQUAL,
            3: GL_LEQUAL,
            4: GL_GREATER,
            5: GL_NOTEQUAL,
            6: GL_GEQUAL,
            7: GL_ALWAYS
        }
        sfactor = {
            0: GL_ZERO,
            1: GL_ONE,
            2: GL_DST_COLOR,
            3: GL_ONE_MINUS_DST_COLOR,
            4: GL_SRC_ALPHA,
            5: GL_ONE_MINUS_SRC_ALPHA,
            6: GL_DST_ALPHA,
            7: GL_ONE_MINUS_DST_ALPHA,
            8: GL_SRC_ALPHA_SATURATE
        }
        dfactor = {
            0: GL_ZERO,
            1: GL_ONE,
            2: GL_SRC_COLOR,
            3: GL_ONE_MINUS_SRC_COLOR,
            4: GL_SRC_ALPHA,
            5: GL_ONE_MINUS_SRC_ALPHA,
            6: GL_DST_ALPHA,
            7: GL_ONE_MINUS_DST_ALPHA
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(0.0, 0.0, -4.0)
        glEnable(GL_SCISSOR_TEST)
        glEnable(GL_ALPHA_TEST)
        glEnable(GL_BLEND)
        glScissor(self.scissor[0], self.scissor[1], self.scissor[2], self.scissor[3])
        glAlphaFunc(alpha.get(self.alphaTest, GL_NEVER), self.alphaVal)
        glBlendFunc(sfactor.get(self.sfactor, GL_ONE), dfactor.get(self.dfactor, GL_ZERO))
        glBegin(modes.get(self.mode, GL_POINTS))
        posx = 0
        posy = 0
        radius = 1
        for i in range(self.sides):
            glColor4dv(colors.get(i % 7))
            x = radius * cos(i * 2 * pi / self.sides) + posx
            y = radius * sin(i * 2 * pi / self.sides) + posy
            glVertex3f(x, y, 1)
        glEnd()
        glDisable(GL_BLEND)
        glDisable(GL_ALPHA_TEST)
        glDisable(GL_SCISSOR_TEST)
        glFlush()
    # paintGL
