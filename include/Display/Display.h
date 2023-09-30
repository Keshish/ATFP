#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "stb/stb_image.h"

#include "Camera.h"
#include "Mesh.h"
#include "Projection.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Data.h"
#include "Actuator.h"

class Display {
 private:
    // window
    GLFWwindow* window{nullptr};
    const std::string windowName{"CodeLikeABosch"};

    // view
    std::unique_ptr<Camera> camera{};
    std::unique_ptr<Projection> projection = std::make_unique<Orthographic>();

    // shaders
    std::unique_ptr<Shader> colorShader{};
    std::unique_ptr<Shader> textureShader{};

    // meshes
    float rect_size = 0.03;
    std::unique_ptr<RectMesh> rect{};

    // colors
    glm::vec4 colorBlack{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 colorLightGray{0.3f, 0.3f, 0.3f, 1.0f};
    glm::vec4 colorWaterBlue{0.0f, 0.7f, 0.9f, 1.0f};
    glm::vec4 colorCyanBlue{rgbaToSingle(glm::vec4{56, 183, 190, 1})};
    std::vector<glm::vec4> objectColors{rgbaToSingle(glm::vec4{38, 70, 83, 1}),      // shit green
                                        rgbaToSingle(glm::vec4{244, 162, 97, 1}),    // orange
                                        rgbaToSingle(glm::vec4{231, 111, 81, 1}),    // bloody mary
                                        rgbaToSingle(glm::vec4{162, 210, 255, 1})};  // baby blue
    glm::vec4 colorGrassGreen{rgbaToSingle(glm::vec4{0.0f, 135.0f, 62.0f, 1.0f})};

    // textures
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures{};

    // map
    float scale{1.0f};

    // callback variables
    static inline bool isMouseBtnRightPressed{false};
    static inline double prevX{}, prevY{}, scrollOffsetY{};
    static inline bool scrollOffsetYChanged{false};
    static inline bool cameraPosChanged{false};

    Display() {
        glfwInit();
    }

    ~Display() {
        glfwTerminate();
    }

    std::mutex mtx{};

 public:
    explicit Display(const Display&) = delete;

    Display& operator=(const Display&) = delete;

    explicit Display(Display&&) = delete;

    Display& operator=(Display&&) = delete;

    static Display& display() {
        static Display instance;
        return instance;
    }

    void run() {
        std::lock_guard locker(mtx);
        init();

        auto data = &Data::data();
        size_t tick = 0;

        Actuator act;

        /// Render loop
        while (!glfwWindowShouldClose(window)) {
            int width{}, height{};
            glfwGetWindowSize(window, &width, &height);

            glClearColor(colorGrassGreen.x, colorGrassGreen.y, colorGrassGreen.z, colorGrassGreen.w);
            glClear(GL_COLOR_BUFFER_BIT);

            updateCameraPos();
            glm::mat4 view = camera->getViewMatrix();
            updateViewPos(&view);
            updateViewZoom(&view);

            colorShader->use();
            colorShader->setMat4("projection", projection->getProjection(width, height));
            colorShader->setMat4("view", view);

            textureShader->use();
            textureShader->setMat4("projection", projection->getProjection(width, height));
            textureShader->setMat4("view", view);

            /** WORK FROM HERE **/
            // make decision
            auto decision = act.run(tick);

            // render
            std::shared_ptr<Texture> texture{};
            glm::mat4 model{};

            texture = getTexture("ARROW");
            model = glm::rotate(glm::mat4(1.0f), float(data->yaws[tick]), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(data->speeds[tick] / 200.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(rect_size, 0.0f, 0.0f));
            textureShader->use();
            textureShader->setMat4("model", model);
            texture->bind();
            rect->render();

            auto angleVec1 = glm::vec4{1, 0, 0, 0};
            angleVec1 = angleVec1 * model;

            texture = getTexture("CAR");
            model = glm::rotate(glm::mat4(1.0f), float(data->yaws[tick]), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(4.0f, 2.0f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            textureShader->use();
            textureShader->setMat4("model", model);
            texture->bind();
            rect->render();

            // move rect
            size_t i = 0;
            for (const auto& object : data->objects[tick]) {
                if (object.x == 0 && object.y == 0)
                    continue;

                double length = sqrt(pow(object.vel_x, 2) + pow(object.vel_y, 2));
                double vel_dir = acos(object.vel_x / length);

                texture = getTexture("ARROW");
                model = glm::mat4(1.0f);
                model = glm::rotate(model, float(-data->yaws[tick]), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(float(object.x) / 5000.0f, float(object.y) / 5000.0f, 0.0f));
                model = glm::rotate(model, float(vel_dir), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, glm::vec3(float(length)/400.0f, 1.0f, 0.0f));
                model = glm::translate(model, glm::vec3(float(rect_size), 0.0f, 0.0f));
                textureShader->use();
                textureShader->setMat4("model", model);
                texture->bind();
                rect->render();

                std::cout << glm::to_string(model) << "\n";
//                auto angleVec2 = glm::vec4{1, 0, 0, 0};
//                angleVec2 = angleVec2 * model;
//                std::cout << glm::to_string(angleVec2) << "\n";

//                auto angle = acos(glm::dot(angleVec1, angleVec2) / (glm::length(angleVec1) * glm::length(angleVec2)));
//                std::cout << angle << "\n";

                model = glm::rotate(glm::mat4(1.0f), float(-data->yaws[tick]), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(float(object.x) / 5000.0f, float(object.y) / 5000.0f, 0.0f));
                colorShader->use();
                colorShader->setVec4("customColor", objectColors[i]);
                colorShader->setMat4("model", model);
                rect->render();
                i++;
            }

            std::this_thread::sleep_for(std::chrono::microseconds(20000));
            tick = (tick + 1) % data->timestamps.size();
            /** UNTIL HERE **/

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwDestroyWindow(window);
    }

 private:
    static inline void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
    }

    static inline void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        std::ignore = window;
        std::ignore = xoffset;

        if (yoffset != 0) {
            scrollOffsetY = yoffset;
            scrollOffsetYChanged = true;
        }
    }

    static inline void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        std::ignore = mods;

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            isMouseBtnRightPressed = true;
            glfwGetCursorPos(window, &prevX, &prevY);
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
            if (isMouseBtnRightPressed) {
                isMouseBtnRightPressed = false;
                cameraPosChanged = true;
            }
        }
    }

    static inline void cursorEnterCallback(GLFWwindow* window, int entered) {
        std::ignore = window;
        if (!entered) {
            if (isMouseBtnRightPressed) {
                isMouseBtnRightPressed = false;
                cameraPosChanged = false;
            }
        }
    }

    static inline glm::vec4 rgbaToSingle(glm::vec4 in) {
        return glm::vec4{in.x / 255.0f, in.y / 255.0f, in.z / 255.0f, in.w};
    }

    void init() {
        initWindow();
        initCallbacks();
        initShaders();
        initMeshes();
        initCamera();
        initTextures();
    }

    void initWindow() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (!primaryMonitor) {
            std::cout << "Could not get primary monitor.\n";
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_DECORATED, GL_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GL_TRUE);

        window = glfwCreateWindow(glfwGetVideoMode(primaryMonitor)->width,
                                  glfwGetVideoMode(primaryMonitor)->height,
                                  windowName.c_str(),
                                  nullptr,
                                  nullptr);
        if (!window) {
            std::cout << "Could not create window.\n";
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        glewInit();
    }

    void initCallbacks() {
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetScrollCallback(window, scrollCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetCursorEnterCallback(window, cursorEnterCallback);
    }

    void initShaders() {
        colorShader.reset();
        textureShader.reset();
        colorShader = std::make_unique<Shader>("../assets/shaders/color.vs", "../assets/shaders/color.fs");
        textureShader = std::make_unique<Shader>("../assets/shaders/texture.vs", "../assets/shaders/texture.fs");
    }

    void initMeshes() {
        rect.reset();

        // let the range of sensors be 50 m
        // 1 / 50 = 0.02 is 1 m
        rect = std::make_unique<RectMesh>(Point{rect_size, rect_size});  // set object size
    }

    void initCamera() {
        camera.reset();
        if (projection->type == ProjectionType::PERSPECTIVE) {
            camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, projection->zoomLowerLimit));
        } else if (projection->type == ProjectionType::ORTHO) {
            camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));
        }
    }

    void initTextures() {
        textures.clear();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    std::pair<double, double> getTranslations() {
        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        int width{}, height{};
        glfwGetWindowSize(window, &width, &height);

        double ratio = static_cast<double>(width) / static_cast<double>(height);
        double deltaX = ratio * 2 * (currX - prevX) / static_cast<double>(width);
        double deltaY = 2 * (prevY - currY) / static_cast<double>(height);
        return {deltaX, deltaY};
    }

    void updateViewPos(glm::mat4* view) {
        if (isMouseBtnRightPressed && glfwGetWindowAttrib(window, GLFW_HOVERED)) {
            auto [deltaX, deltaY] = getTranslations();
            *view = glm::translate(*view, glm::vec3(deltaX, deltaY, 0.0f));
        }
    }

    void updateCameraPos() {
        if (cameraPosChanged && glfwGetWindowAttrib(window, GLFW_HOVERED)) {
            auto [deltaX, deltaY] = getTranslations();
            camera->pos.x -= static_cast<float>(deltaX);
            camera->pos.y -= static_cast<float>(deltaY);
            cameraPosChanged = false;
        }

        if (scrollOffsetYChanged && glfwGetWindowAttrib(window, GLFW_HOVERED) &&
            projection->type == ProjectionType::PERSPECTIVE) {
            camera->pos.z -= static_cast<float>(scrollOffsetY);
            if (camera->pos.z < projection->zoomLowerLimit)
                camera->pos.z = projection->zoomLowerLimit;
            if (camera->pos.z > projection->zoomUpperLimit)
                camera->pos.z = projection->zoomUpperLimit;
            scrollOffsetYChanged = false;
        }
    }

    void updateViewZoom(glm::mat4* view) {
        if (scrollOffsetYChanged && glfwGetWindowAttrib(window, GLFW_HOVERED) &&
            projection->type == ProjectionType::ORTHO) {
            scale -= static_cast<float>(scrollOffsetY) / 10;
            if (scale < projection->zoomLowerLimit) {
                scale = projection->zoomLowerLimit;
            } else if (scale > projection->zoomUpperLimit) {
                scale = projection->zoomUpperLimit;
            }
            scrollOffsetYChanged = false;
        }
        *view = glm::scale(*view, glm::vec3(scale, scale, scale));
    }

    void loadTexture(const std::string& id) {
        if (id == "PALM") {
            textures[id] = {std::make_shared<Texture>("../assets/sprites/palm.png")};
        } else if (id == "ARROW") {
            textures[id] = {std::make_shared<Texture>("../assets/sprites/arrow.png")};
        } else if (id == "CAR") {
            textures[id] = {std::make_shared<Texture>("../assets/sprites/car.png")};
        } else if (id == "ROAD") {
            textures[id] = {std::make_shared<Texture>("../assets/sprites/road.jpg")};
        } else {
            std::cerr << "Invalid id " + id + "\n";
            exit(1);
        }
    }

    std::shared_ptr<Texture> getTexture(const std::string& id) {
        if (textures.find(id) == textures.end()) {
            loadTexture(id);
        }
        return textures.at(id);
    }
};
