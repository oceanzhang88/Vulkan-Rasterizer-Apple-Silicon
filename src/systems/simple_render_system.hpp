#pragma once

#include "camera.hpp"
#include "vulkan/device.hpp"
#include "frame_info.hpp"
#include "game_object.hpp"
#include "vulkan/pipeline.hpp"
#include "vulkan/descriptors.hpp"

// std
#include <memory>
#include <vector>

namespace Ocean {
class SimpleRenderSystem {
 public:
  SimpleRenderSystem(
      OceanDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  OceanDevice &device;

  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;

  std::unique_ptr<DescriptorSetLayout> renderSystemLayout;
};
}  // namespace Ocean
