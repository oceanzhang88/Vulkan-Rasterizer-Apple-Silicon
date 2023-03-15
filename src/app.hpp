#pragma once

#include "vulkan/descriptors.hpp"
#include "vulkan/device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "window.hpp"

// std
#include <memory>
#include <vector>

namespace Ocean {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        static constexpr float PI = 3.1415926;

        App();
        ~App();
        App(const App &) = delete;
        App &operator=(const App &) = delete;
        void run();

    private:
        Window window;
        Device device;
        OceanRenderer renderer;
        // order of declarations matters
        std::unique_ptr<DescriptorPool> globalPool;
        std::vector<std::unique_ptr<DescriptorPool>> framePools;
        GameObjectManager gameObjectManager;

        void loadGameObjects();
    };
}  // namespace lve
