#include "window.hpp"

// std
#include <stdexcept>

namespace Ocean {

OceanWindow::OceanWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
  initWindow();
}

OceanWindow::~OceanWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void OceanWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void OceanWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to craete window surface");
  }
}

void OceanWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto pwindow = reinterpret_cast<OceanWindow *>(glfwGetWindowUserPointer(window));
    pwindow->framebufferResized = true;
    pwindow->width = width;
    pwindow->height = height;
}

}  // namespace Ocean
