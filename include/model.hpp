#pragma once

#include <cstddef>

struct Model {
    unsigned int vao;
    std::size_t vertexCount;
    bool usesIndices;
};