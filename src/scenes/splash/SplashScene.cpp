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

#include "SplashScene.hpp"
#include "SplashNode.hpp"

#include <cmath>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <imgui.h>

#include <Engine.hpp>
#include <system/ResourcesManager.hpp>
#include <system/Internationalization.hpp>
#include <system/Configuration.hpp>

#include <scene/nodes/Text.hpp>


using namespace lpm;

SplashScene::SplashScene(Engine* engine)
: Scene(engine)
{
    splash = &addSceneNode<SplashNode>();
    splash->setDrawOrder(CommonDepths::BACKGROUND);

    //TODO: Get textures from Resouces instead "loadFromFile"
    splash->changeTexture(SplashNode::TEXTURE_0, "splash/splash00.jpg");
    splash->changeTexture(SplashNode::TEXTURE_1, "splash/splash04.jpg");
    splash->changeTexture(SplashNode::TEXTURE_2, "splash/splash02.jpg");
    splash->changeTexture(SplashNode::TEXTURE_3, "splash/splash03.jpg");

//    auto& logoTextShadow = addSceneNode<lpm::Text>("FontLogo", 150);
//    logoTextShadow.setTextFillColor(sf::Color::Black);
//    logoTextShadow.setTextString("La Prision");
//    logoTextShadow.setDrawOrder(CommonDepths::MIDDLE, 0);
//    logoTextShadow.setPosition(223 + 2, Configuration::BACKGROUND_TEX_SIZE_Y - 230.f - 2);
//
//    auto& logoText = addSceneNode<lpm::Text>("FontLogo", 150);
//    logoText.setTextString("La Prision");
//    logoText.setDrawOrder(CommonDepths::MIDDLE, 1);
//    logoText.setPosition(223, Configuration::BACKGROUND_TEX_SIZE_Y - 230.f);
//
//
//    auto& museoTextShadow = addSceneNode<lpm::Text>("FontEntry", 125);
//    museoTextShadow.setTextFillColor(sf::Color::Black);
//    museoTextShadow.setTextString("MUSEO");
//    museoTextShadow.setDrawOrder(CommonDepths::MIDDLE, 0);
//    museoTextShadow.setPosition(513 + 2, Configuration::BACKGROUND_TEX_SIZE_Y - 215.f - 2);
//
//    auto& museoText = addSceneNode<lpm::Text>("FontEntry", 125);
//    museoText.setTextFillColor(sf::Color::Red);
//    museoText.setTextString("MUSEO");
//    museoText.setDrawOrder(CommonDepths::MIDDLE, 1);
//    museoText.setPosition(513, Configuration::BACKGROUND_TEX_SIZE_Y - 215.f);
//
//
//    pressAnyKeyText = &addSceneNode<lpm::Text>("FontEntry", 25);
//    pressAnyKeyText->setTextFillColor(sf::Color::White);
//    pressAnyKeyText->setTextString(getEngine()->getI18N().getString("ui", "press_any_key"));
//    pressAnyKeyText->setDrawOrder(CommonDepths::MIDDLE, 0);
//    pressAnyKeyText->setPosition(Configuration::BACKGROUND_TEX_SIZE_X / 2.f, Configuration::BACKGROUND_TEX_SIZE_Y - 85.f);
}

void SplashScene::tick(float deltaTime)
{
    Scene::tick(deltaTime);

    sf::Color color = sf::Color::White;

    static float totalTime = 0;
    totalTime += deltaTime;
    color.a = static_cast<uint8_t>(std::abs(std::sin(totalTime * 2)) * 255);
    pressAnyKeyText->setTextFillColor(color);


//    ImGui::Begin("SplashScene");
//    if (ImGui::BeginTable("table1", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
//    {
//        for (int row = 0; row < 8; row++)
//        {
//            ImGui::TableNextRow();
//            for (int column = 0; column < 4; column++)
//            {
//                ImGui::TableSetColumnIndex(column);
//
//                char buf[32];
//                sprintf(buf, "%d%d_%d", column, row, row);
//                if(ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
//                {
//                    char fileName[64];
//                    sprintf(fileName, "splash/splash0%d.jpg", row);
//                    splash->changeTexture(column, fileName);
//                }
//            }
//        }
//    }
//    ImGui::EndTable();
//    ImGui::End();

    //TODO: Detect any key instead this
    bool bLoadLoginScene = sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
                        || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)
                        || sf::Mouse::isButtonPressed(sf::Mouse::Left)
                        || sf::Mouse::isButtonPressed(sf::Mouse::Right);
    if(bLoadLoginScene)
    {
        getEngine()->loadScene("login");
    }
}
