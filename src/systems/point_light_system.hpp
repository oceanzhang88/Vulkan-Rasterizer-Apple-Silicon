#pragma once

#include "camera.hpp"
#include "vulkan/device.hpp"
#include "frame_info.hpp"
#include "game_object.hpp"
#include "vulkan/pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace Ocean {
    class PointLightSystem {
    public:
        PointLightSystem(
                Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~PointLightSystem();

        PointLightSystem(const PointLightSystem &) = delete;

        PointLightSystem &operator=(const PointLightSystem &) = delete;

        static void update(FrameInfo &frameInfo, GlobalUbo &ubo);

        void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void createPipeline(VkRenderPass renderPass);

        Device &oceanDevice;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout{};
    };
}  // namespace Ocean
