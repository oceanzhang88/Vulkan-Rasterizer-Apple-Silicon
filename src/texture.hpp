#pragma once

#include "vulkan/device.hpp"

// libs
#include <vulkan/vulkan.h>

// std
#include <memory>
#include <string>

namespace Ocean {
    class Texture {
    public:
        Texture(Device &device, const std::string &textureFilepath);

        Texture(Device &device,
                VkFormat format,
                VkExtent3D extent,
                VkImageUsageFlags usage,
                VkSampleCountFlagBits sampleCount);

        ~Texture();

        // delete copy constructors
        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        [[nodiscard]] VkImageView imageView() const { return mTextureImageView; }

        [[nodiscard]] VkSampler sampler() const { return mTextureSampler; }

        [[nodiscard]] VkImage getImage() const { return mTextureImage; }

        [[nodiscard]] VkImageView getImageView() const { return mTextureImageView; }

        [[nodiscard]] VkDescriptorImageInfo getImageInfo() const { return mDescriptor; }

        [[nodiscard]] VkImageLayout getImageLayout() const { return mTextureLayout; }

        [[nodiscard]] VkExtent3D getExtent() const { return mExtent; }

        [[nodiscard]] VkFormat getFormat() const { return mFormat; }

        void updateDescriptor();

        void transitionLayout(
                VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);

        static std::unique_ptr<Texture> createTextureFromFile(
                Device &device, const std::string &filepath);

    private:
        void createTextureImage(const std::string &filepath);

        void createTextureImageView(VkImageViewType viewType);

        void createTextureSampler();

        VkDescriptorImageInfo mDescriptor{};

        Device &mDevice;
        VkImage mTextureImage = nullptr;
        VkDeviceMemory mTextureImageMemory = nullptr;
        VkImageView mTextureImageView = nullptr;
        VkSampler mTextureSampler = nullptr;
        VkFormat mFormat;
        VkImageLayout mTextureLayout;
        uint32_t mMipLevels{1};
        uint32_t mLayerCount{1};
        VkExtent3D mExtent{};
    };

}  // namespace lve
