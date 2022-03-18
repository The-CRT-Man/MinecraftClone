#pragma once

#include <list>
#include <vector>
#include <memory>

#include "chunk.hpp"
#include "renderer.hpp"

class ChunkRenderer;

class World {
public:
	World(Loader& loader, unsigned int texture);

	void setBlock(glm::vec3 position, int blockID);

	void render(std::shared_ptr<ChunkRenderer> renderer);
	int getBlockAtPosition(glm::vec3 position);
	void tick();

private:
	std::vector<std::vector<std::shared_ptr<Chunk>>> chunks;
	void decorateWorld();
	void loadStructure(glm::vec3 position, const Structure& structure);
	Loader& loader;
};

