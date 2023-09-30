#pragma once

#include <GL/glew.h>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

struct Point {
    float x{}, y{};
};

struct Vertex {
    glm::vec3 pos{};
    glm::vec2 texture{};

    Vertex(float x, float y, float z) {
        pos = glm::vec3(x, y, z);
    }
};

class Mesh {
protected:
    GLuint VAO{}, VBO{};
    std::vector<Vertex> vertices{};

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                     &vertices[0],
                     GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, texture)));

        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

public:
    virtual void render() const = 0;
    virtual ~Mesh() {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
};

class RectMesh final : public Mesh {
public:
    explicit RectMesh(const Point size) {
        vertices.emplace_back(static_cast<float>(size.x), static_cast<float>(size.y), 0.0f);
        vertices.emplace_back(static_cast<float>(-size.x), static_cast<float>(size.y), 0.0f);
        vertices.emplace_back(static_cast<float>(-size.x), static_cast<float>(-size.y), 0.0f);
        vertices.emplace_back(static_cast<float>(size.x), static_cast<float>(-size.y), 0.0f);

        vertices[0].texture = glm::vec2(0.0f, 0.0f);
        vertices[1].texture = glm::vec2(1.0f, 0.0f);
        vertices[2].texture = glm::vec2(1.0f, 1.0f);
        vertices[3].texture = glm::vec2(0.0f, 1.0f);

        setupMesh();
    }

    void render() const override {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices.size()));
        glBindVertexArray(0);
    }
};

