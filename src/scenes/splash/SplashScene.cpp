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

#include <imgui.h>

using namespace lpm;

SplashScene::SplashScene(Engine* engine)
: Scene(engine)
{
    splash = &addSceneNode<SplashNode>();

    splash->changeTexture(SplashNode::TEXTURE_0, "splash/splash00.jpg");
    splash->changeTexture(SplashNode::TEXTURE_1, "splash/splash01.jpg");
    splash->changeTexture(SplashNode::TEXTURE_2, "splash/splash02.jpg");
    splash->changeTexture(SplashNode::TEXTURE_3, "splash/splash03.jpg");
}

void SplashScene::tick(float deltaTime)
{
    Scene::tick(deltaTime);

    ImGui::Begin("SplashScene");
    if (ImGui::BeginTable("table1", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        for (int row = 0; row < 8; row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 4; column++)
            {
                ImGui::TableSetColumnIndex(column);

                char buf[32];
                sprintf(buf, "%d%d_%d", column, row, row);
                if(ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f)))
                {
                    char fileName[64];
                    sprintf(fileName, "splash/splash0%d.jpg", row);
                    splash->changeTexture(column, fileName);
                }
            }
        }
    }
    ImGui::EndTable();
    ImGui::End();
}
