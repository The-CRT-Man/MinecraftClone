#define STB_IMAGE_IMPLEMENTATION

#include "loader.hpp"

#include <vector>
#include <array>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

#include "stb_image.h"
#include <list>

Loader::Loader() {
    stbi_set_flip_vertically_on_load(true);  
}

Loader::~Loader() {
    for (auto vao: vaos)
        glDeleteVertexArrays(1, &vao);

    for (auto vbo: vbos)
        glDeleteBuffers(1, &vbo);

    for (auto texture: textures)
        glDeleteTextures(1, &texture);
}

Model Loader::generateModel(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals) {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);

    glBindVertexArray(vao);

    generateVBO(0, 3, vertices, GL_STATIC_DRAW);
    generateVBO(1, 2, textureCoords, GL_STATIC_DRAW);
    generateVBO(2, 3, normals, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Model model = {vao, vertices.size(), false};

    return model;
}

Model Loader::generateModel(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> normals, std::vector<int> indices) {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);

    glBindVertexArray(vao);

    generateVBO(0, 3, vertices, GL_STATIC_DRAW);
    generateVBO(1, 2, textureCoords, GL_STATIC_DRAW);
    generateVBO(2, 3, normals, GL_STATIC_DRAW);

    generateIndicesVBO(indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Model model = {vao, indices.size(), true};

    return model;
}

Model Loader::generateTexuturelessModel(std::vector<float> vertices, std::vector<int> indices) {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);

    glBindVertexArray(vao);

    generateVBO(0, 3, vertices, GL_STATIC_DRAW);

    generateIndicesVBO(indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Model model = {vao, indices.size(), true};

    return model;
}

Model Loader::generateModelBlockFace(std::vector<float> face, std::vector<int> indices) {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);

    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    vbos.push_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, face.size() * sizeof(float), &face[0], GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    vbos.push_back(ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    Model model = {vao, indices.size(), true};

    return model;
}

BlockModel Loader::generateInstancedBlockMesh(std::vector<float> face, std::vector<int> indices, std::vector<glm::vec3> positions, std::vector<int> textureIDs) {
    std::list<unsigned int> vbos;
    
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    //vaos.push_back(vao);

    glBindVertexArray(vao);

    for (int i = 0; i < 5; i++)
        glEnableVertexAttribArray(i);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    vbos.push_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, face.size() * sizeof(float), &face[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    vbos.push_back(generateInstancedVBO(3, positions));
    vbos.push_back(generateInstancedVBO(4, textureIDs));

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    vbos.push_back(ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    for (int i = 0; i < 5; i++)
        glDisableVertexAttribArray(i);

    BlockModel model(vao, indices.size(), true, vbos);

    return model;
}

Model Loader::generate2DModel(std::vector<float> vertices, std::vector<float> textureCoords, std::vector<int> indices) {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    vaos.push_back(vao);

    glBindVertexArray(vao);

    generateVBO(0, 2, vertices, GL_STATIC_DRAW);
    generateVBO(1, 2, textureCoords, GL_STATIC_DRAW);

    generateIndicesVBO(indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    Model model = { vao, indices.size(), true };

    return model;
}

unsigned int Loader::loadTexture(std::string path) {
    return loadTexture(path, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
}

unsigned int Loader::loadTexture(std::string path, unsigned int interpolation, unsigned int mipmapInterpolation) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    
    unsigned int texture;
    glGenTextures(1, &texture);
    textures.push_back(texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapInterpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture at " << path << "\n";
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture;
}

unsigned int Loader::generateVBO(int index, int coordinateSize, std::vector<float> data, unsigned int type) {
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    vbos.push_back(vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], type);
    glVertexAttribPointer(index, coordinateSize, GL_FLOAT, GL_FALSE, coordinateSize * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

unsigned int Loader::generateInstancedVBO(int index, std::vector<glm::vec3> positionVectors) {
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    //vbos.push_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, positionVectors.size() * sizeof(glm::vec3), &positionVectors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(index, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

unsigned int Loader::generateInstancedVBO(int index, std::vector<int> data) {
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    //vbos.push_back(vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(int), &data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
    glVertexAttribDivisor(index, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

unsigned int Loader::generateIndicesVBO(std::vector<int> indices, unsigned int type) {
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    vbos.push_back(ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], type);

    return ebo;
}
