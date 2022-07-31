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
#include <memory>

#include <components/Animator.hpp>

#include <SFML/System/Vector2.hpp>

namespace sf
{
    class SoundBuffer;
}

class RoomCamera
{
    using Anim = std::pair<sf::Vector2u, Animator::Animation>;

    struct Area
    {
        using Script = void(*)(size_t /*argc*/, void** /**argv*/);

        std::string cursorName_;
        std::vector<sf::Vector2u> vertices;
        Script script_;

        struct Debug
        {
            uint32_t color;
            std::string comment;
        } debug_;
    };

public:
    RoomCamera();
    ~RoomCamera();

private:
    void changeCamera(std::string_view name);
    void changeRoom(std::string_view name);
    void showInfo(std::string_view comment);

private:
    std::string name_;
    std::unique_ptr<sf::SoundBuffer> backgroundSfx_;
    std::vector<Anim> animations_;
};
