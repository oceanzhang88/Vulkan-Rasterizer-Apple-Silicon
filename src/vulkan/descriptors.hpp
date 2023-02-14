#pragma once

#include "device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace Ocean {

class DescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(OceanDevice &device) : device{device} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<DescriptorSetLayout> build() const;

   private:
    OceanDevice &device;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  DescriptorSetLayout(
          OceanDevice &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~DescriptorSetLayout();
  DescriptorSetLayout(const DescriptorSetLayout &) = delete;
  DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  OceanDevice &device;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class DescriptorWriter;
};

class OceanDescriptorPool {
 public:
  class Builder {
   public:
    Builder(OceanDevice &device) : device{device} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<OceanDescriptorPool> build() const;

   private:
    OceanDevice &device;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  OceanDescriptorPool(
      OceanDevice &device,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~OceanDescriptorPool();
  OceanDescriptorPool(const OceanDescriptorPool &) = delete;
  OceanDescriptorPool &operator=(const OceanDescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  OceanDevice &oceanDevice;
  VkDescriptorPool descriptorPool;

  friend class DescriptorWriter;
};

class DescriptorWriter {
 public:
  DescriptorWriter(DescriptorSetLayout &setLayout, OceanDescriptorPool &pool);

  DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  DescriptorSetLayout &setLayout;
  OceanDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace Ocean
