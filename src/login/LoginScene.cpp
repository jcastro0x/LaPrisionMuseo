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

#include "LoginScene.hpp"

#include <components/Internationalization.hpp>
#include <scene/nodes/BackgroundNode.hpp>
#include <scene/nodes/ClickableText.hpp>

#include <scene/SceneManager.hpp>

#include <Engine.hpp>

#include <imgui.h>

using namespace lpm;

LoginScene::LoginScene(class Engine* engine) : Scene(engine)
{
    addSceneNode<BackgroundNode>("loginScreen.png")
    .setName("Background")
    .setDrawOrder(CommonDepths::BACKGROUND);

    auto addButton = [&](sf::String const& string, sf::Vector2f pos, const std::function<void()>& onClicked){
        auto& button = addSceneNode<ClickableText>();
        button.setTextString(string);
        button.setDrawOrder(CommonDepths::MIDDLE);
        button.setPosition(pos);
        button.bindOnClick(onClicked);
    };

    addButton(getEngine()->getI18N().getString("ui", "play_online"),  {320.f, 227.f}, [this](){
        getEngine()->loadScene("world");
    });
    addButton(getEngine()->getI18N().getString("ui", "play_offline"), {320.f, 268.f}, [&](){

    });
    addButton(getEngine()->getI18N().getString("ui", "settings"),     {320.f, 307.f}, [&](){

    });
    addButton(getEngine()->getI18N().getString("ui", "credits"),      {320.f, 346.f}, [&](){

    });
    addButton(getEngine()->getI18N().getString("ui", "colaborate"),   {320.f, 385.f}, [&](){

    });
    addButton(getEngine()->getI18N().getString("ui", "quit"),         {320.f, 424.f}, [this](){
        getEngine()->stop();
    });
}

LoginScene::~LoginScene() = default;


void LoginScene::tick(float deltaTime)
{
    Scene::tick(deltaTime);
}
