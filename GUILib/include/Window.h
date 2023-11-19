
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>

namespace GUI {
class Window {
public:
  Window();
  ~Window();

  void BeginDraw();
  void EndDraw();

  bool ShouldClose();

private:
  GLFWwindow *mWindow = nullptr;
};
} // namespace GUI
