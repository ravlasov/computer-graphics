#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include "glm/glm.hpp"

class Shader
{
public:
    GLuint Program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();

    void setVec3(const char* uniform, GLfloat x, GLfloat y, GLfloat z);
    void setVec3(const char* uniform, glm::vec3 vec);
    void setMat4(const char* uniform, glm::mat4 mat);
    void setFloat(const char* uniform, GLfloat f);
};

#endif
