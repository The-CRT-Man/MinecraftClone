#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "model.hpp"
#include "loader.hpp"

std::vector<std::string> splitLine(std::string line, char splitChar);
Model loadObjModel(std::string path, Loader& loader);
void processVertex(std::vector<std::string> vertexData, std::vector<int>& indices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals,
    std::vector<float>& textureArray, std::vector<float>& normalArray);