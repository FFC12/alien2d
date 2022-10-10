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
#ifndef ALIEN_DX11_HPP
#define ALIEN_DX11_HPP
#endif
#endif

#ifdef _WIN32
#ifdef ALIEN_DX11
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#endif
#endif

#ifdef _WIN32
#include <Windows.h>
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <wingdi.h>
#endif

#include "base.hpp"
#include "common.hpp"
#include "math.hpp"

#ifdef ALIEN_DX11
namespace Extension::DX11 {
class App;
struct DX11PhysicalDevice {
  friend class App;
  explicit DX11PhysicalDevice(u32 w = 800, u32 h = 600)
      : m_Width(w), m_Height(h) {}

  void destroy_device() {
    m_Device->Release();
    m_SwapChain->Release();
    m_DeviceContext->Release();
    m_RenderTargetView->Release();
  }

  // Initialize m_Device (retrieve windows handle and instance)
  void init_device(HWND handle, HINSTANCE instance) {
    m_Instance = instance;
    m_WindowHandle = handle;
    m_IsInitialized = true;
  }

  // Initialize DirectX 11 context: buffer descriptor, swap chain
  // and render target.
  bool init_context_dx11(bool isDepthAndStencilEnabled,
                         bool didEnabledDebugLayer = true) {
    assert(m_IsInitialized &&
           "Device must be initialized! Probably you called the "
           "init_context_dx11() function before the init_device() function!");

    HRESULT hr;

    // Initialize buffer descriptor
    DXGI_MODE_DESC bufferDesc = {0};

    bufferDesc.Width = m_Width;
    bufferDesc.Height = m_Height;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Initialize swapchain descriptor
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};

    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = m_WindowHandle;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    u32 deviceFlags = 0;
    if (didEnabledDebugLayer) deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

    // Create m_Device instance and swapchain
    hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, NULL, NULL,
        D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL,
        &m_DeviceContext);

    if (FAILED(hr)) {
      return false;
    }

    if (didEnabledDebugLayer) {
      // Get the debug layer object
      ID3D11Debug* debug = nullptr;
      m_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);

