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

#include "ClickableText.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Audio/Sound.hpp>

#include <scene/Scene.hpp>
#include <Engine.hpp>

#include <imgui.h>


ClickableText::ClickableText(const sf::String& string, Scene* owner)
: SceneNode(owner)
, text_(std::make_unique<sf::Text>())
, soundHover_(std::make_unique<sf::Sound>())
, soundClick_(std::make_unique<sf::Sound>())
{
    const auto& resources = owner_->getEngine()->getResources();

    if(auto font = resources.getFont("FontEntry"))
    {
        text_->setFont(**font);
        text_->setString(string);
        text_->setOrigin(text_->getGlobalBounds().width / 2.f, text_->getGlobalBounds().height / 2.f);
    }
    else
    {
        throw resource_exception();
    }

    if(auto soundHover = resources.getSoundBuffer("ButtonHover"))
    {
        soundHover_->setBuffer(**soundHover);
    }
    else
    {
        throw resource_exception();
    }

    if(auto soundClick = resources.getSoundBuffer("ButtonClick"))
    {
        soundClick_->setBuffer(**soundClick);
    }
    else
    {
        throw resource_exception();
    }

}

ClickableText::~ClickableText() = default;

void ClickableText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(*text_, states);
}

void ClickableText::tick(float deltaTime)
{
    SceneNode::tick(deltaTime);

    const auto mousePos = owner_->getSceneMousePos();

    auto clickableGlobalRect = text_->getGlobalBounds();
    clickableGlobalRect.left += getPosition().x;
    clickableGlobalRect.top += getPosition().y;

    if(clickableGlobalRect.contains((float)mousePos.x, (float)mousePos.y))
    {
        text_->setFillColor(sf::Color::Yellow);

        if(!bMouseEnter_)
        {
            bMouseEnter_ = true;
            onMouseEnter();
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !bMouseClickDown_)
        {
            bMouseClickDown_ = true;
            onMouseClickDown();
        }

        if(!sf::Mouse::isButtonPressed(sf::Mouse::Left) && bMouseClickDown_)
        {
            bMouseClickDown_ = false;
            onMouseClickUp();
        }
    }
    else
    {
        text_->setFillColor(sf::Color::White);

        if(bMouseEnter_)
        {
            bMouseEnter_ = false;
            bMouseClickDown_ = false;
            onMouseExit();
        }
    }
}

void ClickableText::bindOnClick(const std::function<void()>& onClicked)
{
    onClicked_ = onClicked;
}

void ClickableText::onMouseEnter() const
{
    owner_->getEngine()->getCursor().setCursor("arrow_rotate");
    soundHover_->play();
}

void ClickableText::onMouseExit() const
{
    owner_->getEngine()->getCursor().setCursor("default");
}

void ClickableText::onMouseClickDown() const
{

}

void ClickableText::onMouseClickUp() const
{
    soundClick_->play();
    if(onClicked_)
    {
        onClicked_();
    }
}
