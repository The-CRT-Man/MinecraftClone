#pragma once

#include <cstddef>

#include <list>

struct Model {
    Model() {}
    Model(unsigned int vao, std::size_t vertexCount, bool usesIndices)
        : vao(vao), vertexCount(vertexCount), usesIndices(usesIndices) {}

    unsigned int vao;
    std::size_t vertexCount;
    bool usesIndices;
};

struct BlockModel : public Model {
    BlockModel() {}
    BlockModel(unsigned int vao, std::size_t vertexCount, bool usesIndices)
        : Model(vao, vertexCount, usesIndices) {}
    BlockModel(unsigned int vao, std::size_t vertexCount, bool usesIndices, std::list<unsigned int> vbos)
        : Model(vao, vertexCount, usesIndices), vbos(vbos) {}

    std::list<unsigned int> vbos;
};