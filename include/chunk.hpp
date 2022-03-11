#pragma once

#include <array>
#include <memory>
#include <list>
#include <unordered_map>

#include <glm/glm.hpp>

#include "model.hpp"
#include "loader.hpp"

const unsigned int WIDTH = 16;
const unsigned int HEIGHT = 256;

typedef std::array<std::array<std::array<int, WIDTH>, HEIGHT>, WIDTH> Array3D;
typedef std::vector<std::vector<std::vector<int>>> Structure;

enum class Face { Top, Bottom, Left, Right, Front, Back };

class Chunk {
public:
    Chunk(Loader& loader, unsigned int texture, glm::vec2 position);

    void tick();

    void buildMesh(std::unordered_map<Face, std::shared_ptr<Chunk>>& neighbouringChunks);
    void removeFaceFromMesh(glm::vec3 position, Face face);
    void finaliseMesh();

    std::unordered_map<Face, std::vector<glm::vec3>>& getFacePositions();
    std::unordered_map<Face, std::vector<int>>& getFaceTextureIDs();

    std::vector<Model*> getModels();
    unsigned int getTexture() { return texture; }
    unsigned int getNumberOfFaces(unsigned int faceIndex);
    glm::vec2 getPosition();
    glm::mat4 getTransform();
    std::shared_ptr<Array3D> getChunkData();

private:
    std::unordered_map<Face, std::vector<float>*> faceVertices;

    std::unordered_map<Face, std::vector<glm::vec3>> facePositions;
    std::unordered_map<Face, std::vector<int>> faceTextureIDs;

    std::shared_ptr<Array3D> chunkData;
    Loader& loader;

    std::unordered_map<Face, Model> models;
    std::unordered_map<Face, unsigned int> numberOfFaces;

    glm::vec2 position;

    unsigned int texture;

    bool shouldAddFace(int x, int y, int z, Face face, std::unordered_map<Face, std::shared_ptr<Chunk>>& neighbouringChunks, std::unordered_map<Face, bool>& existingNeighbours);
    int getTextureID(Face face, int blockID);
    void loadStructure(glm::vec3 position, Structure& structure);
};