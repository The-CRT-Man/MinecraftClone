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

    unsigned int texture = loader.loadTexture("res/texture_atlas.png", GL_NEAREST, GL_NEAREST);

    World world(loader, texture);
    //world.finishMesh();

    unsigned int crosshairTexture = loader.loadTexture("res/crosshair.png", GL_NEAREST, GL_NEAREST);
    std::shared_ptr<HUDElement> crosshair = std::make_shared<HUDElement>(crosshairTexture, loader);
    crosshair->scale = glm::vec2(32.0f / 1280.0f, 32.0f / 720.0f);

    renderingEngine->hudElements.push_back(crosshair);

    sf::Clock clock;

    while (renderingEngine->isRunning()) {
        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        renderingEngine->tick(dt);

        world.tick();
        crosshair->tick();

        renderingEngine->render(world);
    }

    return 0;
}