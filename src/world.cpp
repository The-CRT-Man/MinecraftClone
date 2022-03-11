#include "world.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <memory>

World::World(Loader& loader, unsigned int texture) : loader(loader) {
	std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks;

	std::cout << "Building World\n";

	for (int x = -8; x < 8; x++) {
		std::vector<std::shared_ptr<Chunk>> chunkColumn;

		for (int y = -8; y < 8; y++) {
			glm::vec2 position(x, y);
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(loader, texture, position);
			chunkColumn.push_back(chunk);
		}

		chunks.push_back(chunkColumn);
	}

	std::cout << "Building Mesh\n";

	for (int x = -8; x < 8; x++)
		for (int y = -8; y < 8; y++) {
			neighbouringChunks = {};

			if (x != -8) neighbouringChunks[Face::Left] = chunks[x + 7][y + 8];
			if (x != 7) neighbouringChunks[Face::Right] = chunks[x + 9][y + 8];

			if (y != -8) neighbouringChunks[Face::Back] = chunks[x + 8][y + 7];
			if (y != 7) neighbouringChunks[Face::Front] = chunks[x + 8][y + 9];

			chunks[x + 8][y + 8]->buildMesh(neighbouringChunks);
		}
}

void World::decorateWorld() {
	std::shared_ptr<Chunk> testChunk = chunks[8][8];

	testChunk->getChunkData();
}

void World::render(std::shared_ptr<ChunkRenderer> renderer) {
	for (auto& chunkColumn : chunks)
		for (auto& chunk: chunkColumn)
			renderer->render(chunk);
}		

void World::tick() {
	for (auto& chunkColumn : chunks)
		for (auto& chunk: chunkColumn)
			chunk->tick();
}