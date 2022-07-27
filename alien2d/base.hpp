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
#ifndef BASE_HPP
#define BASE_HPP
#endif
#endif
#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <memory>
#include <map>

// Primitive types.
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = unsigned int;
using u64 = unsigned long long;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int;
using i64 = long long;
using f32 = float;
using f64 = double;

#ifndef _WIN32
#ifndef BASE_HPP
#define BASE_HPP
#endif
#endif