      // If we got the debug layer object
      if (debug) {
        ID3D11InfoQueue* debugInfoQueue = nullptr;
        if (SUCCEEDED(debug->QueryInterface(__uuidof(ID3D11InfoQueue),
                                            (void**)&debugInfoQueue))) {
          debugInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION,
                                             true);
          debugInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR,
                                             true);
          debugInfoQueue->Release();
        }
        debug->Release();
      }
    }

    // Prepare back buffer for double buffering (DX11)
    ID3D11Texture2D* backBuffer;
    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                (void**)&backBuffer);
    assert(SUCCEEDED(hr));

    // Create render target view for the texture
    hr =
        m_Device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
    assert(SUCCEEDED(hr));
    backBuffer->Release();

    if (isDepthAndStencilEnabled) {
      init_depth_stencil_view();
    }

    m_IsDepthOrStencilBufferEnable = isDepthAndStencilEnabled;

    // Set m_Device context with render target views which are basically a
    // textures of render target view and depth and stencil view
    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView,
                                        m_DepthStencilView);

    return true;
  }

  // Creating texture from depth and stencil view for the render targets.
  void init_depth_stencil_view() {
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc = {0};
    depthStencilDesc.Width = m_Width;
    depthStencilDesc.Height = m_Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    m_Device->CreateTexture2D(&depthStencilDesc, NULL, &m_DepthStencilBuffer);
    m_Device->CreateDepthStencilView(m_DepthStencilBuffer, NULL,
                                     &m_DepthStencilView);
  }

  // Compile vertex shader from the source
  bool compile_vertex_shader(const std::string& src, ID3DBlob** vertShaderBlob,
                             ID3D11VertexShader** vertShader) const {
    // Create Vertex Shader
    ID3DBlob* vsBlob;
    ID3D11VertexShader* vertexShader;
    {
      ID3DBlob* shaderCompileErrorsBlob;
      HRESULT hResult = D3DCompile(src.c_str(), src.length(), nullptr, nullptr,
                                   nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob,
                                   &shaderCompileErrorsBlob);

      if (FAILED(hResult)) {
        const char* errorString = NULL;
        if (shaderCompileErrorsBlob) {
          errorString =
              (const char*)shaderCompileErrorsBlob->GetBufferPointer();
          shaderCompileErrorsBlob->Release();
        } else {
          errorString = "Unknown";
        }
        MessageBoxA(0, errorString, "Shader Compiler Error",
                    MB_ICONERROR | MB_OK);
        return false;
      }

      hResult = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(),
                                             vsBlob->GetBufferSize(), nullptr,
                                             &vertexShader);
      assert(SUCCEEDED(hResult));
    }

    *vertShaderBlob = vsBlob;
    *vertShader = vertexShader;

    return true;
  }

  // Compile pixel shader from the source
  bool compile_pixel_shader(const std::string& src, ID3DBlob** pixelShaderBlob,
                            ID3D11PixelShader** pixShader) const {
    // Create Pixel Shader
    ID3DBlob* psBlob;
    ID3D11PixelShader* pixelShader;
    {
      ID3DBlob* shaderCompileErrorsBlob;
      HRESULT hResult = D3DCompile(src.c_str(), src.length(), nullptr, nullptr,
                                   nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob,
                                   &shaderCompileErrorsBlob);

      if (FAILED(hResult)) {
        const char* errorString = NULL;
        if (shaderCompileErrorsBlob) {
          errorString =
              (const char*)shaderCompileErrorsBlob->GetBufferPointer();
          shaderCompileErrorsBlob->Release();
        } else {
          errorString = "Unknown";
        }
        MessageBoxA(0, errorString, "Shader Compiler Error",
                    MB_ICONERROR | MB_OK);
        return false;
      }

      hResult = m_Device->CreatePixelShader(psBlob->GetBufferPointer(),
                                            psBlob->GetBufferSize(), nullptr,
                                            &pixelShader);
      assert(SUCCEEDED(hResult));
    }

    *pixelShaderBlob = psBlob;
    *pixShader = pixelShader;

    return true;
  }

  // Compile vertex shader from the path
  bool compile_vertex_shader(LPCWSTR path, ID3DBlob** vertShaderBlob,
                             ID3D11VertexShader** vertShader) const {
    // Create Vertex Shader
    ID3DBlob* vsBlob;
    ID3D11VertexShader* vertexShader;
    {
      ID3DBlob* shaderCompileErrorsBlob;
      HRESULT hResult =
          D3DCompileFromFile(path, nullptr, nullptr, "vs_main", "vs_5_0", 0, 0,
                             &vsBlob, &shaderCompileErrorsBlob);
      if (FAILED(hResult)) {
        const char* errorString = NULL;
        if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
          errorString = "Could not compile shader; file not found";
        else if (shaderCompileErrorsBlob) {
          errorString =
              (const char*)shaderCompileErrorsBlob->GetBufferPointer();
          shaderCompileErrorsBlob->Release();
        }
        MessageBoxA(0, errorString, "Shader Compiler Error",
                    MB_ICONERROR | MB_OK);
        return false;
      }

      hResult = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(),
                                             vsBlob->GetBufferSize(), nullptr,
                                             &vertexShader);
      assert(SUCCEEDED(hResult));
    }

    *vertShaderBlob = vsBlob;
    *vertShader = vertexShader;

    return true;
  }

  // Compile pixel shader from the path
  bool compile_pixel_shader(LPCWSTR path, ID3DBlob** pixelShaderBlob,
                            ID3D11PixelShader** pixShader) const {
    // Create Pixel Shader
    ID3DBlob* psBlob;
    ID3D11PixelShader* pixelShader;
    {
      ID3DBlob* shaderCompileErrorsBlob;
      HRESULT hResult =
          D3DCompileFromFile(path, nullptr, nullptr, "ps_main", "ps_5_0", 0, 0,
                             &psBlob, &shaderCompileErrorsBlob);
      if (FAILED(hResult)) {
        const char* errorString = NULL;
        if (hResult == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
          errorString = "Could not compile shader; file not found";
        else if (shaderCompileErrorsBlob) {
          errorString =
              (const char*)shaderCompileErrorsBlob->GetBufferPointer();
          shaderCompileErrorsBlob->Release();
        }
        MessageBoxA(0, errorString, "Shader Compiler Error",
                    MB_ICONERROR | MB_OK);
        return false;
      }

      hResult = m_Device->CreatePixelShader(psBlob->GetBufferPointer(),
                                            psBlob->GetBufferSize(), nullptr,
                                            &pixelShader);
      assert(SUCCEEDED(hResult));
      psBlob->Release();
    }

    *pixelShaderBlob = psBlob;
    *pixShader = pixelShader;

    return true;
  }

  void set_viewport(u32 w, u32 h) {
    D3D11_VIEWPORT vp = {.Width = (float)w, .Height = (float)h};
    m_DeviceContext->RSSetViewports(1, &vp);
  }

  // When buffers resized (when window resize) this will help
  // us to resize back buffers (render target), handling the window resizing
  // let to the user thru Resized Event.
  void resize_and_set_framebuffer(u32 w = 0, u32 h = 0) {
    m_DeviceContext->OMSetRenderTargets(0, 0, 0);
    m_RenderTargetView->Release();

    // Zero-ed parameters helps us that being used client area by swap chain
    // automatically
    HRESULT hResult =
        m_SwapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
    assert(SUCCEEDED(hResult));

    ID3D11Texture2D* newFrameBuffer;

    hResult = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                     (void**)&newFrameBuffer);
    assert(SUCCEEDED(hResult));

    hResult = m_Device->CreateRenderTargetView(newFrameBuffer, NULL,
                                               &m_RenderTargetView);

    assert(SUCCEEDED(hResult));
    newFrameBuffer->Release();
  }

  // Create buffer (position & color)
  std::unique_ptr<Extra::BufferDescriptor> create_quad_buffer(ID3DBlob* vs) {
    // Input layout creation
    ID3D11InputLayout* inputLayout;
    {
      D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
          {"POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
           D3D11_INPUT_PER_VERTEX_DATA, 0},
          {"COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
           D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

      HRESULT hResult = m_Device->CreateInputLayout(
          inputElementDesc, ARRAYSIZE(inputElementDesc), vs->GetBufferPointer(),
          vs->GetBufferSize(), &inputLayout);
      assert(SUCCEEDED(hResult));
      vs->Release();
    }

    // Buffer creation
    ID3D11Buffer* buffer;
    u32 numVers;
    u32 stride;
    u32 offset;
    {
      f32 vertexData[] = {// x, y, r, g, b, a
                          -0.5f, 0.5f, 0.f,  1.f,  0.f,   1.f,   0.5f, -0.5f,
                          1.f,   0.f,  0.f,  1.f,  -0.5f, -0.5f, 0.f,  0.f,
                          1.f,   1.f,  0.5f, 0.5f, 1.f,   1.f,   0.f,  1.f};

      stride = 6 * sizeof(f32);
      numVers = sizeof(vertexData) / stride;
      offset = 0;

      D3D11_BUFFER_DESC vertexBufferDesc = {};
      vertexBufferDesc.ByteWidth = sizeof(vertexData);
      vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
      vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

      D3D11_SUBRESOURCE_DATA vertexSubresourceData = {vertexData};

      HRESULT hResult = m_Device->CreateBuffer(&vertexBufferDesc,
                                               &vertexSubresourceData, &buffer);
      assert(SUCCEEDED(hResult));
    }

    // Create index buffer
    ID3D11Buffer* indexBuffer;
    u32 indexData[] = {0, 1, 2, 0, 3, 1};
    {
      D3D11_BUFFER_DESC indexBufferDesc = {};
      indexBufferDesc.ByteWidth = sizeof(indexData);
      indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
      indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

      D3D11_SUBRESOURCE_DATA indexSubresourceData = {indexData};

      HRESULT hResult = m_Device->CreateBuffer(
          &indexBufferDesc, &indexSubresourceData, &indexBuffer);
      assert(SUCCEEDED(hResult));
    }

    return std::move(std::make_unique<Extra::BufferDescriptor>(
        buffer, indexBuffer, inputLayout, stride, offset, numVers,
        ARRAYSIZE(indexData)));
  }

  void next_frame() {
    // Clear render view
    static f32 bg[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, bg);

    // Clear depth and stencil view
    if (m_IsDepthOrStencilBufferEnable) {
      m_DeviceContext->ClearDepthStencilView(
          m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    // Set the viewport
    static RECT winRect;
    GetClientRect(m_WindowHandle, &winRect);
    D3D11_VIEWPORT viewPort = {0.0f,
                               0.0f,
                               (f32)(winRect.right - winRect.left),
                               (f32)(winRect.bottom - winRect.top),
                               0.0f,
                               1.0f};

    if (m_IsDepthOrStencilBufferEnable) {
      viewPort.MinDepth = 0.0f;
      viewPort.MaxDepth = 1.0f;
    }

    m_DeviceContext->RSSetViewports(1, &viewPort);

    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
  }

  void draw_command(Extra::BufferDescriptor* bufferDescriptor,
                    ID3D11VertexShader* vertShader,
                    ID3D11PixelShader* pixelShader) {
    // Set the topology (method that will be used while drawing elements)
    m_DeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set the input layout (for buffer usage)
    m_DeviceContext->IASetInputLayout(bufferDescriptor->inputLayout);

    // Set the shaders
    m_DeviceContext->VSSetShader(vertShader, nullptr, 0);
    m_DeviceContext->PSSetShader(pixelShader, nullptr, 0);

    // Set the vertex buffers
    m_DeviceContext->IASetVertexBuffers(0, 1, &bufferDescriptor->buffer,
                                        &bufferDescriptor->stride,
                                        &bufferDescriptor->offset);

    // Set the index buffer
    m_DeviceContext->IASetIndexBuffer(bufferDescriptor->indexBuffer,
                                      DXGI_FORMAT_R32_UINT, 0);

    // Draw call
    if (bufferDescriptor->indexCount == 0) {
      m_DeviceContext->Draw(bufferDescriptor->vertexCount, 0);
    } else {
      m_DeviceContext->DrawIndexed(bufferDescriptor->indexCount, 0, 0);
    }

    // Set the how to sync presentation of frame will handle
    m_SwapChain->Present(1, 0);
  }

 private:
  ID3D11Device* m_Device = nullptr;
  ID3D11DeviceContext* m_DeviceContext = nullptr;
  ID3D11RenderTargetView* m_RenderTargetView = nullptr;
  IDXGISwapChain* m_SwapChain = nullptr;

  // for the depth and stencil buffer
  ID3D11DepthStencilView* m_DepthStencilView;
  ID3D11Texture2D* m_DepthStencilBuffer;

  u32 m_Width{800};
  u32 m_Height{600};
  HWND m_WindowHandle = nullptr;
  HINSTANCE m_Instance = nullptr;

  bool m_IsInitialized{false};
  bool m_IsDepthOrStencilBufferEnable{false};
};
}  // namespace Extension::DX11

namespace Alien {
class DX11Context {
 public:
  DX11Context() = default;

  ~DX11Context() {
    //    physicalDevice.destroy_device();
  }

  DX11Context(HWND handle, HINSTANCE instance, HDC context)
      : m_DeviceContext(context),
        m_WindowHandle(handle),
        m_Instance(instance) {}

  void set_context(HWND handle, HINSTANCE instance, HDC context) {
    m_WindowHandle = handle;
    m_Instance = instance;
    m_DeviceContext = context;
    m_Initialized = true;
  }

  void create_context_dx11() {
    assert(m_Initialized && "You must set context first!");
    physicalDevice.init_device(m_WindowHandle, m_Instance);
    physicalDevice.init_context_dx11(true);
  }

  Extension::DX11::DX11PhysicalDevice physicalDevice;

 private:
  HWND m_WindowHandle{nullptr};
  HINSTANCE m_Instance{};
  HDC m_DeviceContext{};

  bool m_Initialized{false};
};
}  // namespace Alien
#endif

#ifndef _WIN32
#ifndef ALIEN_DX11_HPP
#define ALIEN_DX11_HPP
#endif
#endif