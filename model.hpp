#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstddef>

struct Model {
    unsigned int vao;
    std::size_t vertexCount;
    bool usesIndices;
};

#endif