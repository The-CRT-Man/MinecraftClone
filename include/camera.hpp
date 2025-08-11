#pragma once

#include <memory>
#include <chrono>

#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>

#include "world.hpp"

class World;

class Camera {
public:
    Camera(std::shared_ptr<sf::RenderWindow> window, float aspectRatio);

    void tick(float dt);

    glm::mat4 getProjection();
    glm::mat4 getView();

    void setCursorLocked(bool locked);
    void updateAspectRatio(float aspectRatio);

    void setPosition(glm::vec3 position) { this->position = position; }
    glm::vec3 getPosition() { return this->position; }
    glm::vec3 getFront() { return this->front; }
    glm::vec3 getUp() { return this->up; }

    glm::vec3 castCollisionRay(std::shared_ptr<World> world, float depth, float quality);
    glm::vec3 castCollisionRaySurface(std::shared_ptr<World> world, float depth, float quality);

private:
    bool cursorLocked = false;
    std::shared_ptr<sf::RenderWindow> window;

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90;
    float pitch = 0;

    void updateCameraFront();
};
