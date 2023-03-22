#include "world.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <cmath>

#include "perlin.hpp"

inline int mod(int a, int b) {
	return (a % b + b) % b;
}

std::vector<std::vector<std::vector<int>>> tree = {
	{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 4, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 4, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{5, 5, 5, 5, 5},
		{5, 5, 5, 5, 5},
		{5, 5, 4, 5, 5},
		{5, 5, 5, 5, 5},
		{5, 5, 5, 5, 5}
	},
	{
		{0, 5, 5, 5, 0},
		{5, 5, 5, 5, 5},
		{5, 5, 4, 5, 5},
		{5, 5, 5, 5, 5},
		{0, 5, 5, 5, 5}
	},
	{
		{0, 0, 0, 0, 0},
		{0, 0, 5, 0, 0},
		{0, 5, 4, 5, 0},
		{0, 0, 5, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0, 0},
		{0, 0, 5, 0, 0},
		{0, 5, 5, 5, 0},
		{0, 0, 5, 0, 0},
		{0, 0, 0, 0, 0}
	}
};

World::World(Loader& loader, unsigned int texture) : loader(loader) {
	generateWorld(texture);
	buildMesh();
}

void World::generateWorld(unsigned int texture) {
	for (int x = -8; x < 8; x++) {
		std::vector<std::shared_ptr<Chunk>> chunkColumn;

		for (int y = -8; y < 8; y++) {
			glm::vec2 position(x, y);
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(loader, texture, position);
			chunkColumn.push_back(chunk);
		}

		chunks.push_back(chunkColumn);
	}

	decorateWorld();
}

