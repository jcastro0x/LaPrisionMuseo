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

#include "ClickableText.h"

#include <SFML/Graphics/Text.hpp>

#include <scene/Scene.h>
#include <Engine.h>

#include <imgui.h>


ClickableText::ClickableText(sf::String string, Scene* owner)
: SceneNode(owner)
, text_(std::make_unique<sf::Text>())
{
    text_->setFont(owner_->getEngine()->getEntryFont());
    text_->setString(string);
    text_->setOrigin(text_->getGlobalBounds().width / 2.f, text_->getGlobalBounds().height / 2.f);
}

ClickableText::~ClickableText() = default;

void ClickableText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    const auto mousePos = owner_->getSceneMousePos(target);

    auto clickableGlobalRect = text_->getGlobalBounds();
    clickableGlobalRect.left += getPosition().x;
    clickableGlobalRect.top += getPosition().y;

    if(clickableGlobalRect.contains((float)mousePos.x, (float)mousePos.y))
    {
        text_->setFillColor(sf::Color::Yellow);
        owner_->getEngine()->getCursor().setCursor("arrow_rotate");
    }
    else
    {
        text_->setFillColor(sf::Color::White);
        owner_->getEngine()->getCursor().setCursor("default");
    }

    states.transform *= getTransform();
    target.draw(*text_, states);
}
