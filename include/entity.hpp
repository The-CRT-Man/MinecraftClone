#pragma once

#include "model.hpp"
#include "loader.hpp"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Entity {
    unsigned int texture;
    Model model;

    glm::mat4 transform = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    Entity(unsigned int texture, Model model) : texture(texture), model(model) {}

    void tick() {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
    }
};

struct HUDElement {
    std::vector<float> vertices2D = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
    -0.5f,  0.5f,
     0.5f,  0.5f
    };

    std::vector<float> textureCoords2D = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    std::vector<int> indices2D = {
        0, 1, 2, 1, 3, 2
    };

    unsigned int texture;
    Model model;

    glm::mat3 transform = glm::mat3(1.0f);
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 scale = glm::vec2(1.0f, 1.0f);

    HUDElement(unsigned int texture, Loader& loader)
        : texture(texture) {
        model = loader.generate2DModel(vertices2D, textureCoords2D, indices2D);
    }

    void tick() {
        transform = glm::mat3(1.0f);
        transform[0][2] = position.x;
        transform[1][2] = position.y;
        transform[0][0] = scale.x;
        transform[1][1] = scale.y;
    }
};