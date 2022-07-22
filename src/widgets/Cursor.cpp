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

#include "Cursor.h"

#include <components/Animator.h>

#include <imgui.h>
#include <SFML/Window/Mouse.hpp>


Cursor::Cursor()
: animator_(std::make_unique<Animator>())
, currentAnimation_("default")
{
    texture_.loadFromFile("cursors.png");
    setTexture(texture_);
    setTextureRect(sf::IntRect(2, 4, 23, 23));

    animator_->loadAnimations("cursors.json");
}

Cursor::~Cursor() = default;

void Cursor::tick(float deltaTime, const sf::Window& window)
{
    animator_->tick(deltaTime);
    setTextureRect(animator_->getCurrentRect(currentAnimation_));

    const auto mouseX = static_cast<float>(sf::Mouse::getPosition(window).x);
    const auto mouseY = static_cast<float>(sf::Mouse::getPosition(window).y);
    setPosition(mouseX, mouseY);

    ImGui::Begin("Cursor - Animations");
    for(const auto& anim : animator_->getAnimations())
    {
        if(ImGui::Button(anim.name.c_str())) setCursor(anim.name);
    }
    ImGui::End();
}

void Cursor::setCursor(std::string_view name)
{
    currentAnimation_ = name;
}
