#pragma once

#include <memory>

#include "shader.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "chunk.hpp"

class Renderer {
public:
    Renderer(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera, int vbos)
        : shader(shader), camera(camera), vbos(vbos) {}

    virtual void render(Entity& entity);

    virtual void begin();
    virtual void end();

protected:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Camera> camera;
    int vbos;

    void enableVertexAttribArrays();
    void disableVertexAttribArrays();

    virtual void setUniforms();
};

class ChunkRenderer : public Renderer {
public:
    ChunkRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera)
        : Renderer(shader, camera, 5) {}

    void render(std::shared_ptr<Chunk> chunk);
    void begin() override;
    void end() override;

private:
    void setUniforms() override;
};