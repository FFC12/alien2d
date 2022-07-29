/*
MIT License

Copyright(c) 2022 Furkan Fatih Cetindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
                                                              copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions :

The above copyright noticeand this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
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
#ifdef ALIEN_DX11
#include "alien_dx11.hpp"
#else
#include "alien_gl.hpp"
#endif

#include "common.hpp"

namespace Alien {
struct IRenderable {
  IRenderable() = default;

  virtual void on_init() = 0;
  virtual void on_draw() = 0;
  virtual void on_release() = 0;
};

struct RenderQueueInfo {
  IRenderable* sprite;
  u32 priority;
  u32 zOrder;
};

struct Renderer {
  static Renderer& instance() {
    static Renderer ins;
    return ins;
  }

  ~Renderer() {
    for(auto &i: m_RenderQueue) {
      i.sprite->on_release();
    }
  }

  void init() {
    for(auto &i: m_RenderQueue) {
      i.sprite->on_init();
    }
  }

  void draw() {
    for(auto &i: m_RenderQueue) {
      i.sprite->on_draw();
    }
  }

#ifdef ALIEN_DX11
  void set_context(Alien::DX11Context* ctx) {
    Context = ctx;
  }
#else
  void set_context(Alien::GLContext* ctx) {
    m_Context = ctx;
  }
#endif

  void push_queue(RenderQueueInfo queueInfo) {
    m_RenderQueue.push_back(queueInfo);
  }

#ifndef ALIEN_DX11
  static Alien::GLContext* GetContext() { return Context; }
#else
  static Alien::DX11Context* GetContext() { return Context; }
#endif
 private:
  Renderer() = default;

#ifndef ALIEN_DX11
  static inline Alien::GLContext* Context {nullptr};
#else
  static inline Alien::DX11Context* Context {nullptr};
#endif

  std::vector<RenderQueueInfo> m_RenderQueue;
};


}  // namespace Alien

#ifndef _WIN32
#ifndef ALIEN_HPP
#define ALIEN_HPP
#endif
#endif