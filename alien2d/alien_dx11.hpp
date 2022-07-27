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

#define ALIEN_DX11

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

namespace Extension::DX11 {
struct DX11PhysicalDevice {
  explicit DX11PhysicalDevice(u32 w = 800, u32 h = 600)
      : m_Width(w), m_Height(h) {}

  ~DX11PhysicalDevice() {
    device->Release();
    swapChain->Release();
    deviceContext->Release();
    renderTargetView->Release();
  }

  // Initialize device (retrieve windows handle and instance)
  void init_device(HWND handle, HINSTANCE instance) {
    m_Instance = instance;
    m_WindowHandle = handle;
    m_IsInitialized = true;
  }

  // Initialize DirectX 11 context: buffer descriptor, swap chain
  // and render target.
  bool init_context_dx11() {
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

    // Create device instance and swapchain
    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
                                       NULL, NULL, NULL, D3D11_SDK_VERSION,
                                       &swapChainDesc, &swapChain, &device,
                                       NULL, &deviceContext);

    if (FAILED(hr)) {
      return false;
    }

    // Prepare back buffer for double buffering (DX11)
    ID3D11Texture2D* backBuffer;
    hr =
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    assert(SUCCEEDED(hr));

    // Create render target view for the texture
    hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
    assert(SUCCEEDED(hr));
    backBuffer->Release();

    // Set device context with render target view which is basically a texture
    // TODO: Stencil and depth buffers will be considered later
    deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

    return true;
  }

  // Compile vertex shader from the path
  bool compile_vertex_shader(LPCWSTR path, ID3DBlob** vertShaderBlob, ID3D11VertexShader** vertShader) const {
    // Create Vertex Shader
    ID3DBlob* vsBlob;
    ID3D11VertexShader* vertexShader;
    {
      ID3DBlob* shaderCompileErrorsBlob;
      HRESULT hResult =
          D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0,
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

      hResult = device->CreateVertexShader(vsBlob->GetBufferPointer(),
                                           vsBlob->GetBufferSize(), nullptr,
                                           &vertexShader);
      assert(SUCCEEDED(hResult));
    }

    *vertShaderBlob = vsBlob;
    *vertShader = vertexShader;

    return true;
  }

  // Compile pixel shader from the path
  bool compile_pixel_shader(LPCWSTR path, ID3DBlob** pixelShaderBlob, ID3D11PixelShader** pixShader) const {
    // Create Pixel Shader
    ID3DBlob* psBlob;
    ID3D11PixelShader* pixelShader;
    {
      ID3DBlob* shaderCompileErrorsBlob;
      HRESULT hResult =
          D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "ps_main",
                             "ps_5_0", 0, 0, &psBlob, &shaderCompileErrorsBlob);
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

      hResult = device->CreatePixelShader(psBlob->GetBufferPointer(),
                                          psBlob->GetBufferSize(), nullptr,
                                          &pixelShader);
      assert(SUCCEEDED(hResult));
      psBlob->Release();
    }

    *pixelShaderBlob = psBlob;
    *pixShader = pixelShader;

    return true;
  }

  // Create buffer (position & color)
  std::unique_ptr<Extra::BufferDescriptor> create_buffer(ID3DBlob* vs) {
    // Input layout creation
    ID3D11InputLayout* inputLayout;
    {
      D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
          {"POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
           D3D11_INPUT_PER_VERTEX_DATA, 0},
          {"COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
           D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}};

      HRESULT hResult = device->CreateInputLayout(
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
                          0.0f, 0.5f, 0.f, 1.f,   0.f,   1.f, 0.5f, -0.5f, 1.f,
                          0.f,  0.f,  1.f, -0.5f, -0.5f, 0.f, 0.f,  1.f,   1.f};

      stride = 6 * sizeof(f32);
      numVers = sizeof(vertexData) / stride;
      offset = 0;

      D3D11_BUFFER_DESC vertexBufferDesc = {};
      vertexBufferDesc.ByteWidth = sizeof(vertexData);
      vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
      vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

      D3D11_SUBRESOURCE_DATA vertexSubresourceData = {vertexData};

      HRESULT hResult = device->CreateBuffer(&vertexBufferDesc,
                                             &vertexSubresourceData, &buffer);
      assert(SUCCEEDED(hResult));
    }

    return std::move(std::make_unique<Extra::BufferDescriptor>(
        buffer, inputLayout, stride, offset, numVers));
  }

  void next_frame() {
    // Clear buffer color
    f32 bg[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    deviceContext->ClearRenderTargetView(renderTargetView, bg);

    // Set the viewport
    static RECT winRect;
    GetClientRect(m_WindowHandle, &winRect);
    D3D11_VIEWPORT viewPort = {0.0f,
                               0.0f,
                               (f32)(winRect.right - winRect.left),
                               (f32)(winRect.bottom - winRect.top),
                               0.0f,
                               1.0f};
    deviceContext->RSSetViewports(1, &viewPort);

    // Currently depth and stencil buffers are omitted
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
  }

  void draw_command(Extra::BufferDescriptor* bufferDescriptor,
                    ID3D11VertexShader* vertShader,
                    ID3D11PixelShader* pixelShader) {
    // Set the topology (method that will be used while drawing elements)
    deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set the input layout (for buffer usage)
    deviceContext->IASetInputLayout(bufferDescriptor->inputLayout);

    // Set the shaders
    deviceContext->VSSetShader(vertShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);

    // Set the vertex buffers
    deviceContext->IASetVertexBuffers(0, 1, &bufferDescriptor->buffer,
                                      &bufferDescriptor->stride,
                                      &bufferDescriptor->offset);

    // Draw call
    deviceContext->Draw(bufferDescriptor->vertexCount, 0);

    // Set the how to sync presentation of frame will handle
    swapChain->Present(1, 0);
  }

  ID3D11Device* device = nullptr;
  ID3D11DeviceContext* deviceContext = nullptr;
  ID3D11RenderTargetView* renderTargetView = nullptr;
  IDXGISwapChain* swapChain = nullptr;

 private:
  u32 m_Width{800};
  u32 m_Height{600};
  HWND m_WindowHandle = nullptr;
  HINSTANCE m_Instance = nullptr;

  bool m_IsInitialized{false};
};
}  // namespace Extension::DX11

namespace Alien {
class DX11Context {
 public:
  DX11Context() = default;
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
      physicalDevice.init_device(m_WindowHandle,m_Instance);
      physicalDevice.init_context_dx11();
  }

  Extension::DX11::DX11PhysicalDevice physicalDevice;
 private:
  HWND m_WindowHandle{nullptr};
  HINSTANCE m_Instance;
  HDC m_DeviceContext;

  bool m_Initialized{false};
};
}  // namespace Alien

#ifndef _WIN32
#ifndef ALIEN_DX11_HPP
#define ALIEN_DX11_HPP
#endif
#endif