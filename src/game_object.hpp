#pragma once

#include "model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace Ocean {

struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
};

struct PointLightComponent {
  float lightIntensity = 1.0f;
};

class OceanGameObject {
 public:
  using id_t = unsigned int;
  using Map = std::unordered_map<id_t, OceanGameObject>;

  static OceanGameObject createGameObject() {
    static id_t currentId = 0;
    return OceanGameObject{currentId++};
  }

  static OceanGameObject makePointLight(
      float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

  OceanGameObject(const OceanGameObject &) = delete;
  OceanGameObject &operator=(const OceanGameObject &) = delete;
  OceanGameObject(OceanGameObject &&) = default;
  OceanGameObject &operator=(OceanGameObject &&) = default;

  id_t getId() { return id; }

  glm::vec3 color{};
  TransformComponent transform{};

  // Optional pointer components
  std::shared_ptr<Model> model{};
  std::unique_ptr<PointLightComponent> pointLight = nullptr;

 private:
  OceanGameObject(id_t objId) : id{objId} {}

  id_t id;
};
}  // namespace Ocean
