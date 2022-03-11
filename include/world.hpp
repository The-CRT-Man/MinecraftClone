#pragma once

#include <list>
#include <vector>
#include <memory>

#include "chunk.hpp"
#include "renderer.hpp"

class World {
public:
	World(Loader& loader, unsigned int texture);

	void render(std::shared_ptr<ChunkRenderer> renderer);
	void tick();

private:
	std::vector<std::vector<std::shared_ptr<Chunk>>> chunks;
	void decorateWorld();
	Loader& loader;
};

