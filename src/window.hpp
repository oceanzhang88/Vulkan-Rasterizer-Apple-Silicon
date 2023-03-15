#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <string>

namespace Ocean {

    class Window {
    public:
        Window(int w, int h, std::string name);

        ~Window();

        Window(const Window &) = delete;

        Window &operator=(const Window &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }

        VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        bool wasWindowResized() const { return framebufferResized; }

        void resetWindowResizedFlag() { framebufferResized = false; }

        [[nodiscard]] GLFWwindow *getGLFWwindow() const { return window; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        void initWindow();

        int width;
        int height;
        bool framebufferResized = false;

        std::string windowName;
        GLFWwindow *window{};
    };
}  // namespace Ocean
