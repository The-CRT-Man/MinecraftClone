#pragma once

#include <list>
#include <memory>

#include "chunk.hpp"
#include "renderer.hpp"

class World {
public:
	World(Loader& loader, unsigned int texture);

	void finishMesh();

	void render(std::shared_ptr<ChunkRenderer> renderer);
	void tick();

private:
	std::list<std::shared_ptr<Chunk>> chunks;
};

