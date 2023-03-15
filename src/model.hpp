#pragma once

#include "vulkan/buffer.hpp"
#include "vulkan/device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace Ocean {
    class Model {
    public:
        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position && color == other.color && normal == other.normal &&
                       uv == other.uv;
            }
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &filepath);
        };

        Model(OceanDevice &device, const Model::Builder &builder);

        ~Model();

        Model(const Model &) = delete;

        Model &operator=(const Model &) = delete;

        static std::unique_ptr<Model> createModelFromFile(
                OceanDevice &device, const std::string &filepath);

        void bind(VkCommandBuffer commandBuffer);

        void draw(VkCommandBuffer commandBuffer) const;

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        void createIndexBuffers(const std::vector<uint32_t> &indices);

        OceanDevice &device;

        std::unique_ptr<OceanBuffer> vertexBuffer;
        uint32_t vertexCount{};

        bool hasIndexBuffer = false;
        std::unique_ptr<OceanBuffer> indexBuffer;
        uint32_t indexCount{};
    };
}  // namespace Ocean
