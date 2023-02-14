#include "app.hpp"

#include "vulkan/buffer.hpp"
#include "camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace Ocean {

App::App() {
  globalPool =
      OceanDescriptorPool::Builder(device)
          .setMaxSets(OceanSwapChain::MAX_FRAMES_IN_FLIGHT)
          .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, OceanSwapChain::MAX_FRAMES_IN_FLIGHT)
          .build();
  loadGameObjects();
}

App::~App() {}

void App::run() {
  std::vector<std::unique_ptr<OceanBuffer>> uboBuffers(OceanSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<OceanBuffer>(
            device,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffers[i]->map();
  }

  auto globalSetLayout =
      DescriptorSetLayout::Builder(device)
          .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
          .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(OceanSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    DescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{
          device,
          renderer.getSwapChainRenderPass(),
          globalSetLayout->getDescriptorSetLayout()};
  PointLightSystem pointLightSystem{
          device,
          renderer.getSwapChainRenderPass(),
          globalSetLayout->getDescriptorSetLayout()};
  Camera camera{};

  auto viewerObject = OceanGameObject::createGameObject();
  viewerObject.transform.translation.z = -2.5f;
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!window.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = renderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

    if (auto commandBuffer = renderer.beginFrame()) {
      int frameIndex = renderer.getFrameIndex();
      FrameInfo frameInfo{
          frameIndex,
          frameTime,
          commandBuffer,
          camera,
          globalDescriptorSets[frameIndex],
          gameObjects};

      // update
      GlobalUbo ubo{};
      ubo.projection = camera.getProjection();
      ubo.view = camera.getView();
      ubo.inverseView = camera.getInverseView();
      pointLightSystem.update(frameInfo, ubo);
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      renderer.beginSwapChainRenderPass(commandBuffer);

      // order here matters
      simpleRenderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);

      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}

void App::loadGameObjects() {
  std::shared_ptr<Model> model =
      Model::createModelFromFile(device, "models/bunny.obj");
  auto bunny = OceanGameObject::createGameObject();
    bunny.model = model;
    bunny.transform.translation = {-.5f, .5f, 0.f};
    bunny.transform.scale = {.5f, .5f, .5f};
    // Pi = atan(1)*4
    bunny.transform.rotation = {0.0f, atan(1)*4,  atan(1)*4};
  gameObjects.emplace(bunny.getId(), std::move(bunny));

  model = Model::createModelFromFile(device, "models/dragon.obj");
  auto dragon = OceanGameObject::createGameObject();
  dragon.model = model;
  dragon.transform.translation = {.5f, .2f, 0.f};
  dragon.transform.scale = {1.f, 1.f, 1.f};
  dragon.transform.rotation = {PI, -PI/2, 0.0f};
  gameObjects.emplace(dragon.getId(), std::move(dragon));

  model = Model::createModelFromFile(device, "models/quad.obj");
  auto floor = OceanGameObject::createGameObject();
  floor.model = model;
  floor.transform.translation = {0.f, .5f, 0.f};
  floor.transform.scale = {6.f, 1.f, 6.f};
  gameObjects.emplace(floor.getId(), std::move(floor));

  std::vector<glm::vec3> lightColors{
      {2.f, .2f, .2f},
//      {.2f, .2f, 2.f},
//      {.2f, 2.f, .2f},
      {2.f, 2.f, .2f},
      {.2f, 2.f, 2.f},
      {2.f, 2.f, 2.f}  //
  };

  for (int i = 0; i < lightColors.size(); i++) {
    auto pointLight = OceanGameObject::makePointLight(0.5f);
    pointLight.color = lightColors[i];
    auto rotateLight = glm::rotate(
        glm::mat4(1.f),
        (i * glm::two_pi<float>()) / lightColors.size(),
        {0.f, -1.f, 0.f});
    pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
    gameObjects.emplace(pointLight.getId(), std::move(pointLight));
  }
}

}  // namespace Ocean
