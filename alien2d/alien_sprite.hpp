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
    const std::string vertexSrc = R"(
/* vertex attributes go here to input to the vertex shader */
struct vs_in {
    float3 position_local : POS;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out {
    float4 position_clip : SV_POSITION; // required output of VS
};

vs_out vs_main(vs_in input) {
  vs_out output = (vs_out)0; // zero the memory first
  output.position_clip = float4(input.position_local, 1.0);
  return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
  return float4( input.position_clip ); // must return an RGBA colour
})";

    // vertex and fragment shaders are combined into only one sources.
    const std::string fragSrc = vertexSrc;

    Context->physicalDevice.compile_vertex_shader(vertexSrc, &m_VertShaderBlob,
                                                  &m_VertShader);
    Context->physicalDevice.compile_pixel_shader(fragSrc, &m_FragShaderBlob,
                                                 &m_FragShader);
    m_BufferDesc =
        std::move(Context->physicalDevice.create_quad_buffer(m_VertShaderBlob));
#else
    const std::string vertexSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aCol;

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
  vertexColor = vec4(aCol,1.0);
  gl_Position = vec4(aPos, 1.0f); // see how we directly give a vec3 to vec4's constructor
})";

    const std::string fragSrc = R"(
#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)

void main()
{
    FragColor = vertexColor;
})";

    m_VertexShaderSrc = vertexSrc;
    m_FragShaderSrc = fragSrc;

    Context->compile_vertex_shader(vertexSrc, m_VertexShaderSrc, m_VertShader);
    Context->compile_pixel_shader(fragSrc, m_FragShaderSrc, m_FragShader);
    Context->create_program(m_Program, m_VertShader, m_FragShader);

    m_BufferDesc = std::move(Context->create_quad_buffer(m_Program));
#endif
  }

  void on_draw() override {
#ifdef ALIEN_DX11
    Context->physicalDevice.next_frame();
    Context->physicalDevice.draw_command(m_BufferDesc.get(), m_VertShader,
                                         m_FragShader);
#else
    Context->next_frame();
    Context->draw_command(m_BufferDesc.get(), m_Program);
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
  std::string m_VertexShaderSrc;
  std::string m_FragShaderSrc;
  GLuint m_VertShader;
  GLuint m_FragShader;
  GLuint m_Program;

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