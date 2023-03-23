#include "chunk.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <list>
#include <cstdlib>
#include <unordered_map>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "loader.hpp"
#include "entity.hpp"
#include "renderer.hpp"
#include "perlin.hpp"

std::vector<float> frontFace = {
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f
};

std::vector<float> backFace = {
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f
};

std::vector<float> rightFace = {
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f
};

std::vector<float> leftFace = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f
};

std::vector<float> topFace = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f
};

std::vector<float> bottomFace = {
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f
};

std::vector<int> faceIndices = {
    0, 1, 3,
    3, 1, 2
};

const std::vector<Face> allFaces = { Face::Top, Face::Bottom, Face::Left, Face::Right, Face::Front, Face::Back };

Chunk::Chunk(Loader& loader, unsigned int texture, glm::vec2 position)
    : loader(loader), texture(texture), position(position) {
    this->chunkData = std::make_shared<Array3D>();

    faceVertices[Face::Top] = &topFace;
    faceVertices[Face::Bottom] = &bottomFace;
    faceVertices[Face::Left] = &leftFace;
    faceVertices[Face::Right] = &rightFace;
    faceVertices[Face::Front] = &frontFace;
    faceVertices[Face::Back] = &backFace;

    Perlin perlin(372498748);

    for (unsigned int x = 0; x < WIDTH; x++) {
        for (unsigned int z = 0; z < WIDTH; z++) {
            int randomValue = std::rand() % 100;

            float perlinX = (x) / ((float)WIDTH) + position.x;
            float perlinY = (z) / ((float)WIDTH) + position.y;

            int height = (int)(20 * perlin.getPerlin(perlinX, perlinY) + 64);
            if (height < 1)
                height = 1;
            if (height > HEIGHT)
                height = HEIGHT;

            for (unsigned int y = 0; y < height - 4; y++) {
                (*chunkData)[x][y][z] = 1;
            }
            for (unsigned int y = height - 4; y < height - 1; y++) {
                (*chunkData)[x][y][z] = 3;
            }
            (*chunkData)[x][height - 1][z] = 2;
            for (unsigned int y = height; y < HEIGHT; y++) {
                (*chunkData)[x][y][z] = 0;
            }
        }
    }
}

Chunk::~Chunk() {
    for (Face face : allFaces) {
        glDeleteVertexArrays(1, &models[face].vao);

        for (auto& vbo : models[face].vbos)
            glDeleteBuffers(1, &vbo);
    }
}

void Chunk::setBlock(glm::vec3 position, int blockID, std::unordered_map<Face, std::shared_ptr<Chunk>>& neighbouringChunks) {
    (*chunkData)[position.x][position.y][position.z] = blockID;

    rebuildMesh(neighbouringChunks);
}

void Chunk::rebuildMesh(std::unordered_map<Face, std::shared_ptr<Chunk>>& neighbouringChunks) {
    facePositions.clear();
    faceTextureIDs.clear();

    for (Face face : allFaces) {
        glDeleteVertexArrays(1, &models[face].vao);

        for (auto& vbo : models[face].vbos)
            glDeleteBuffers(1, &vbo);
    }

    buildMesh(neighbouringChunks);
}

void Chunk::buildMesh(std::unordered_map<Face, std::shared_ptr<Chunk>>& neighbouringChunks) {
    std::unordered_map<Face, bool> existingNeighbours = {};
    for (auto& face : allFaces)
        existingNeighbours[face] = (neighbouringChunks.find(face) == neighbouringChunks.end());

    for (unsigned int x = 0; x < WIDTH; x++)
        for (unsigned int y = 0; y < HEIGHT; y++)
            for (unsigned int z = 0; z < WIDTH; z++) {
                int blockID = (*chunkData)[x][y][z];

                if (blockID == 0) continue;
     
                for (auto face : allFaces) {
                    if (shouldAddFace(x, y, z, face, neighbouringChunks, existingNeighbours)) {
                        facePositions[face].push_back(glm::vec3(x, y, z));
                        faceTextureIDs[face].push_back(getTextureID(face, blockID));
                    }
                }
            }
    
    finaliseMesh();
}

