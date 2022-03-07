#pragma once

#include <array>
#include <memory>
#include <list>
#include <unordered_map>

#include <glm/glm.hpp>

#include "model.hpp"
#include "loader.hpp"

const unsigned int WIDTH = 16;
const unsigned HEIGHT = 256;

typedef std::array<std::array<std::array<int, WIDTH>, HEIGHT>, WIDTH> Array3D;

enum class Face { Top, Bottom, Left, Right, Front, Back };

class Chunk {
public:
    Chunk(Loader& loader, unsigned int texture, glm::vec2 position);

    void tick();

    void buildMesh();

    std::vector<Model*> getModels();
    unsigned int getTexture() { return texture; }
    unsigned int getNumberOfFaces(unsigned int faceIndex);
    glm::mat4 getTransform();

private:
    std::unordered_map<Face, std::vector<float>*> faceVertices;

    std::unique_ptr<Array3D> chunkData;
    Loader& loader;

    /*
    Model topFaceM;
    Model bottomFaceM;
    Model leftFaceM;
    Model rightFaceM;
    Model frontFaceM;
    Model backFaceM;*/
    std::unordered_map<Face, Model> models;

    /*
    unsigned int topFaces;
    unsigned int bottomFaces;
    unsigned int leftFaces;
    unsigned int rightFaces;
    unsigned int frontFaces;
    unsigned int backFaces;*/
    std::unordered_map<Face, unsigned int> numberOfFaces;

    glm::vec2 position;

    unsigned int texture;

    bool shouldAddFace(int x, int y, int z, Face face);
    int getTextureID(Face face, int blockID);
};