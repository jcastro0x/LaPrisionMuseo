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

#include <cstdint>
#include <optional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>

class AspectRatio
{
public:
    enum class EAspectRatioRule : uint8_t
    {
        Width,
        FitToParent
    };

    static sf::View getViewportAspectRatio(const sf::Vector2u& textureSize, const sf::Vector2u& targetSize, EAspectRatioRule rule);

    [[nodiscard]] static std::optional<sf::Vector2i> transformPointToTextureCoords(const sf::Vector2u& textureSize, const sf::Vector2u& targetSize, EAspectRatioRule rule, sf::Vector2i point);

private:
    struct QuadSize
    {
        float texWidth;
        float texHeight;
        float heightGap;
        float widthGap;
    };
    static QuadSize getQuadSize(const sf::Vector2u& textureSize, const sf::Vector2u& targetSize, EAspectRatioRule rule);
};
