#ifndef LOADER_HPP
#define LOADER_HPP

#include <vector>
#include <array>
#include <string>

#include <glm/glm.hpp>

#include "model.hpp"

class Loader {
public:
    Loader();
    virtual ~Loader();

    Model generateModel(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals);
    Model generateModel(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals, std::vector<int> indices);
    Model generateModelBlockFace(std::vector<float> face, std::vector<int> indices);
    Model generateInstancedBlockMesh(std::vector<float> face, std::vector<int> indices, std::vector<glm::vec3> positions, std::vector<int> textureIDs);

    unsigned int loadTexture(std::string path, unsigned int interpolation, unsigned int mipmapInterpolation);
    unsigned int loadTexture(std::string path);

private:
    std::vector<unsigned int> vaos;
    std::vector<unsigned int> vbos;
    std::vector<unsigned int> textures;

    unsigned int generateVBO(int index, int coordinateSize, std::vector<float> data, unsigned int type);
    unsigned int generateInstancedVBO(int index, std::vector<glm::vec3> positionVectors);
    unsigned int generateInstancedVBO(int index, std::vector<int> data);
    unsigned int generateIndicesVBO(std::vector<int> indices, unsigned int type);
};

#endif