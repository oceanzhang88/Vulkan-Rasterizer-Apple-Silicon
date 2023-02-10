#pragma once

#include "descriptors.hpp"
#include "device.hpp"
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

  App();
  ~App();

  App(const App &) = delete;
  App &operator=(const App &) = delete;

  void run();

 private:
  void loadGameObjects();

  OceanWindow window{WIDTH, HEIGHT, "Vulkan MacOS M1"};
  OceanDevice device{window};
  OceanRenderer renderer{window, device};

  // note: order of declarations matters
  std::unique_ptr<OceanDescriptorPool> globalPool{};
  OceanGameObject::Map gameObjects;
};
}  // namespace Ocean
