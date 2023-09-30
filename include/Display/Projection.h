#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class ProjectionType {
    PERSPECTIVE, ORTHO
};

class Projection {
public:
    ProjectionType type{};
    const float zoomLowerLimit{};
    const float zoomUpperLimit{};

    explicit Projection(ProjectionType aType, float aZoomLowerLimit, float aZoomUpperLimit)
            : type{aType}, zoomLowerLimit{aZoomLowerLimit}, zoomUpperLimit{aZoomUpperLimit} {}

    virtual ~Projection() = default;

    [[nodiscard]] virtual glm::mat4 getProjection(int width, int height) const = 0;
};

class Perspective : public Projection {
public:
    // 0.0 < Z < 10.0 is ok (limits depends on the 3rd and 4th params of glm::perspective),
    // but to look good Z should be 2.0 < Z < 8.0
    Perspective()
            : Projection(ProjectionType::PERSPECTIVE, 2.0f, 8.0f) {}

    [[nodiscard]] glm::mat4 getProjection(int width, int height) const override {
        return glm::perspective(glm::radians(45.0f),
                                static_cast<float>(width) / static_cast<float>(height),
                                0.0f,
                                10.0f);
    }
};

class Orthographic : public Projection {
public:
    Orthographic()
            : Projection(ProjectionType::ORTHO, 0.05f, 3.0f) {}

    [[nodiscard]] glm::mat4 getProjection(int width, int height) const override {
        float ratio = static_cast<float>(width) / static_cast<float>(height);
        float newWidth = 2 * ratio;
        return glm::ortho(-(newWidth / 2), newWidth / 2, -1.0f, 1.0f, 0.0f, 10.0f);
    }
};
