#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "world.hpp"
#include "player.hpp"

class CollisionDetection {
public:
	CollisionDetection(std::shared_ptr<Player> player, std::shared_ptr<World> world);

	void tick();

private:
	bool isColliding(glm::vec3 blockPosition);

	std::shared_ptr<Player> player;
	std::shared_ptr<World> world;
};

