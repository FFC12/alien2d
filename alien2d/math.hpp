/*
MIT License

Copyright(c) 2022 Furkan Fatih Cetindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifdef _WIN32
#pragma once
#elif
#ifndef ALIEN_HPP
#define ALIEN_HPP
#endif
#endif
#include "base.hpp"
#include <DirectXMath.h>

namespace Alien {
    namespace Math {
        struct Vector4 {
            f32 x, y, z, w;
        };

        struct Vector3 {
            f32 x, y, z;
        };

        struct Vector2 {
            f32 x, y;
        };
    }

    namespace Primitive {
        using namespace Math;

        struct Vertex {
            DirectX::XMFLOAT3 pos;

            Vertex(float x, float y, float z): pos(x,y,z) {}
            //Vector4 pos;
            //Vector4 col;
            //Vector2 texCoord;
        };
    }
}

#ifndef _WIN32
#ifndef ALIEN_HPP
#define ALIEN_HPP
#endif
#endif