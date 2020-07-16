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

    void setVec3(std::string uniform, GLfloat x, GLfloat y, GLfloat z);
    void setVec3(std::string uniform, glm::vec3 vec);
    void setMat4(std::string uniform, glm::mat4 mat);
    void setFloat(std::string uniform, GLfloat f);
    void setBool(std::string uniform, bool b);
};

#endif
