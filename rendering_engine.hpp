#ifndef RENDERING_ENGINE_HPP
#define RENDERING_ENGINE_HPP

#include "model.hpp"
#include "entity.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "chunk.hpp"
#include "world.hpp"

#include <memory>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>

class RenderingEngine {
public:
    std::vector<std::shared_ptr<Entity>> entities;

    RenderingEngine();
    bool isRunning();

    void tick(float dt);
    void render(World& world);

private:
    std::shared_ptr<sf::RenderWindow> window;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> chunkShader;

    std::shared_ptr<Camera> camera;

    std::unique_ptr<Renderer> renderer;
    std::shared_ptr<ChunkRenderer> chunkRenderer;

    bool running = true;

    void initialiseOpenGL();
};

#endif