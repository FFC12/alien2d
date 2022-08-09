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

#ifdef _WIN32
//#include <wingdi.h>
#include <windows.h>
#endif
#include <gl/GL.h>

#include <fstream>

#include "base.hpp"
#include "common.hpp"
#include "gl/glcorearb.h"
#include "gl/glext.h"
#include "gl/wglext.h"
#include "math.hpp"

#ifndef ALIEN_DX11

#ifdef _DEBUG
#define CHECK(expr)                                           \
  do {                                                        \
    expr;                                                     \
    Extension::GL::check_gl_error(#expr, __FILE__, __LINE__); \
  } while (0)
#else
#define gl(expr) expr
#endif

namespace Extension::GL {
static inline HMODULE GLModule = nullptr;

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
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBINDBUFFERARBPROC glBindBuffer;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLTEXIMAGE2DPROC glTexImage2D;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLTEXPARAMETERIPROC glTexParameteri;
PFNGLTEXPARAMETERFPROC glTexParameterf;
PFNGLTEXPARAMETERIVPROC glTexParameteriv;
PFNGLTEXPARAMETERFVPROC glTexParameterfv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLBINDTEXTURESPROC glBindTextures;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;

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
  glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)get_proc("glDeleteBuffers");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)get_proc("glGenVertexArrays");
  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)get_proc("glBindVertexArray");
  glBufferData = (PFNGLBUFFERDATAPROC)get_proc("glBufferData");
  glBindBuffer = (PFNGLBINDBUFFERPROC)get_proc("glBindBuffer");
  glBufferSubData = (PFNGLBUFFERSUBDATAPROC)get_proc("glBufferSubData");
  glTexImage2D = (PFNGLTEXIMAGE2DPROC)get_proc("glTexImage2D");
  glActiveTexture = (PFNGLACTIVETEXTUREPROC)get_proc("glActiveTexture");
  glTexParameteri = (PFNGLTEXPARAMETERIPROC)get_proc("glTexParameteri");
  glTexParameterf = (PFNGLTEXPARAMETERFPROC)get_proc("glTexParameterf");
  glTexParameteriv = (PFNGLTEXPARAMETERIVPROC)get_proc("glTexParameteriv");
  glTexParameterfv = (PFNGLTEXPARAMETERFVPROC)get_proc("glTexParameterfv");
  glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)get_proc("glGenerateMipmap");
  glCreateShader = (PFNGLCREATESHADERPROC)get_proc("glCreateShader");
  glShaderSource = (PFNGLSHADERSOURCEPROC)get_proc("glShaderSource");
  glCompileShader = (PFNGLCOMPILESHADERPROC)get_proc("glCompileShader");
  glDeleteShader = (PFNGLDELETESHADERPROC)get_proc("glDeleteShader");
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)get_proc("glCreateProgram");
  glAttachShader = (PFNGLATTACHSHADERPROC)get_proc("glAttachShader");
  glLinkProgram = (PFNGLLINKPROGRAMPROC)get_proc("glLinkProgram");
  glUseProgram = (PFNGLUSEPROGRAMPROC)get_proc("glUseProgram");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)get_proc("glGetShaderiv");
  glGetShaderInfoLog =
      (PFNGLGETSHADERINFOLOGPROC)get_proc("glGetShaderInfoLog");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)get_proc("glGetProgramiv");
  glGetProgramInfoLog =
      (PFNGLGETPROGRAMINFOLOGPROC)get_proc("glGetProgramInfoLog");
  glDeleteProgram = (PFNGLDELETEPROGRAMPROC)get_proc("glDeleteProgram");
  glVertexAttribPointer =
      (PFNGLVERTEXATTRIBPOINTERPROC)get_proc("glVertexAttribPointer");
  glEnableVertexAttribArray =
      (PFNGLENABLEVERTEXATTRIBARRAYPROC)get_proc("glEnableVertexAttribArray");
  glDrawElementsInstanced =
      (PFNGLDRAWELEMENTSINSTANCEDPROC)get_proc("glDrawElementsInstanced");
  glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)get_proc("glBindFramebuffer");
  glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)get_proc("glBlitFramebuffer");
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

static void check_gl_error(const char *stmt, const char *fn, int l) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("GL error %08x, at %s:%i - for %s\n", err, fn, l, stmt);
    abort();
  }
}

}  // namespace Extension::GL

namespace Alien {
using namespace Extension::GL;
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

#ifdef _WIN32
    // Creating OpenGL rendering context
    HGLRC renderingContext = wglCreateContext(m_DeviceContext);
#endif

    // Make current context OpenGL rendering context for the m_Device
    wglMakeCurrent(m_DeviceContext, renderingContext);

