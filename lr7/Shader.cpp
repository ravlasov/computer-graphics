#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
    
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
    
        vShaderFile.close();
        fShaderFile.close();
    
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use(){ 
    glUseProgram(this->Program);
}

void Shader::setVec3(std::string uniform, GLfloat x, GLfloat y, GLfloat z){
    GLint Loc = glGetUniformLocation(Program, uniform.c_str());
    glUniform3f(Loc, x, y, z);
}

void Shader::setVec3(std::string uniform, glm::vec3 vec){
    GLint Loc = glGetUniformLocation(Program, uniform.c_str());
    glUniform3f(Loc, vec.x, vec.y, vec.z);
}

void Shader::setMat4(std::string uniform, glm::mat4 mat){
   GLint uniformLoc = glGetUniformLocation(Program, uniform.c_str());
   glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setFloat(std::string uniform, GLfloat f){
    GLint Loc = glGetUniformLocation(Program, uniform.c_str());
    glUniform1f(Loc, f);
}

void Shader::setBool(std::string uniform, bool b){
    GLint Loc = glGetUniformLocation(Program, uniform.c_str());
    glUniform1i(Loc, b);
}
