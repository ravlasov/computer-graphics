import os
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"
import pygame
from OpenGL.GLU import *
from PySide2.QtCore import QTimer
from PySide2.QtWidgets import QOpenGLWidget

from lr5.Shader import *

VS_SOURCE = "./shader.vert"
FS_SOURCE = "./shader.frag"
TEX_SOURCE = "./texture.png"

class OpenGLView(QOpenGLWidget):
    frameRate = 30
    visionAngleHorizontal = 90.0


    def initializeGL(self):
        glDepthRange(0, 1)
        glMatrixMode(GL_PROJECTION)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glShadeModel(GL_SMOOTH)
        glClearColor(0.0, 0.0, 0.0, 0.0)
        glClearDepth(1.0)
        glDisable(GL_DEPTH_TEST)
        glDisable(GL_LIGHTING)
        glDepthFunc(GL_LEQUAL)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
        glEnable(GL_BLEND)

        surf = pygame.image.load(TEX_SOURCE)
        s = pygame.image.tostring(surf, 'RGBA')
        self.texID = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, self.texID)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf.get_rect()[2], surf.get_rect()[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, s)
        glGenerateMipmap(GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, 0)

        VS = open(VS_SOURCE, "r").read()
        FS = open(FS_SOURCE, "r").read()

        self.shader = Shader()
        self.shader.compile(VS, FS)

        self.__timer = QTimer()
        self.__timer.timeout.connect(self.repaint)
        self.__timer.start(1000 / self.frameRate)

    def resizeGL(self, width, height):
        glViewport(0, 0, width, height)
        gluPerspective(self.visionAngleHorizontal, (width / height), 0.1, 100.0)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT)
        glLoadIdentity()
        glDisable(GL_LIGHTING)
        glEnable(GL_TEXTURE_2D)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glClearColor(0, 0, 0, 1.0)

        self.shader.use()

        glBindTexture(GL_TEXTURE_2D, self.texID)
        glBegin(GL_QUADS)
        glTexCoord2f(1, 1)
        glVertex2f(-0.5, -0.5)
        glTexCoord2f(1, 0)
        glVertex2f(-0.5, 0.5)
        glTexCoord2f(0, 0)
        glVertex2f(0.5, 0.5)
        glTexCoord2f(0, 1)
        glVertex2f(0.5, -0.5)
        glEnd()

        self.shader.unuse()
