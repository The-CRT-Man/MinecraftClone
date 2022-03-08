#define GLEW_STATIC

#include "main.hpp"

#include <iostream>
#include <memory>
#include <chrono>

#include <SFML/Graphics.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include "rendering_engine.hpp"
#include "shader.hpp"
#include "loader.hpp"
#include "model.hpp"
#include "entity.hpp"
#include "obj_loader.hpp"
#include "chunk.hpp"
#include "perlin.hpp"
#include "world.hpp"

int main() {/*
    Perlin perlin;
    for (float x = -1.0f; x < 1.0f; x += 0.1f)
        std::cout << perlin.getPerlin(0, x) << "\n";*/
    
    std::unique_ptr<RenderingEngine> renderingEngine(std::make_unique<RenderingEngine>());

    Loader loader;

    std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    std::vector<float> textureCoords = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };
    std::vector<float> normals = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    std::vector<int> indices = {  // note that we start from 0!
        3, 1, 0,   // first triangle
        3, 2, 1    // second triangle
    };

    Model model = loader.generateModel(vertices, textureCoords, normals, indices);
    unsigned int texture = loader.loadTexture("res/texture_atlas.png", GL_NEAREST, GL_NEAREST);

    World world(loader, texture);
    //world.finishMesh();

    sf::Clock clock;

    while (renderingEngine->isRunning()) {
        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        renderingEngine->tick(dt);

        world.tick();

        renderingEngine->render(world);
    }

    return 0;
}