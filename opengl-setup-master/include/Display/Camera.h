#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

class Camera {
public:
    Camera() = default;
    explicit Camera(glm::vec3 aPos)
            : pos(aPos) {}
    ~Camera() = default;

    glm::vec3 pos{glm::vec3(0.0f, 0.0f, 1.0f)};
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 dir{0, 0, 1};
    glm::vec3 right{glm::normalize(glm::cross(worldUp, dir))};
    glm::vec3 up{glm::normalize(glm::cross(dir, right))};
    glm::vec3 front{-dir};

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return glm::lookAt(pos, pos + front, up);
    }
};
