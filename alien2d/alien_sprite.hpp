/*
MIT License

Copyright(c) 2022 Furkan Fatih Cetindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifdef _WIN32
#pragma once
#elif
#ifndef ALIEN_HPP
#define ALIEN_HPP
#endif
#endif
#include "alien_gfx.hpp"
#include "common.hpp"

namespace Alien {
class Sprite : public Alien::IRenderable {
  friend struct Renderer;

 public:
  Sprite() {
    // If the context could not set, then
    // set the context
    if (!Context) Context = Alien::Renderer::GetContext();
  }

 private:
  void on_init() override {
#ifdef ALIEN_DX11
    Context->physicalDevice.compile_vertex_shader(
        L"shaders.hlsl", &m_VertShaderBlob, &m_VertShader);
    Context->physicalDevice.compile_pixel_shader(
        L"shaders.hlsl", &m_FragShaderBlob, &m_FragShader);
    m_BufferDesc =
        std::move(Context->physicalDevice.create_quad_buffer(m_VertShaderBlob));
#else
#endif
  }

  void on_draw() override {
#ifdef ALIEN_DX11
    Context->physicalDevice.next_frame();
    Context->physicalDevice.draw_command(m_BufferDesc.get(), m_VertShader,
                                         m_FragShader);
#else
#endif
  }

  void on_release() override{
#ifdef ALIEN_DX11

#else
#endif
  }

#ifdef ALIEN_DX11
  ID3DBlob *m_VertShaderBlob = nullptr;
  ID3DBlob *m_FragShaderBlob = nullptr;
  ID3D11VertexShader *m_VertShader = nullptr;
  ID3D11PixelShader *m_FragShader = nullptr;

  static inline Alien::DX11Context *Context{nullptr};
#else

  static inline Alien::GLContext* Context{nullptr};
#endif

  std::unique_ptr<Extra::BufferDescriptor> m_BufferDesc;
};
}  // namespace Alien

#ifndef _WIN32
#ifndef ALIEN_HPP
#define ALIEN_HPP
#endif
#endif