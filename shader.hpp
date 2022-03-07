#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
    virtual ~Shader();

    void use();
    void unUse();

    void setMatrix4fv(std::string uniformName, glm::mat4 value);
    void setInt(std::string uniformName, int value);
    void setVector2fv(std::string uniformName, glm::vec2 value);

private:
    unsigned int shaderProgram;

    unsigned int loadShader(const std::string path, const unsigned int type);
    unsigned int linkShaders(const unsigned int vertexShader, const unsigned int fragmentShader);
};

#endif