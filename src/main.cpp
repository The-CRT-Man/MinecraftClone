#define GLEW_STATIC

#include "main.hpp"

#include <iostream>
#include <memory>
#include <chrono>

#include <SFML/Graphics.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include "player.hpp"
#include "rendering_engine.hpp"
#include "shader.hpp"
#include "loader.hpp"
#include "model.hpp"
#include "entity.hpp"
#include "obj_loader.hpp"
#include "chunk.hpp"
#include "perlin.hpp"
#include "world.hpp"

std::vector<float> outlineCubeVertices = {
    -0.501f,  0.501f,  0.501f,
    -0.501f, -0.501f,  0.501f,
     0.501f, -0.501f,  0.501f,
     0.501f,  0.501f,  0.501f,
     0.501f,  0.501f, -0.501f,
     0.501f, -0.501f, -0.501f,
    -0.501f, -0.501f, -0.501f,
    -0.501f,  0.501f, -0.501f
};

std::vector<int> outlineCubeIndices = {
    0, 1, 2, 3, // front face
    4, 5, 6, 7, // back face
    0, 7, 6, 1, // left face
    2, 5, 4, 3, // right face
    1, 6, 5, 2, // bottom face
    0, 3, 4, 7  // top face

};

int main() {/*
    Perlin perlin;
    for (float x = -1.0f; x < 1.0f; x += 0.1f)
        std::cout << perlin.getPerlin(0, x) << "\n";*/

    //std::shared_ptr<Camera> camera;
    std::unique_ptr<RenderingEngine> renderingEngine(std::make_unique<RenderingEngine>());

    Loader loader;

    unsigned int texture = loader.loadTexture("res/texture_atlas.png", GL_NEAREST, GL_NEAREST);

    World world(loader, texture);

    unsigned int crosshairTexture = loader.loadTexture("res/crosshair.png", GL_NEAREST, GL_NEAREST);
    std::shared_ptr<HUDElement> crosshair = std::make_shared<HUDElement>(crosshairTexture, loader);
    crosshair->scale = glm::vec2(32.0f / 1280.0f, 32.0f / 720.0f);

    renderingEngine->hudElements.push_back(crosshair);

    sf::Clock clock;

    bool leftMouseButtonBeenPressed = false;
    bool rightMouseButtonBeenPressed = false;

    Model outlineModel = loader.generateTexuturelessModel(outlineCubeVertices, outlineCubeIndices);
    std::shared_ptr<Entity> outline = std::make_shared<Entity>(0, outlineModel);
    renderingEngine->outline = outline;

    while (renderingEngine->isRunning()) {
        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        renderingEngine->tick(dt);

        world.tick();
        glm::vec3 ray = renderingEngine->getCamera()->castCollisionRay(world, 10.0f, 0.05f);
        outline->position = ray;
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !leftMouseButtonBeenPressed) {
            
            if (ray.y != -1.0f)
                world.setBlock(ray, 0);
            leftMouseButtonBeenPressed = true;
            //std::cout << "(" << ray.x << ", " << ray.y << ", " << ray.z << ")\n";
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && leftMouseButtonBeenPressed) {
            leftMouseButtonBeenPressed = false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rightMouseButtonBeenPressed) {
            glm::vec3 ray = renderingEngine->getCamera()->castCollisionRaySurface(world, 10.0f, 0.05f);
            if (ray.y != -1.0f)
                world.setBlock(ray, 1);
            rightMouseButtonBeenPressed = true;
        }
        else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && rightMouseButtonBeenPressed) {
            rightMouseButtonBeenPressed = false;
        }

        crosshair->tick();
        outline->tick();

        renderingEngine->render(world);
    }

    return 0;
}