void Chunk::removeFaceFromMesh(glm::vec3 position, Face face) {
    int i = 0;
    for (auto& facePosition : facePositions[face]) {
        if (facePosition == position) {
            facePositions[face].erase(facePositions[face].begin() + i);
            faceTextureIDs[face].erase(faceTextureIDs[face].begin() + i);
        }
        i++;
    }
}

void Chunk::finaliseMesh() {
    for (auto face : allFaces)
        numberOfFaces[face] = facePositions[face].size();

    for (auto face : allFaces) {
        if (facePositions[face].size() == 0 || faceTextureIDs.size() == 0)
            // TODO: load empty model
            models[face] = BlockModel(0, 0, true);
        else
            models[face] = loader.generateInstancedBlockMesh(*faceVertices[face], faceIndices, facePositions[face], faceTextureIDs[face]);
        
    }
}

std::unordered_map<Face, std::vector<glm::vec3>>& Chunk::getFacePositions() {
    return facePositions;
}

std::unordered_map<Face, std::vector<int>>& Chunk::getFaceTextureIDs() {
    return faceTextureIDs;
}

bool Chunk::shouldAddFace(int x, int y, int z, Face face, std::unordered_map<Face, std::shared_ptr<Chunk>>& neighbouringChunks, std::unordered_map<Face, bool>& existingNeighbours) {
    if (face == Face::Top) {
        if (y == HEIGHT - 1)
            return true;
        else
            return ((*chunkData)[x][y + 1][z] == 0);
    } 
    else if (face == Face::Bottom) {
        if (y == 0)
            return true;
        else
            return ((*chunkData)[x][y - 1][z] == 0);
    } 
    else if (face == Face::Left) {
        if (x == 0)
            if (!existingNeighbours[Face::Left])
                return ((*neighbouringChunks[Face::Left]->getChunkData())[WIDTH - 1][y][z] == 0);
            else
                return true;
        else
            return ((*chunkData)[x - 1][y][z] == 0);
    } 
    else if (face == Face::Right) {
        if (x == WIDTH - 1)
            if (!existingNeighbours[Face::Right])
                return ((*neighbouringChunks[Face::Right]->getChunkData())[0][y][z] == 0);
            else
                return true;
        else
            return ((*chunkData)[x + 1][y][z] == 0);
    } 
    else if (face == Face::Back) {
        if (z == 0)
            if (!existingNeighbours[Face::Back])
                return ((*neighbouringChunks[Face::Back]->getChunkData())[x][y][WIDTH - 1] == 0);
            else
                return true;
        else
            return ((*chunkData)[x][y][z - 1] == 0);
    } 
    else if (face == Face::Front) {
        if (z == WIDTH - 1)
            if (!existingNeighbours[Face::Front])
                return ((*neighbouringChunks[Face::Front]->getChunkData())[x][y][0] == 0);
            else
                return true;
        else
            return ((*chunkData)[x][y][z + 1] == 0);
    } 

    return false;
}

void Chunk::tick() {
    
}

std::vector<Model*> Chunk::getModels() {
    std::vector<Model*> models;

    for (auto& face : allFaces)
        models.push_back(&this->models[face]);

    return models;
}

unsigned int Chunk::getNumberOfFaces(unsigned int faceIndex) {
    Face face = allFaces[faceIndex];
    return numberOfFaces[face];
}

glm::mat4 Chunk::getTransform() {
    glm::vec3 worldPosition = glm::vec3(position.x * WIDTH, 0, position.y * WIDTH);

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, worldPosition);

    return transform;
}

int Chunk::getTextureID(Face face, int blockID) {
    int textureID = 0;

    switch (blockID) {
    case 1:
        textureID = 0;
        break;
    case 2:
        if (face == Face::Top)
            textureID = 2;
        else
            textureID = 1;
        break;
    case 3:
        textureID = 3;
        break;
    case 4:
        if (face == Face::Top || face == Face::Bottom)
            textureID = 5;
        else
            textureID = 4;
        break;
    case 5:
        textureID = 6;
        break;
    case 6:
        textureID = 7;
        break;
    }

    return textureID;
}

glm::vec2 Chunk::getPosition() {
    return position;
}

std::shared_ptr<Array3D> Chunk::getChunkData() {
    return chunkData;
}