    // Load gl extension function for the context creation
    Extension::GL::load_gl_context_funcs();
    Extension::GL::load_gl_funcs();
    Extension::GL::load_gl_funcs_done();

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

  // Read and compile GL vertex shader
  bool compile_vertex_shader(const char *path, std::string &vertShaderSrc,
                             GLuint &vertShader) {
    vertShader = Extension::GL::glCreateShader(GL_VERTEX_SHADER);

    std::string shaderBuffer;
    std::ifstream f(path);
    if (f.good()) {
      f.seekg(0, std::ios::end);
      auto size = f.tellg();
      shaderBuffer.resize(size);
      f.seekg(0);
      f.read(shaderBuffer.data(), shaderBuffer.size());
      f.close();
    } else {
      if (f.is_open()) f.close();
      return false;
    }

    auto src = (const GLchar *)shaderBuffer.c_str();
    CHECK(glShaderSource(vertShader, 1, &src, nullptr));

    GLint isCompiled = 0;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
    if (vertShader == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(vertShader, maxLength, &maxLength, &infoLog[0]);

      std::string log(infoLog.begin(), infoLog.end());
      std::cerr << log << std::endl;

      glDeleteShader(vertShader);
      return false;
    }

    vertShaderSrc = shaderBuffer;
    return true;
  }

  // Read and compile GL frag shader
  bool compile_pixel_shader(const char *path, std::string &fragShaderSrc,
                            GLuint &fragShader) {
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string shaderBuffer;
    std::ifstream f(path);
    if (f.good()) {
      f.seekg(0, std::ios::end);
      auto size = f.tellg();
      shaderBuffer.resize(size);
      f.seekg(0);
      f.read(shaderBuffer.data(), shaderBuffer.size());
      f.close();
    } else {
      if (f.is_open()) f.close();
      return false;
    }

    auto src = (const GLchar *)shaderBuffer.c_str();
    CHECK(glShaderSource(fragShader, 1, &src, nullptr));

    GLint isCompiled = 0;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if (fragShader == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(fragShader, maxLength, &maxLength, &infoLog[0]);

      std::string log(infoLog.begin(), infoLog.end());
      std::cerr << log << std::endl;

      glDeleteShader(fragShader);
      return false;
    }

    fragShaderSrc = shaderBuffer;
    return true;
  }

  // create program
  bool create_program(GLuint &program, GLuint vertShader, GLuint fragShader) {
    program = glCreateProgram();
    CHECK(glAttachShader(program, vertShader));
    CHECK(glAttachShader(program, fragShader));
    CHECK(glLinkProgram(program));

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (GLint *)&isLinked);
    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

      glDeleteProgram(program);
      glDeleteShader(vertShader);
      glDeleteShader(fragShader);

      return false;
    }

    return true;
  }

//  https://stackoverflow.com/questions/35414826/draw-opengl-renderbuffer-to-screen
  void resize_and_set_framebuffer(u32 w = 0, u32 h = 0) {

  }

  std::unique_ptr<Extra::BufferDescriptor> create_quad_buffer(GLuint program) {
    GLuint VBO, VAO, IBO;

    // Create vertex array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create vertex buffer
    glGenBuffers(1, &VBO);
    static GLfloat vertexData[] = {
        // x, y, r, g, b, a
        -0.5f, 0.5f,  0.f, 1.f, 0.f, 1.f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
        -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f, 0.5f, 0.5f,  1.f, 1.f, 0.f, 1.f};

    // Upload quad vertex data into server
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData,
                 GL_STATIC_DRAW);

    // Create index buffer
    glGenBuffers(1, &IBO);
    GLuint indexData[] = {0, 1, 2, 0, 3, 1};

    // Upload index data into server
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData,
                 GL_STATIC_DRAW);

    auto stride = 2;
    auto offset = 6;

    // Enable and select vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat),
                          nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat),
                          (GLvoid *)(3 * sizeof(GLfloat)));

    return std::move(std::make_unique<Extra::BufferDescriptor>(
        VAO, VBO, IBO, stride, offset,
        sizeof(vertexData) / stride * sizeof(GLfloat), ARRAYSIZE(indexData)));
  }

  void next_frame() {
    Extension::GL::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Extension::GL::glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  }

  void draw_command(Extra::BufferDescriptor *bufferDescriptor, GLuint program) {
    glUseProgram(program);
    glDrawElementsInstanced(GL_TRIANGLES, bufferDescriptor->indexCount,
                            GL_UNSIGNED_INT, (GLvoid *)0, 1);
  }

 private:
#ifdef _WIN32
  HWND m_WindowHandle{0};
  HINSTANCE m_Instance;
  HDC m_DeviceContext;
#endif
};
}  // namespace Alien
#endif

#ifndef _WIN32
#ifndef ALIEN_GL_HPP
#define ALIEN_GL_HPP
#endif
#endif