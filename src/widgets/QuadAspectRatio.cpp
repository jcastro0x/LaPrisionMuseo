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

#include <imgui.h>

#include <chrono>

/**
 [0]----[1]
  .      .
  .      .
 [3]----[2]
 */

QuadAspectRatio::QuadAspectRatio(std::string_view textureName, EAspectRatioRule rule)
: vertices_(sf::Quads, 4)
, aspectRatioRule_(rule)
{
    texture_.loadFromFile(textureName.data());
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
}

std::optional<sf::Vector2u> QuadAspectRatio::transformPointToTextureCoords(sf::RenderTarget& target, sf::Vector2i point) const
{
    sf::Vector2u transform = { 
        static_cast<unsigned>(std::max(0, point.x)),
        static_cast<unsigned>(std::max(0, point.y)) 
    };


    sf::Vector2f tarSizeF = { static_cast<float>(target.getSize().x),   static_cast<float>(target.getSize().y) };
    sf::Vector2f texSizeF = { static_cast<float>(texture_.getSize().x), static_cast<float>(texture_.getSize().y)};

    auto h = tarSizeF.x / aspectRatio_;
    auto w = tarSizeF.x;

    if(h > tarSizeF.y && aspectRatioRule_ == EAspectRatioRule::FitToParent)
    {
        h = tarSizeF.y;
        w = aspectRatio_ * h;
    }

    const auto heightGap = (tarSizeF.y - h) / 2;
    const auto widthGap  = (tarSizeF.x - w) / 2;


    auto a1 = texSizeF.y / h;
    auto a2 = texSizeF.x / w;

    transform.y = transform.y * a1 - heightGap * a1;
    transform.x = transform.x * a2 - widthGap * a2;


    transform.x = std::clamp<float>(0, texSizeF.x, transform.x);
    transform.y = std::clamp<float>(0, texSizeF.y, transform.y);

    ImGui::Begin("QuadAspectRatio - transformMouseCoordToTextureCoords");
    ImGui::LabelText("transform", "x: %u y: %u", transform.x, transform.y);
    ImGui::End();

    return transform;
}

void QuadAspectRatio::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    auto start = std::chrono::system_clock::now();
    states.texture = &texture_;

    sf::Vector2f tarSizeF = { static_cast<float>(target.getSize().x),   static_cast<float>(target.getSize().y) };
    sf::Vector2f texSizeF = { static_cast<float>(texture_.getSize().x), static_cast<float>(texture_.getSize().y)};

    // height needed to use with target.getSize().x that generate correct aspect ratio
    auto h = tarSizeF.x / aspectRatio_;
    auto w = tarSizeF.x;

    if(h > tarSizeF.y && aspectRatioRule_ == EAspectRatioRule::FitToParent)
    {
        h = tarSizeF.y;

        // width needed to use with target.getSize().y that generate correct aspect ratio
        w = aspectRatio_ * h;
    }

    // Top and bottom gap to vertically center de image
    const auto heightGap = (tarSizeF.y - h) / 2;

    // Right and left gap to horizontally center de image
    const auto widthGap  = (tarSizeF.x - w) / 2;

    vertices_[0].position = sf::Vector2f(widthGap,     heightGap);
    vertices_[1].position = sf::Vector2f(w + widthGap, heightGap);
    vertices_[2].position = sf::Vector2f(w + widthGap, h + heightGap);
    vertices_[3].position = sf::Vector2f(widthGap,     h + heightGap);

    const auto end      = std::chrono::system_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    ImGui::Begin("QuadAspectRatio - Debug");
    ImGui::LabelText("Duration", "%d nanoseconds", duration);
    ImGui::LabelText("Window", "x: %d y: %d", target.getSize().x, target.getSize().y);
    ImGui::Spacing();
    ImGui::LabelText("TextSiz", "(x: %.2f y: %.2f)", vertices_[1].position.x - vertices_[0].position.x, vertices_[3].position.y - vertices_[0].position.y);
    ImGui::LabelText("TextAR", "%.4f", (vertices_[1].position.x - vertices_[0].position.x) / (vertices_[3].position.y - vertices_[0].position.y));
    ImGui::LabelText("TextDesHe", "%.4f", h);

    ImGui::End();

    target.draw(vertices_, states);
}

QuadAspectRatio::QuadSize QuadAspectRatio::getQuadSize(sf::RenderTarget& target) const
{
    QuadSize quadSize;

    sf::Vector2f tarSizeF = { static_cast<float>(target.getSize().x),   static_cast<float>(target.getSize().y) };
    sf::Vector2f texSizeF = { static_cast<float>(texture_.getSize().x), static_cast<float>(texture_.getSize().y)};

    // height needed to use with target.getSize().x that generate correct aspect ratio
    quadSize.texHeight = tarSizeF.x / aspectRatio_;
    quadSize.texWidth = tarSizeF.x;

    if(quadSize.texHeight > tarSizeF.y && aspectRatioRule_ == EAspectRatioRule::FitToParent)
    {
        quadSize.texHeight = tarSizeF.y;

        // width needed to use with target.getSize().y that generate correct aspect ratio
        quadSize.texWidth = aspectRatio_ * quadSize.texHeight;
    }

    // Top and bottom gap to vertically center de image
    quadSize.heightGap = (tarSizeF.y - quadSize.texHeight) / 2;

    // Right and left gap to horizontally center de image
    quadSize.widthGap  = (tarSizeF.x - quadSize.texWidth) / 2;

    return quadSize;
}
