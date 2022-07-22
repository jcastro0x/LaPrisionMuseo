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

#include "LoginSceneNode.h"

#include <widgets/QuadAspectRatio.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include <imgui.h>



LoginSceneNode::LoginSceneNode()
: background_(std::make_unique<QuadAspectRatio>("loginScreen.png", QuadAspectRatio::EAspectRatioRule::FitToParent))
{
    texture.loadFromFile("loginScreen.png");
    texture.setSmooth(false);
    sprite.setTexture(texture);
}

LoginSceneNode::~LoginSceneNode() = default;


void LoginSceneNode::init()
{

}

void LoginSceneNode::tick(float /*deltaTime*/)
{
    
}

void LoginSceneNode::destroy()
{
    
}

float x = 0, y = 0, w = 640, h = 480;
float vx = 0, vy = 0, vw = 1, vh = 1;
bool b = false;
void LoginSceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //target.draw(*background_, states);
    const auto quadSize = getQuadSize(target);

    ImGui::Begin("AspectRatio");
    ImGui::Checkbox("auto", &b);
    if(b)
    {
        x = 0;
        y = 0;
        w = quadSize.texWidth;
        h = quadSize.texHeight;

        ImGui::LabelText("x", "%f", x);
        ImGui::LabelText("y", "%f", y);
        ImGui::LabelText("w", "%f", w);
        ImGui::LabelText("h", "%f", h);
    }
    else
    {
        ImGui::DragFloat("x", &x);
        ImGui::DragFloat("y", &y);
        ImGui::DragFloat("w", &w);
        ImGui::DragFloat("h", &h);
    }
    ImGui::Separator();

    ImGui::DragFloat("vx", &vx);
    ImGui::DragFloat("vy", &vy);
    ImGui::DragFloat("vw", &vw);
    ImGui::DragFloat("vh", &vh);

    ImGui::Spacing();
    ImGui::LabelText("win", "%dx%d", target.getSize().x, target.getSize().y);
    ImGui::LabelText("req", "%.0fx%.0f", quadSize.texWidth, quadSize.texHeight);
    ImGui::End();

    sf::View copy = target.getView();

    auto view = sf::View({
        x,y,w,h
    });
    view.setViewport({vx,vy,vw,vh});

    target.setView(view);
    target.draw(sprite, states);
    target.setView(copy);
}


LoginSceneNode::QuadSize LoginSceneNode::getQuadSize(sf::RenderTarget& target) const
{
    QuadSize quadSize;

    sf::Vector2f tarSizeF = { static_cast<float>(target.getSize().x),   static_cast<float>(target.getSize().y) };

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
