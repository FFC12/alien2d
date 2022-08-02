#define ALIEN_DX11
#include <alien_sprite.hpp>
#include <alien_window.hpp>

int main() {
  Alien::App app("Alien Test",800,600);
  auto ctx = app.get_context();

  Alien::Renderer renderer = Alien::Renderer::instance();
  renderer.set_context(&ctx);

  Alien::Sprite sprite;
  renderer.push_queue(Alien::RenderQueueInfo{
      .sprite = dynamic_cast<Alien::IRenderable *>(&sprite),
      .priority = 0,
      .zOrder = 0});

  app.add_resize_event([&](auto s) {
    auto sc = (Alien::EventResized *)s.get();
    auto h = sc->height;
    auto w = sc->width;
    std::cout << w << " : " << h << std::endl;
  });

  app.add_event_queue(
      [&](Alien::AppState &state) {
        renderer.init();
      },
      Alien::Queue::e_Init);

  app.add_event_queue(
      [&](Alien::AppState &state) {
        renderer.draw();
      },
      Alien::Queue::e_Update);

  app.update();
  return 0;
}
