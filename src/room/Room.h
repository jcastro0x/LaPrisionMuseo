// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <string>
#include <array>

/**
 * @brief Compass directions.
 */
enum class EDirection : uint8_t
{
    None    = 0,

    North   = 0 << 1,
    East    = 1 << 2,
    South   = 1 << 3,
    West    = 1 << 4,

    Door    = 1 << 5,

    NorthEast = North | East,
    SouthEast = South | East,
    SouthWest = South | West,
    NorthWest = North | West
};


struct Node
{

};

struct Backgrounds : public Node
{
    std::string textureName;
    std::string maskName;

};

struct Exits
{
    uint32_t color;
    EDirection direction;
    Node* destiny;
};

template<size_t ArraySize>
struct Room : public Node
{




    std::string name;
    std::array<Backgrounds, ArraySize> backgrounds;
};
