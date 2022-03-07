#include "shader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    unsigned int vertexShader = this->loadShader(vertexShaderPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = this->loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
    this->shaderProgram = this->linkShaders(vertexShader, fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(this->shaderProgram);
}

void Shader::use() {
    glUseProgram(this->shaderProgram);
}

void Shader::unUse() {
    glUseProgram(0);
}

unsigned int Shader::loadShader(const std::string path, const unsigned int type) {
    std::ifstream file;
    std::string sourceString;
    const char* source;

    try {
        file.open(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        sourceString = buffer.str();
        source = sourceString.c_str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR!! \n";
    }

    int success;
    char infoLog[512];
    unsigned int shader;
    shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Error compiling shader\n" << infoLog << "\n";
    }

    return shader;
}

unsigned int Shader::linkShaders(const unsigned int vertexShader, const unsigned int fragmentShader) {
    int success;
    char infoLog[512];
    unsigned int program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetShaderiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Error linking shader\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Shader::setMatrix4fv(std::string uniformName, glm::mat4 value) {
    unsigned int location = glGetUniformLocation(this->shaderProgram, uniformName.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVector2fv(std::string uniformName, glm::vec2 value) {
    unsigned int location = glGetUniformLocation(shaderProgram, uniformName.c_str());
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setInt(std::string uniformName, int value) {
    unsigned int location = glGetUniformLocation(shaderProgram, uniformName.c_str());
    glUniform1i(location, value);
}