void World::buildMesh() {
	std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks;

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

void World::setBlock(glm::vec3 position, int blockID) {
	glm::vec2 worldChunkCoords(floor(position.x / WIDTH), floor(position.z / WIDTH));
	//int x = floor(position.x / WIDTH);
	//int y = floor(position.z / WIDTH);

	glm::vec3 chunkPosition(mod((int)position.x, WIDTH), position.y, mod((int)position.z, WIDTH));

	std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks = getNeighbouringChunks(worldChunkCoords);
	std::shared_ptr<Chunk> chunk = chunks[worldChunkCoords.x + 8][worldChunkCoords.y + 8];

	chunk->setBlock(chunkPosition, blockID, neighbouringChunks);

	if (chunkPosition.x == WIDTH - 1 && worldChunkCoords.x != 7) {
		std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks2 = getNeighbouringChunks(glm::vec2(worldChunkCoords.x + 1, worldChunkCoords.y));
		chunks[worldChunkCoords.x + 9][worldChunkCoords.y + 8]->rebuildMesh(neighbouringChunks2);
	}
	if (chunkPosition.x == 0 && worldChunkCoords.x != -8) {
		std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks2 = getNeighbouringChunks(glm::vec2(worldChunkCoords.x - 1, worldChunkCoords.y));
		chunks[worldChunkCoords.x + 7][worldChunkCoords.y + 8]->rebuildMesh(neighbouringChunks2);
	}
	if (chunkPosition.z == WIDTH - 1 && worldChunkCoords.y != 7) {
		std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks2 = getNeighbouringChunks(glm::vec2(worldChunkCoords.x, worldChunkCoords.y + 1));
		chunks[worldChunkCoords.x + 8][worldChunkCoords.y + 9]->rebuildMesh(neighbouringChunks2);
	}
	if (chunkPosition.z == 0 && worldChunkCoords.y != -8) {
		std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks2 = getNeighbouringChunks(glm::vec2(worldChunkCoords.x, worldChunkCoords.y - 1));
		chunks[worldChunkCoords.x + 8][worldChunkCoords.y + 7]->rebuildMesh(neighbouringChunks2);
	}
}

std::unordered_map<Face, std::shared_ptr<Chunk>> World::getNeighbouringChunks(glm::vec2 worldPosition) {
	std::unordered_map<Face, std::shared_ptr<Chunk>> neighbouringChunks;
	int x = worldPosition.x;
	int y = worldPosition.y;

	if (x != -8) neighbouringChunks[Face::Left] = chunks[x + 7][y + 8];
	if (x != 7) neighbouringChunks[Face::Right] = chunks[x + 9][y + 8];

	if (y != -8) neighbouringChunks[Face::Back] = chunks[x + 8][y + 7];
	if (y != 7) neighbouringChunks[Face::Front] = chunks[x + 8][y + 9];

	return neighbouringChunks;
}

void World::decorateWorld() {
	Perlin perlin(372498748);

	for (unsigned int i = 0; i < 80; i++) {
		unsigned int treeX = std::rand() % 16;
		unsigned int treeY = std::rand() % 16;
		int chunkX = (std::rand() % 16) - 8;
		int chunkY = (std::rand() % 16) - 8;

		float perlinX = (treeX) / ((float)WIDTH) + (float)chunkX;
		float perlinY = (treeY) / ((float)WIDTH) + (float)chunkY;
		int height = (int)(20 * perlin.getPerlin(perlinX, perlinY) + 64);

		glm::vec3 treePosition((int)treeX + (int)chunkX * (int)WIDTH - 2, height, (int)treeY + (int)chunkY * (int)WIDTH - 2);

		loadStructure(treePosition, tree);
	}
}

void World::loadStructure(glm::vec3 position, const Structure& structure) {
	glm::vec2 worldChunkCoords(floor(position.x / WIDTH), floor(position.z / WIDTH)); // position of chunk in world
	glm::vec3 dimensions(structure[0][0].size(), structure.size(), structure[0].size());  
	std::vector<glm::vec3> chunkPositions = { glm::vec3(mod((int)position.x, WIDTH), position.y, mod((int)position.z, WIDTH)) }; // position of origin corner of structure, relative to chunk
	std::vector<std::shared_ptr<Chunk>> chunks;
	
	try {
		chunks.push_back(this->chunks.at(worldChunkCoords.x + 8).at(worldChunkCoords.y + 8));
	}
	catch (const std::exception& e) {}

	if (chunkPositions[0].x + dimensions.x > WIDTH) {
		try {
			chunks.push_back(this->chunks.at(worldChunkCoords.x + 1 + 8).at(worldChunkCoords.y + 8));
			chunkPositions.push_back(glm::vec3(chunkPositions[0].x - WIDTH, chunkPositions[0].y, chunkPositions[0].z));
		}
		catch (const std::exception& e) {}
	}
	if (chunkPositions[0].z + dimensions.z > WIDTH) {
		try {
			chunks.push_back(this->chunks.at(worldChunkCoords.x + 8).at(worldChunkCoords.y + 1 + 8));
			chunkPositions.push_back(glm::vec3(chunkPositions[0].x, chunkPositions[0].y, chunkPositions[0].z - WIDTH));
		}
		catch (const std::exception& e) {}
	}
	if (chunkPositions[0].x + dimensions.x > WIDTH && chunkPositions[0].z + dimensions.z > WIDTH) {
		try {
			chunks.push_back(this->chunks.at(worldChunkCoords.x + 1 + 8).at(worldChunkCoords.y + 1 + 8));
			chunkPositions.push_back(glm::vec3(chunkPositions[0].x - WIDTH, chunkPositions[0].y, chunkPositions[0].z - WIDTH));
		}
		catch (const std::exception& e) {}
	}

	if (chunkPositions.size() > chunks.size())
		chunkPositions.erase(chunkPositions.begin());

	for (unsigned int i = 0; i < chunks.size(); i++)
		for (int y = 0; y < dimensions.y; y++)
			for (int z = 0; z < dimensions.z; z++)
				for (int x = 0; x < dimensions.x; x++) {
					std::shared_ptr<Chunk> chunk = chunks[i];
					glm::vec3& chunkPosition = chunkPositions[i];
					try {
						int& chunkBlock = (*chunk->getChunkData()).at(chunkPosition.x + x).at(chunkPosition.y + y).at(chunkPosition.z + z);

						if (chunkBlock == 0)
							chunkBlock = structure[y][x][z];
					}
					catch (const std::exception& e) {}
				}
}

int World::getBlockAtPosition(glm::vec3 position) {
	glm::vec3 intPosition(round(position.x), round(position.y), round(position.z));
	glm::vec2 worldChunkCoords(floor(intPosition.x / WIDTH), floor(intPosition.z / WIDTH));
	glm::vec3 chunkPosition(mod(intPosition.x, WIDTH), intPosition.y, mod(intPosition.z, WIDTH));

	int blockID = -1;
	try {
		blockID = (*chunks.at(worldChunkCoords.x + 8).at(worldChunkCoords.y + 8)->getChunkData()).at(chunkPosition.x).at(chunkPosition.y).at(chunkPosition.z);
	}
	catch (const std::exception& e) {}

	return blockID;
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