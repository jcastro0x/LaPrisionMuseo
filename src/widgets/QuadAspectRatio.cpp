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

/**
 [0]----[1]
  .      .
  .      .
 [3]----[2]
 */

QuadAspectRatio::QuadAspectRatio()
: vertices_(sf::Quads, 4)
{

    //texture_.loadFromFile("AL_Almacen1.jpg");
    texture_.loadFromFile("checker640x480.png");
    texture_.setSmooth(false);
    texture_.setSrgb(false);
    texture_.setRepeated(false);

    aspectRatio_ = static_cast<float>(texture_.getSize().y)
                 / static_cast<float>(texture_.getSize().x);

    const sf::Vector2f texSizeF = {static_cast<float>(texture_.getSize().x), static_cast<float>(texture_.getSize().y)};

    vertices_[0].texCoords = { 0.f,        0.f};
    vertices_[1].texCoords = { texSizeF.x, 0.f};
    vertices_[2].texCoords = { texSizeF.x, texSizeF.y};
    vertices_[3].texCoords = { 0.f,        texSizeF.y};
}

#define VtxToStr(V) V.position.x, V.position.y
void QuadAspectRatio::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &texture_;


    const sf::Vector2f tarSizeF   = {static_cast<float>(target.getSize().x), static_cast<float>(target.getSize().y)};

    const auto targetAspectRatio = tarSizeF.y / tarSizeF.x;
    const auto deltaAspectRatio  = .0f + (targetAspectRatio - aspectRatio_);

    auto heightDiff = static_cast<float>(target.getSize().y - texture_.getSize().y);
    auto widthDiff  = static_cast<float>(target.getSize().x - texture_.getSize().x);

    vertices_[0].position = sf::Vector2f(0,             heightDiff / 2 - widthDiff / 2);
    vertices_[1].position = sf::Vector2f(tarSizeF.x,    heightDiff / 2 - widthDiff / 2);
    vertices_[2].position = sf::Vector2f(tarSizeF.x,    480 + heightDiff / 2 + widthDiff / 2);
    vertices_[3].position = sf::Vector2f(0,             480 + heightDiff / 2 + widthDiff / 2);



    ImGui::Begin("Redraw");
    ImGui::LabelText("Window", "x: %d y: %d", target.getSize().x, target.getSize().y);
    ImGui::LabelText("AR", "%.4f", deltaAspectRatio);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::LabelText("0", "(x: %.2f y: %.2f)", VtxToStr(vertices_[0]));
    ImGui::LabelText("1", "(x: %.2f y: %.2f)", VtxToStr(vertices_[1]));
    ImGui::LabelText("2", "(x: %.2f y: %.2f)", VtxToStr(vertices_[2]));
    ImGui::LabelText("3", "(x: %.2f y: %.2f)", VtxToStr(vertices_[3]));
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::LabelText("TextSiz", "(x: %.2f y: %.2f)", vertices_[1].position.x - vertices_[0].position.x, vertices_[3].position.y - vertices_[0].position.y);
    ImGui::LabelText("TextAR", "%.4f", (vertices_[1].position.x - vertices_[0].position.x) / (vertices_[3].position.y - vertices_[0].position.y));

    ImGui::End();

    target.draw(vertices_, states);
}
