#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "camera.hpp"

class Player {
public:
	Player(std::shared_ptr<Camera> camera, glm::vec3 position);

	void tick(float dt);

	glm::vec3 position;
	glm::vec3 velocity;

	bool grounded = false;

private:
	void keyboardInput(float dt);

	std::shared_ptr<Camera> camera;
};

