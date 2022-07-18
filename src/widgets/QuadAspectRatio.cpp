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

#include "QuadAspectRatio.h"
#include <SFML/Graphics/RenderTarget.hpp>

/**
 [0]----[1]
  .      .
  .      .
 [3]----[2]
 */

QuadAspectRatio::QuadAspectRatio()
: vertices_(sf::Quads, 4)
{
    texture_.loadFromFile("AL_Almacen1.jpg");
    texture_.setSmooth(false);
    texture_.setSrgb(false);
    texture_.setRepeated(false);

    aspectRatio_ = static_cast<float>(texture_.getSize().x)
                 / static_cast<float>(texture_.getSize().y);

    const sf::Vector2f texSizeF = {static_cast<float>(texture_.getSize().x), static_cast<float>(texture_.getSize().y)};

    vertices_[0].texCoords = { 0.f,        0.f};
    vertices_[1].texCoords = { texSizeF.x, 0.f};
    vertices_[2].texCoords = { texSizeF.x, texSizeF.y};
    vertices_[3].texCoords = { 0.f,        texSizeF.y};

//    vertices_[0].color = sf::Color::Red;
//    vertices_[1].color = sf::Color::Green;
//    vertices_[2].color = sf::Color::Blue;
//    vertices_[3].color = sf::Color::Yellow;

    vertices_[0].position = sf::Vector2f(0,   0);
    vertices_[1].position = sf::Vector2f(100, 0);
    vertices_[2].position = sf::Vector2f(100, 100);
    vertices_[3].position = sf::Vector2f(0,   100);
}

void QuadAspectRatio::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &texture_;

    const sf::Vector2f tarSizeF = {static_cast<float>(target.getSize().x), static_cast<float>(target.getSize().y)};

    vertices_[0].position = sf::Vector2f(0,   0);
    vertices_[1].position = sf::Vector2f(640, 0);
    vertices_[2].position = sf::Vector2f(640, 480);
    vertices_[3].position = sf::Vector2f(0,   480);

    target.draw(vertices_, states);
}
