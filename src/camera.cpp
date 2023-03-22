#include "camera.hpp"

#include <iostream>
#include <cmath>
#include <memory>
#include <chrono>

#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "world.hpp"

Camera::Camera(std::shared_ptr<sf::RenderWindow> window, float aspectRatio) {
    projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.001f, 200.0f);
    this->window = window;
}

void Camera::tick(float dt) {
    if (!this->cursorLocked)
        return;

    //std::cout << dts << "\n";

    float speed = 5.0f * dt;
    float sensitivity = 100.0f * dt;

    //float speed = 0.03f;
    //float sensitivity = 1.0f;

    //std::cout << dt.count() << ", " << dts << ", " << sensitivity << "\n";

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        position += speed * glm::normalize(glm::vec3(front.x, 0, front.z)) * glm::length(front);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        position -= speed * glm::normalize(glm::vec3(front.x, 0, front.z)) * glm::length(front);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        position -= speed * glm::normalize(glm::cross(front, up));
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
        position += speed * glm::normalize(glm::cross(front, up));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
        position += glm::vec3(0, speed, 0);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        position -= glm::vec3(0, speed, 0);

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
    sf::Vector2f mouseOffset = sf::Vector2f(mousePosition - sf::Vector2i(1280 / 2, 720 / 2));

    sf::Mouse::setPosition(sf::Vector2i(1280 / 2, 720 / 2), *window);

    mouseOffset.x *= sensitivity;
    mouseOffset.y *= sensitivity;

    yaw += mouseOffset.x;
    pitch -= mouseOffset.y;

    if (pitch > 89)
        pitch = 89;
    else if (pitch < -89)
        pitch = -89;

    if (yaw > 180)
        yaw -= 360;
    else if (yaw < -180)
        yaw += 360;
    
    updateCameraFront();
}

void Camera::updateCameraFront() {
    glm::vec3 direction(0.0f);

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
}

glm::vec3 Camera::castCollisionRay(World& world, float depth, float quality) {
    glm::vec3 ray = position;

    for (int i = 0; i < (int)(depth / quality); i++) {
        ray += quality * glm::normalize(front);
        if (world.getBlockAtPosition(ray) != 0)
            return glm::vec3(round(ray.x), round(ray.y), round(ray.z));
    }

    return glm::vec3(0.0f, -1.0f, 0.0f);
}

glm::vec3 Camera::castCollisionRaySurface(World& world, float depth, float quality) {
    glm::vec3 ray = position;

    for (int i = 0; i < (int)(depth / quality); i++) {
        ray += quality * glm::normalize(front);
        if (world.getBlockAtPosition(ray) != 0) {
            ray -= quality * glm::normalize(front);
            return glm::vec3(round(ray.x), round(ray.y), round(ray.z));
        }
    }

    return glm::vec3(0.0f, -1.0f, 0.0f);
}

glm::mat4 Camera::getProjection() {
    return projection;
}

glm::mat4 Camera::getView() {
    view = glm::lookAt(position, position + front, up);
    return view;
}

void Camera::setCursorLocked(bool locked) {
    this->cursorLocked = locked;
}

void Camera::updateAspectRatio(float aspectRatio) {
    projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.001f, 200.0f);
}
