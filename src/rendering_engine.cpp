#define GLEW_STATIC

#include "rendering_engine.hpp"
#include "camera.hpp"

#include <memory>
#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include "model.hpp"
#include "entity.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "chunk.hpp"

RenderingEngine::RenderingEngine() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, settings);
    window->setMouseCursorVisible(false);

    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialise GLEW\n"; 
    }
    initialiseOpenGL();

    shader = std::make_shared<Shader>("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    chunkShader = std::make_shared<Shader>("shaders/chunk_vertex_shader.glsl", "shaders/chunk_fragment_shader.glsl");
    hudShader = std::make_shared<Shader>("shaders/hud_vertex_shader.glsl", "shaders/hud_fragment_shader.glsl");
    outlineShader = std::make_shared<Shader>("shaders/outline_vertex_shader.glsl", "shaders/outline_fragment_shader.glsl");

    camera = std::make_shared<Camera>(window, 1280.0f/720.0f);
    camera->setPosition(glm::vec3(0.0f, 70.0f, 0.0f));

    renderer = std::make_unique<Renderer>(shader, camera, 2);
    chunkRenderer = std::make_shared<ChunkRenderer>(chunkShader, camera);
    hudRenderer = std::make_shared<HUDRenderer>(hudShader, camera);
    outlineRenderer = std::make_shared<OutlineRenderer>(outlineShader, camera);
}

void RenderingEngine::initialiseOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);
    glViewport(0, 0, 1280, 720);
    glClearColor(0.729f, 0.918f, 1.0f, 1.0f);
}

bool RenderingEngine::isRunning() {
    return this->running;
}

void RenderingEngine::tick(float dt) {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            this->running = false;
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
            this->camera->setCursorLocked(false);
            this->window->setMouseCursorVisible(true);
        }
        if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
            this->camera->setCursorLocked(true);
            this->window->setMouseCursorVisible(false);
        }
        if (event.type == sf::Event::Resized) {
            sf::Vector2u size = window->getSize();
            this->camera->updateAspectRatio((float)size.x/(float)size.y);
            glViewport(0, 0, size.x, size.y);
        }
    }

    camera->tick(dt);
}

void RenderingEngine::render(World& world) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_BLEND);
    //glEnable(GL_DEPTH_TEST);

    renderer->begin();

    for (auto entity: entities)
        renderer->render(*entity);

    renderer->end();

    chunkRenderer->begin();

    world.render(chunkRenderer);

    chunkRenderer->end();

    //glDisable(GL_DEPTH_TEST);

    outlineRenderer->begin();

    outlineRenderer->render(outline);

    outlineRenderer->end();

    glEnable(GL_BLEND);
    hudRenderer->begin();

    for (auto hudElement : hudElements)
        hudRenderer->render(hudElement);

    hudRenderer->end();

    this->window->display();
}

std::shared_ptr<Camera> RenderingEngine::getCamera() {
    return camera;
}