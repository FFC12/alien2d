/*
MIT License

Copyright(c) 2022 Furkan Fatih Cetindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
                                                              copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

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
#ifndef BASE_HPP
#define BASE_HPP
#endif
#endif

#ifdef _WIN32
#include <Windows.h>
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <wingdi.h>
#endif

#include <base.hpp>

namespace Extra {
struct BufferDescriptor {
#if defined(_WIN32) && defined(ALIEN_DX11)
  BufferDescriptor(ID3D11Buffer* buffer, ID3D11InputLayout* layout, u32 stride,
                   u32 offset, u32 count)
      : buffer(buffer),
        inputLayout(layout),
        stride(stride),
        offset(offset),
        vertexCount(count) {}

  ID3D11Buffer* buffer;
  ID3D11InputLayout* inputLayout;
#else
#endif

  u32 vertexCount;
  u32 offset;
  u32 stride;
};
}  // namespace Extra

#ifndef _WIN32
#ifndef BASE_HPP
#define BASE_HPP
#endif
#endif