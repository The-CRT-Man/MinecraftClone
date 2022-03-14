#include "renderer.hpp"
#include "chunk.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include "model.hpp"

void Renderer::begin() {
    shader->use();
    setUniforms();
}

void Renderer::end() {
    shader->unUse();
}

void Renderer::render(Entity& entity) {
    Model& model = entity.model;

    unsigned int texture = entity.texture;
    
    shader->setMatrix4fv("transform", entity.transform);

    glBindVertexArray(model.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    enableVertexAttribArrays();

    if (model.usesIndices)
        glDrawElements(GL_TRIANGLES, model.vertexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, model.vertexCount); 

    disableVertexAttribArrays();
}

void Renderer::enableVertexAttribArrays() {
    for (int i = 0; i < vbos; i++)
        glEnableVertexAttribArray(i);
}

void Renderer::disableVertexAttribArrays() {
    for (int i = 0; i < vbos; i++)
        glDisableVertexAttribArray(i);
}

void Renderer::setUniforms() {
    shader->setMatrix4fv("view", camera->getView());
    shader->setMatrix4fv("projection", camera->getProjection());
}

void ChunkRenderer::render(std::shared_ptr<Chunk> chunk) {
    auto models = chunk->getModels();
    unsigned int texture = chunk->getTexture();

    shader->setMatrix4fv("transform", chunk->getTransform());

    glActiveTexture(GL_TEXTURE0);

    for (unsigned int i = 0; i < models.size(); i++) {
        enableVertexAttribArrays();

        shader->setInt("face", i);

        glBindVertexArray(models[i]->vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElementsInstanced(GL_TRIANGLES, models[i]->vertexCount, GL_UNSIGNED_INT, 0, chunk->getNumberOfFaces(i));

        disableVertexAttribArrays();
    }
}

void ChunkRenderer::setUniforms() {
    shader->setMatrix4fv("view", camera->getView());
    shader->setMatrix4fv("projection", camera->getProjection());
    shader->setInt("textureAtlasRows", 256 / 16);
}

void HUDRenderer::render(std::shared_ptr<HUDElement> hudElement) {
    Model& model = hudElement->model;
    unsigned int texture = hudElement->texture;

    shader->setMatrix3fv("transform", hudElement->transform);

    glBindVertexArray(model.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    enableVertexAttribArrays();

    if (model.usesIndices)
        glDrawElements(GL_TRIANGLES, model.vertexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);

    disableVertexAttribArrays();

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void HUDRenderer::setUniforms() {
    
}