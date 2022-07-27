#include <alien_window.hpp>
#include <iostream>

int main() {
  Alien::App app("Alien Test");

  // DirectX 11 - Usage Example (Those will be wrapped by Renderer)
  ID3DBlob *vertShaderBlob, *fragShaderBlob;
  ID3D11VertexShader *vertShader;
  ID3D11PixelShader *fragShader;
  std::unique_ptr<Extra::BufferDescriptor> bufferDesc;
  app.add_event_queue(
      [&](Alien::AppState &state) {
        app.m_Context.physicalDevice.compile_vertex_shader(
            L"shaders.hlsl", &vertShaderBlob, &vertShader);
        app.m_Context.physicalDevice.compile_pixel_shader(
            L"shaders.hlsl", &fragShaderBlob, &fragShader);
        bufferDesc = std::move(
            app.m_Context.physicalDevice.create_buffer(vertShaderBlob));
      },
      Alien::Queue::e_Init);

  app.add_event_queue(
      [&](Alien::AppState &state) {
        app.m_Context.physicalDevice.next_frame();
        app.m_Context.physicalDevice.draw_command(bufferDesc.get(), vertShader,
                                                  fragShader);
      },
      Alien::Queue::e_Update);

  app.update();
  return 0;
}
