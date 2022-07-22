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

#include "LoginScene.h"

#include <widgets/QuadAspectRatio.h>
#include <components/Internationalization.h>
#include <scene/nodes/BackgroundNode.h>
#include <scene/nodes/ClickableText.h>

#include <Engine.h>

#include <imgui.h>

LoginScene::LoginScene(class Engine* engine) : Scene(engine)
{
    addSceneNode(std::make_unique<BackgroundNode>("loginScreen.png", this));

    auto addButton = [&](sf::String string, sf::Vector2f pos){
        auto button = std::make_unique<ClickableText>(string, this);
        button->setPosition(pos);
        addSceneNode(std::move(button));
    };

    addButton(i18n::getString("ui", "Play Online"), {320.f, 227.f});
    addButton(i18n::getString("ui", "Play Offline"), {320.f, 268.f});
    addButton(i18n::getString("ui", "Settings"), {320.f, 307.f});
    addButton(i18n::getString("ui", "Credits"), {320.f, 346.f});
    addButton(i18n::getString("ui", "Colaborate"), {320.f, 385.f});
    addButton(i18n::getString("ui", "Exit"), {320.f, 424.f});

}

LoginScene::~LoginScene() = default;


void LoginScene::tick(float deltaTime)
{
    Scene::tick(deltaTime);
}


/*
 * 227
 * 268
 * 307
 * 346
 * 385
 * 424
 * */

// float x = 320, y = 227;
// float px = 0, py = 0;
// char t[100];
// void LoginScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     ImGui::Begin("Font - Debug");
//     ImGui::DragFloat("x", &x);
//     ImGui::DragFloat("y", &y);
//     ImGui::Separator();
//     ImGui::DragFloat("px", &px);
//     ImGui::DragFloat("py", &py);
//     ImGui::Separator();
//     ImGui::InputText("text", &t[0], 100);
//     ImGui::End();

//     sf::View originalView = target.getView();

//     const auto arView =
//             AspectRatio::getViewportAspectRatio(texture.getSize(), target.getSize(),
//                                                 AspectRatio::EAspectRatioRule::FitToParent);
//     target.setView(arView);

//     target.draw(sprite, states);
    
//     text.setString(i18n::getString("ui", "Play Online"));
//     text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
//     text.setPosition({x,y});

//     target.draw(text, states);

//     target.setView(originalView);
// }
