#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Entity {
    unsigned int texture;
    Model model;

    glm::mat4 transform = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    Entity(unsigned int texture, Model model) : model(model) {
        this->texture = texture;
        //this->model = model;
    }

    void tick() {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
    }
};

#endif