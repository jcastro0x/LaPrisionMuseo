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

#include <components/AspectRatio.h>
#include <components/Internationalization.h>

#include <imgui.h>



LoginSceneNode::LoginSceneNode()
{
    texture.loadFromFile("loginScreen.png");
    texture.setSmooth(false);
    sprite.setTexture(texture);

    font.loadFromFile("FontEntry.ttf");
    text.setStyle(sf::Text::Style::Bold);
    text.setFont(font);
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

float x = 268, y = 220;
char t[100];
void LoginSceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    ImGui::Begin("Font - Debug");
    ImGui::DragFloat("x", &x);
    ImGui::DragFloat("y", &y);
    ImGui::InputText("text", &t[0], 100);
    ImGui::End();

    sf::View originalView = target.getView();

    const auto arView = 
    AspectRatio::getViewportAspetRatio(texture.getSize(), target.getSize(), AspectRatio::EAspectRatioRule::FitToParent);
    target.setView(arView);

    target.draw(sprite, states);
    
    text.setString(i18n::getString("ui", "Play Online"));
    text.setString(sf::String(t));
    text.setPosition({x,y});
    target.draw(text, states);

    target.setView(originalView);
}
