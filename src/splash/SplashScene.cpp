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

#include <array>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <scene/nodes/BackgroundNode.hpp>

#include <imgui.h>


using namespace lpm;

SplashScene::SplashScene(Engine* engine)
: Scene(engine)
, shader_(std::make_unique<sf::Shader>())
, rectangleShape_(std::make_unique<sf::RectangleShape>())
{
    initializeTextures();
    initializeShader();

    rectangleShape_->setSize({640, 450});
    rectangleShape_->setPosition(0,0);
    rectangleShape_->setTexture(textures_[0].get());
}

SplashScene::~SplashScene()
{
}

float texture0_intensity = 1;
float texture1_intensity = 1;
float texture2_intensity = 1;
float texture3_intensity = 1;

void SplashScene::tick(float deltaTime)
{
    Scene::tick(deltaTime);

    static float totalTime = 0;
    totalTime += deltaTime;
    shader_->setUniform("time", totalTime);

    ImGui::Begin("SplashScreen");
    ImGui::DragFloat("texture0_intensity", &texture0_intensity, 0.1f);
    ImGui::DragFloat("texture1_intensity", &texture1_intensity, 0.1f);
    ImGui::DragFloat("texture2_intensity", &texture2_intensity, 0.1f);
    ImGui::DragFloat("texture3_intensity", &texture3_intensity, 0.1f);
    ImGui::End();

    shader_->setUniform("texture0_intensity", texture0_intensity);
    shader_->setUniform("texture1_intensity", texture1_intensity);
    shader_->setUniform("texture2_intensity", texture2_intensity);
    shader_->setUniform("texture3_intensity", texture3_intensity);

}

void SplashScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Scene::draw(target, states);

    states.shader = shader_.get();
    target.draw(*rectangleShape_, states);


}

void SplashScene::initializeTextures()
{
    std::array<const char*, 5> files = {
        "splash/splash00.jpg",
        "splash/splash01.jpg",
        "splash/splash02.jpg",
        "splash/splash03.jpg",
        "splash/splashMask.png"
    };

    textures_.reserve(files.size());
    for(size_t i = 0; i < files.size(); i++)
    {
        textures_.emplace_back(std::make_unique<sf::Texture>());
        textures_[i]->loadFromFile(files[i]);
        textures_[i]->setSrgb(true);
        textures_[i]->setRepeated(true);
        textures_[i]->setSmooth(true);
    }

}

void SplashScene::initializeShader()
{
    shader_->loadFromFile("splash/splash.frag", sf::Shader::Fragment);

    shader_->setUniform("mask_texture", *textures_[4]);

    shader_->setUniform("texture0", *textures_[0]);
    shader_->setUniform("texture1", *textures_[1]);
    shader_->setUniform("texture2", *textures_[2]);
    shader_->setUniform("texture3", *textures_[3]);

    shader_->setUniform("texture0_intensity", 1.f);
    shader_->setUniform("texture1_intensity", 1.f);
    shader_->setUniform("texture2_intensity", 1.f);
    shader_->setUniform("texture3_intensity", 1.f);

    shader_->setUniform("pan_velocity", 0.5f);
}