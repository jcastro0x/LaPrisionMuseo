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

#include "AspectRatio.h"

sf::View AspectRatio::getViewportAspectRatio(const sf::Vector2u& textureSize, const sf::Vector2u& targetSize, EAspectRatioRule rule)
{
    QuadSize quadSize = getQuadSize(textureSize, targetSize, rule);

    auto view = sf::View({
        0, 0, static_cast<float>(textureSize.x), static_cast<float>(textureSize.y)
    });
    view.setViewport({
        quadSize.widthGap  / static_cast<float>(targetSize.x),
        quadSize.heightGap / static_cast<float>(targetSize.y),
        quadSize.texWidth  / static_cast<float>(targetSize.x),
        quadSize.texHeight / static_cast<float>(targetSize.y)
    });

    return view;
}

std::optional<sf::Vector2i> AspectRatio::transformPointToTextureCoords(const sf::Vector2u& textureSize, const sf::Vector2u& targetSize, EAspectRatioRule rule, sf::Vector2i point)
{
    sf::Vector2i transform;

    QuadSize quadSize = getQuadSize(textureSize, targetSize, rule);

    auto diffHeight = static_cast<float>(textureSize.y) / quadSize.texHeight;
    auto diffWidth  = static_cast<float>(textureSize.x) / quadSize.texWidth;

    transform.y = int(point.y * diffHeight - quadSize.heightGap * diffHeight);
    transform.x = int(point.x * diffWidth  - quadSize.widthGap  * diffWidth);

    if(transform.y > static_cast<int>(textureSize.y) - 1
    || transform.x > static_cast<int>(textureSize.x) - 1)
    {
        return {};
    }

    return transform;
}

AspectRatio::QuadSize AspectRatio::getQuadSize(const sf::Vector2u& textureSize, const sf::Vector2u& targetSize, EAspectRatioRule rule)
{
    QuadSize quadSize;
    const auto aspectRatioTexture = textureSize.x / static_cast<float>(textureSize.y);

    sf::Vector2f tarSizeF = { static_cast<float>(targetSize.x),   static_cast<float>(targetSize.y) };

    // height needed to use with target.getSize().x that generate correct aspect ratio
    quadSize.texHeight = tarSizeF.x / aspectRatioTexture;
    quadSize.texWidth = tarSizeF.x;

    if(quadSize.texHeight > tarSizeF.y && rule == EAspectRatioRule::FitToParent)
    {
        quadSize.texHeight = tarSizeF.y;

        // width needed to use with target.getSize().y that generate correct aspect ratio
        quadSize.texWidth = aspectRatioTexture * quadSize.texHeight;
    }

    // Top and bottom gap to vertically center de image
    quadSize.heightGap = (tarSizeF.y - quadSize.texHeight) / 2;

    // Right and left gap to horizontally center de image
    quadSize.widthGap  = (tarSizeF.x - quadSize.texWidth) / 2;

    return quadSize;
}
