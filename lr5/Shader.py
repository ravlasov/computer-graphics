from OpenGL.GL import *
from OpenGL.GLUT import *



class Shader:
    def __init__(self) -> None:
        self.program = glCreateProgram()

    def __del__(self) -> None:
        glDeleteProgram(self.program)

    def compile(self, vs_src: str, fs_src: str) -> None:
        vs = self.load_shader(vs_src, GL_VERTEX_SHADER)
        if not vs:
            return
        fs = self.load_shader(fs_src, GL_FRAGMENT_SHADER)
        if not fs:
            return
        glAttachShader(self.program, vs)
        glAttachShader(self.program, fs)
        glLinkProgram(self.program)
        error = glGetProgramiv(self.program, GL_LINK_STATUS)
        glDeleteShader(vs)
        glDeleteShader(fs)
        if error != GL_TRUE:
            info = glGetShaderInfoLog(self.program)
            raise Exception(info)

    def use(self):
        glUseProgram(self.program)

    def unuse(self):
        glUseProgram(0)

    def load_shader(self, src: str, shader_type: int) -> int:
        shader = glCreateShader(shader_type)
        glShaderSource(shader, src)
        glCompileShader(shader)
        error = glGetShaderiv(shader, GL_COMPILE_STATUS)
        if error != GL_TRUE:
            info = glGetShaderInfoLog(shader)
            glDeleteShader(shader)
            raise Exception(info)
        return shader
