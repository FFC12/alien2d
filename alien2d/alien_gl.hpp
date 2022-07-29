/*
MIT License

Copyright (c) 2022 Furkan Fatih Cetindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

- Introduction:

NOTE: Library is still WIP. So many features have not finished yet! But our
goals has explained in rest of introduction text.

This library has created for helping people to create their 2D games without any
dependencies. So, you do not need integrate a lot of dependencies/3rd party
libraries when you use this Alien framework. All neccessarities has already can
be included with header files. So Alien is a header-only library and it can be
extended so easily. There are a lot of different libraries which you can use for
your game projects as well. But Alien is focusing only 2d game development
rather than being general purpose API for graphics applications.

- Modules:

We have our own mathematics module and our own Vector and Matrix types which we
are using as a module. We have our own PNG/JPG parser but it also can be
extended for the different/new type of image files. Alien library also offers a
game-oriented memory allocator which also can be replaced by custom one. Context
creation and loading graphics api functions has all done in the library. So you
don't need to use any GLEW, GLAD or etc. Also for windows and linux systems,
window creation and mouse/keyboard inputs are handling by Alien. So you do not
need to use GLFW or SDL too. And many other features are in-progress and will be
added later as well.

- Contributors:

  FFC12 (author)

*/
#ifdef _WIN32
#pragma once
#elif
#ifndef ALIEN_GL_HPP
#define ALIEN_GL_HPP
#endif
#endif

#ifdef _WIN32
#ifndef ALIEN_DX11
#pragma comment(lib, "opengl32.lib")
#endif
#endif

#include <gl/GL.h>

#include "base.hpp"
#include "gl/glcorearb.h"
#include "gl/glext.h"
#include "gl/wglext.h"
#include "math.hpp"

namespace Extension::GL {
static inline HMODULE GLModule = 0;

// -- wglext
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNGLGETSTRINGPROC glGetString;
PFNGLGETERRORPROC glGetError;

// -- glcorearb
PFNGLCLEARPROC glClear;
PFNGLCLEARCOLORPROC glClearColor;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

static void *get_proc(const char *procName) {
  void *proc = (void *)wglGetProcAddress(procName);
  if (!proc) {
    if (!GLModule) {
      GLModule = LoadLibrary(TEXT("opengl32.dll"));
      if (!GLModule) std::cerr << "Could not found the opengl32.dll\n";
    }

    proc = (void *)GetProcAddress(GLModule, procName);
  }

  return proc;
}

// Load GL funcs
static void load_gl_funcs() {
  glClear = (PFNGLCLEARPROC)get_proc("glClear");
  glClearColor = (PFNGLCLEARCOLORPROC)get_proc("glClearColor");
  glGenBuffers = (PFNGLGENBUFFERSPROC)get_proc("glGenBuffers");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)get_proc("glGenVertexArrays");
}

// Load GL context funcs
static void load_gl_context_funcs() {
  wglGetExtensionsStringARB =
      (PFNWGLGETEXTENSIONSSTRINGARBPROC)get_proc("wglGetExtensionsStringARB");
  wglChoosePixelFormatARB =
      (PFNWGLCHOOSEPIXELFORMATARBPROC)get_proc("wglChoosePixelFormatARB");
  wglCreateContextAttribsARB =
      (PFNWGLCREATECONTEXTATTRIBSARBPROC)get_proc("wglCreateContextAttribsARB");
  glGetString = (PFNGLGETSTRINGPROC)get_proc("glGetString");
  glGetError = (PFNGLGETERRORPROC)get_proc("glGetError");
}

// Finish loading GL funcs.
static void load_gl_funcs_done() { FreeLibrary(GLModule); }
}  // namespace Extension::GL

namespace Alien {
class GLContext {
 public:
  GLContext() = default;
#ifdef _WIN32
  GLContext(HWND handle, HINSTANCE instance, HDC context)
      : m_DeviceContext(context),
        m_WindowHandle(handle),
        m_Instance(instance) {}

  void set_context(HWND handle, HINSTANCE instance, HDC context) {
    m_WindowHandle = handle;
    m_Instance = instance;
    m_DeviceContext = context;
  }

#endif
  bool create_context_gl() {
#ifdef _WIN32
    // Create pixel format descriptor
    PIXELFORMATDESCRIPTOR pfDesc;

    // Zero-ed the struct
    ZeroMemory(&pfDesc, sizeof(pfDesc));

    // Initialization of the pixel format descriptor
    pfDesc.nSize = sizeof(pfDesc);
    pfDesc.nVersion = 1;
    pfDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP |
                     PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED |
                     PFD_DOUBLEBUFFER | PFD_SWAP_LAYER_BUFFERS;
    pfDesc.iPixelType = PFD_TYPE_RGBA;
    pfDesc.cColorBits = 32;
    pfDesc.cRedBits = 8;
    pfDesc.cGreenBits = 8;
    pfDesc.cBlueBits = 8;
    pfDesc.cAlphaBits = 8;
    pfDesc.cDepthBits = 32;
    pfDesc.cStencilBits = 8;

    // Choose supported format which is similar what we asked for.
    i32 pFormat = ChoosePixelFormat(m_DeviceContext, &pfDesc);

    // Set the supported format
    SetPixelFormat(m_DeviceContext, pFormat, &pfDesc);
#endif

    Extension::GL::load_gl_funcs();
    Extension::GL::load_gl_funcs_done();

#ifdef _WIN32
    // Creating OpenGL rendering context
    HGLRC renderingContext = wglCreateContext(m_DeviceContext);
#endif

    // Make current context OpenGL rendering context for the m_Device
    wglMakeCurrent(m_DeviceContext, renderingContext);

    // Load gl extension function for the context creation
    Extension::GL::load_gl_context_funcs();

#ifdef _WIN32
    auto errorCode = Extension::GL::glGetError();
    if (errorCode == GL_INVALID_OPERATION) {
      MessageBoxA(m_WindowHandle, "GL error", "Context could not created",
                  MB_OK | MB_ICONQUESTION);
    }

    // Show the window
    ShowWindow(m_WindowHandle, SW_SHOW);
#endif

    return true;
  }

 private:
#ifdef _WIN32
  HWND m_WindowHandle{0};
  HINSTANCE m_Instance;
  HDC m_DeviceContext;
#endif
};
}  // namespace Alien

#ifndef _WIN32
#ifndef ALIEN_GL_HPP
#define ALIEN_GL_HPP
#endif
#endif