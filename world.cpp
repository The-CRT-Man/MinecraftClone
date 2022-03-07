#include "world.hpp"

#include <glm/glm.hpp>

#include <memory>

World::World(Loader& loader, unsigned int texture) {
	for (int x = -8; x < 8; x++)
		for (int y = -8; y < 8; y++) {
			glm::vec2 position(x, y);
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(loader, texture, position);
			chunk->buildMesh();
			chunks.push_back(chunk);
		}
}

void World::finishMesh() {

}

void World::render(std::shared_ptr<ChunkRenderer> renderer) {
	for (auto& chunk : chunks)
		renderer->render(chunk);
}		

void World::tick() {
	for (auto& chunk : chunks)
		chunk->tick();
}