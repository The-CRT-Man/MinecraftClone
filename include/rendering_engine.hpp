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
    std::vector<std::shared_ptr<HUDElement>> hudElements;
    std::shared_ptr<Entity> outline;

    RenderingEngine();
    bool isRunning();

    void tick(float dt);
    void render(std::shared_ptr<World> world);

    std::shared_ptr<Camera> getCamera();

private:
    std::shared_ptr<sf::RenderWindow> window;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> chunkShader;
    std::shared_ptr<Shader> hudShader;
    std::shared_ptr<Shader> outlineShader;

    std::shared_ptr<Camera> camera;

    std::unique_ptr<Renderer> renderer;
    std::shared_ptr<ChunkRenderer> chunkRenderer;
    std::shared_ptr<HUDRenderer> hudRenderer;
    std::shared_ptr<OutlineRenderer> outlineRenderer;

    bool running = true;

    void initialiseOpenGL();
};

#endif