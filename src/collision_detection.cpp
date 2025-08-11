#include "collision_detection.hpp"

#include <cmath>
#include <iostream>

const float BLOCK_R = 0.5f;
const float PLAYER_R_XZ = 0.25f;
const float PLAYER_R_Y = 1.75f / 2.0f;

CollisionDetection::CollisionDetection(std::shared_ptr<Player> player, std::shared_ptr<World> world)
	: player(player), world(world) {

}

void CollisionDetection::tick() {
	std::vector<glm::vec3> localBlockPositions;

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			for (int z = -1; z <= 1; z++) {
				glm::vec3 offset = player->position + glm::vec3(x, y, z);
				int blockID = world->getBlockAtPosition(offset);
				if (blockID != 0) {
					int blockX = std::roundf(offset.x);
					int blockY = std::roundf(offset.y);
					int blockZ = std::roundf(offset.z);

					localBlockPositions.push_back(glm::vec3(blockX, blockY, blockZ));
				}	
			}

	// X
	player->position.x += player->velocity.x;

	for (auto& position : localBlockPositions) {
		if (isColliding(position)) {
			if (player->velocity.x > 0)
				player->position.x = position.x - BLOCK_R - PLAYER_R_XZ;
			else if (player->velocity.x < 0)
				player->position.x = position.x + BLOCK_R + PLAYER_R_XZ;
			player->velocity.x = 0;
		}
	}

	// Y
	player->position.y += player->velocity.y;

	for (auto& position : localBlockPositions) {
		if (isColliding(position)) {
			if (player->velocity.y > 0)
				player->position.y = position.y - BLOCK_R - PLAYER_R_Y;
			else if (player->velocity.y < 0) {
				player->position.y = position.y + BLOCK_R + PLAYER_R_Y;
				player->grounded = true;
			}
			player->velocity.y = 0;
		}
	}

	// z
	player->position.z += player->velocity.z;

	for (auto& position : localBlockPositions) {
		if (isColliding(position)) {
			if (player->velocity.z > 0)
				player->position.z = position.z - BLOCK_R - PLAYER_R_XZ;
			else if (player->velocity.z < 0)
				player->position.z = position.z + BLOCK_R + PLAYER_R_XZ;
			player->velocity.z = 0;
		}
	}

	if (player->position.y < -16)
		player->position.y = 92;

}

bool CollisionDetection::isColliding(glm::vec3 blockPosition) {
	return (
		player->position.x - PLAYER_R_XZ < blockPosition.x + BLOCK_R &&
		player->position.x + PLAYER_R_XZ > blockPosition.x - BLOCK_R &&

		player->position.y - PLAYER_R_Y < blockPosition.y + BLOCK_R &&
		player->position.y + PLAYER_R_Y > blockPosition.y - BLOCK_R &&

		player->position.z - PLAYER_R_XZ < blockPosition.z + BLOCK_R &&
		player->position.z + PLAYER_R_XZ > blockPosition.z - BLOCK_R
	);
}