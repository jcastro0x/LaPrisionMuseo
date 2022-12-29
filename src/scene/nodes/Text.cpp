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

#include "Text.hpp"

#include <SFML/Graphics/Text.hpp>

#include <scene/Scene.hpp>
#include <Engine.hpp>
#include <assets/AssetManager.hpp>

using namespace lpm;


Text::Text(std::string_view fontName, unsigned fontSize)
: text_(std::make_unique<sf::Text>())
, fontName_(fontName.data())
, fontSize_(fontSize)
{
}

Text::~Text() = default;

void Text::setTextString(const sf::String& string)
{
    text_->setString(string);
    text_->setOrigin(text_->getGlobalBounds().width / 2.f, text_->getGlobalBounds().height / 2.f);
}

void Text::setTextFillColor(const sf::Color& color)
{
    text_->setFillColor(color);
}

void Text::init()
{
    auto& resources = getSceneOwner()->getEngine()->getAssetManager();

    const auto font = resources.loadFont("fonts/" + fontName_ + ".ttf");
    text_->setFont(*font);
    text_->setCharacterSize(fontSize_);
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(*text_, states);
}
