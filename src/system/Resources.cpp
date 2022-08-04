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

#include "Resources.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

using namespace lpm;

Resources::Resources()
{
    auto createTexture = [&](std::string_view key, std::string_view fileName){
        if(auto asset = std::make_unique<sf::Texture>(); asset->loadFromFile(fileName.data()))
        {
            textures_.try_emplace(key.data(), std::move(asset));
        }
        else throw resource_exception();
    };

    auto createSound = [&](std::string_view key, std::string_view fileName){
        if(auto asset = std::make_unique<sf::SoundBuffer>(); asset->loadFromFile(fileName.data()))
        {
            sounds_.try_emplace(key.data(), std::move(asset));
        }
        else throw resource_exception();
    };

    auto createFont = [&](std::string_view key, std::string_view fileName){
        if(auto asset = std::make_unique<sf::Font>(); asset->loadFromFile(fileName.data()))
        {
            fonts_.try_emplace(key.data(), std::move(asset));
        }
        else throw resource_exception();
    };

    createTexture("Cursors", "cursors.png");
    createTexture("LoginScreen", "loginScreen.png");

    createSound("ButtonClick", "ButtonClick.wav");
    createSound("ButtonHover", "ButtonHover.wav");

    createFont("FontEntry", "FontEntry.ttf");
    createFont("FontLogo", "FontLogo.ttf");
}

Resources::~Resources() = default;

std::optional<const sf::Texture*> Resources::getTexture(std::string_view key) const
{
    if(const auto it = textures_.find(key.data()); it != textures_.cend())
    {
        return it->second.get();
    }
    return {};
}

std::optional<const sf::SoundBuffer*> Resources::getSoundBuffer(std::string_view key) const
{
    if(const auto it = sounds_.find(key.data()); it != sounds_.cend())
    {
        return it->second.get();
    }
    return {};
}

std::optional<const sf::Font*> Resources::getFont(std::string_view key) const
{
    if(const auto it = fonts_.find(key.data()); it != fonts_.cend())
    {
        return it->second.get();
    }
    return {};
}
