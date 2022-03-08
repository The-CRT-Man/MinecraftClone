#include "world.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <memory>

World::World(Loader& loader, unsigned int texture) {
	for (int x = -8; x < 8; x++) {
		std::vector<std::shared_ptr<Chunk>> chunkColumn;

		for (int y = -8; y < 8; y++) {
			glm::vec2 position(x, y);
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(loader, texture, position);
			chunk->buildMesh();
			chunkColumn.push_back(chunk);
		}

		chunks.push_back(chunkColumn);
	}
}

void World::finishMesh() {
	for (int y = -8; y < 8; y++)
		for (int x = -8; x < 7; x++) {
			std::shared_ptr<Chunk> leftChunk = chunks[x + 8][y + 8];
			std::shared_ptr<Chunk> rightChunk = chunks[x + 9][y + 8];

			auto leftChunkData = leftChunk->getChunkData();
			auto rightChunkData = rightChunk->getChunkData();

			for (int by = 0; by < HEIGHT; by++)
				for (int bz = 0; bz < WIDTH; bz++) {
					if (((*leftChunkData)[WIDTH - 1][by][bz] != 0) == ((*rightChunkData)[0][by][bz] != 0)) {
						leftChunk->removeFaceFromMesh(glm::vec3(WIDTH - 1, by, bz), Face::Right);
						rightChunk->removeFaceFromMesh(glm::vec3(0, by, bz), Face::Left);
					}
				}
					
		}

	for (int x = -8; x < 8; x++)
		for (int y = -8; y < 7; y++) {
			std::shared_ptr<Chunk> leftChunk = chunks[x + 8][y + 8];
			std::shared_ptr<Chunk> rightChunk = chunks[x + 8][y + 9];

			auto leftChunkData = leftChunk->getChunkData();
			auto rightChunkData = rightChunk->getChunkData();

			for (int by = 0; by < HEIGHT; by++)
				for (int bx = 0; bx < WIDTH; bx++) {
					if (((*leftChunkData)[bx][by][WIDTH - 1] != 0) == ((*rightChunkData)[bx][by][0] != 0)) {
						leftChunk->removeFaceFromMesh(glm::vec3(bx, by, WIDTH - 1), Face::Front);
						rightChunk->removeFaceFromMesh(glm::vec3(bx, by, 0), Face::Back);
					}
				}
		}

	for (auto& chunkColumn : chunks)
		for (auto& chunk : chunkColumn)
			chunk->finaliseMesh();
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