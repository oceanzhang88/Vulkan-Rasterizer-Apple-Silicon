#pragma once

#include "device.hpp"

namespace Ocean {

    class OceanBuffer {
    public:
        OceanBuffer(
                OceanDevice &device,
                VkDeviceSize instanceSize,
                uint32_t instanceCount,
                VkBufferUsageFlags usageFlags,
                VkMemoryPropertyFlags memoryPropertyFlags,
                VkDeviceSize minOffsetAlignment = 1);

        ~OceanBuffer();

        OceanBuffer(const OceanBuffer &) = delete;

        OceanBuffer &operator=(const OceanBuffer &) = delete;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void unmap();

        void writeToBuffer(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void writeToIndex(void *data, int index);

        VkResult flushIndex(int index);

        VkDescriptorBufferInfo descriptorInfoForIndex(int index);

        VkResult invalidateIndex(int index);

        [[nodiscard]] VkBuffer getBuffer() const { return buffer; }

        [[nodiscard]] void *getMappedMemory() const { return mapped; }

        [[nodiscard]] uint32_t getInstanceCount() const { return instanceCount; }

        [[nodiscard]] VkDeviceSize getInstanceSize() const { return instanceSize; }

        [[nodiscard]] VkDeviceSize getAlignmentSize() const { return instanceSize; }

        [[nodiscard]] VkBufferUsageFlags getUsageFlags() const { return usageFlags; }

        [[nodiscard]] VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }

        [[nodiscard]] VkDeviceSize getBufferSize() const { return bufferSize; }

    private:
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        OceanDevice &device;
        void *mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };

}  // namespace Ocean
