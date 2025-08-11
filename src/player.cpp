#include "player.hpp"

Player::Player(std::shared_ptr<Camera> camera, glm::vec3 position) 
	: camera(camera), position(position) {
	velocity = glm::vec3(0.0f);
}

void Player::tick(float dt) {
	keyboardInput(dt);

	//position += velocity;

	camera->setPosition(glm::vec3(position.x, position.y + 0.65f, position.z));
}

void Player::keyboardInput(float dt) {
	glm::vec3 front = camera->getFront();
	glm::vec3 up = camera->getUp();

	float speed = 5.0f * dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		speed = 25.0f * dt;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		speed = 1.0f * dt;

	speed *= 0.2f;

	//velocity = glm::vec3(0.0f);
	velocity.x *= 0.8;
	velocity.z *= 0.8;

	velocity.y -= 0.05f * dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		velocity += speed * glm::normalize(glm::vec3(front.x, 0, front.z)) * glm::length(front);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		velocity -= speed * glm::normalize(glm::vec3(front.x, 0, front.z)) * glm::length(front);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		velocity -= speed * glm::normalize(glm::cross(front, up));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		velocity += speed * glm::normalize(glm::cross(front, up));
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		velocity += glm::vec3(0, speed, 0);
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		//velocity -= glm::vec3(0, speed, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && grounded) {
		velocity.y = 0.015f;
		grounded = false;
	}

	//position += collisionDirection * speed;
}
