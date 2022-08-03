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

#include "SplashNode.hpp"

#include <cassert>
#include <imgui.h>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <scene/nodes/BackgroundNode.hpp>
#include <Configuration.hpp>



using namespace lpm;

SplashNode::SplashNode()
: shader_(std::make_unique<sf::Shader>())
, rectangleShape_(std::make_unique<sf::RectangleShape>())
, maskTexture_(std::make_unique<sf::Texture>())
{
    initializeTextures();
    initializeShader();

    rectangleShape_->setSize({Configuration::BACKGROUND_TEX_SIZE_X, Configuration::BACKGROUND_TEX_SIZE_Y});
    rectangleShape_->setPosition(0,0);
    rectangleShape_->setTexture(textures_[TEXTURE_0].get());
}

SplashNode::~SplashNode() = default;

void SplashNode::changeTexture(size_t index, std::string_view textureName)
{
    assert(index <= 4 && "SplashNode::changeTexture called with value bigger than 4");
    initializeTexture(textures_[index].get(), textureName.data());
}

//float texture0_intensity = 1;
//float texture1_intensity = 1;
//float texture2_intensity = 1;
//float texture3_intensity = 1;

void SplashNode::tick(float deltaTime)
{
    SceneNode::tick(deltaTime);

    static float totalTime = 0;
    totalTime += deltaTime;
    shader_->setUniform("time", totalTime);

    for(size_t i = 0; i < texturesIntensities.size(); i++)
    {
        auto& intensity = texturesIntensities[i];
        const auto direction = intensity > texturesIntensitiesTargets[i]
                             ? -1.f
                             : +1.f;

        intensity = std::clamp(intensity + deltaTime * texturesIntensitiesVelocity * direction, 0.f, 1.f);
    }

    shader_->setUniform("textures_intensity[0]", texturesIntensities[0]);
    shader_->setUniform("textures_intensity[1]", texturesIntensities[1]);
    shader_->setUniform("textures_intensity[2]", texturesIntensities[2]);
    shader_->setUniform("textures_intensity[3]", texturesIntensities[3]);

//    ImGui::Begin("SplashNode");
//    ImGui::DragFloat("textures_intensity[0]", &texture0_intensity, 0.1f);
//    ImGui::DragFloat("textures_intensity[1]", &texture1_intensity, 0.1f);
//    ImGui::DragFloat("textures_intensity[2]", &texture2_intensity, 0.1f);
//    ImGui::DragFloat("textures_intensity[3]", &texture3_intensity, 0.1f);
//    ImGui::End();
//
//    shader_->setUniform("textures_intensity[0]", texture0_intensity);
//    shader_->setUniform("textures_intensity[1]", texture1_intensity);
//    shader_->setUniform("textures_intensity[2]", texture2_intensity);
//    shader_->setUniform("textures_intensity[3]", texture3_intensity);

}

void SplashNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.shader = shader_.get();
    target.draw(*rectangleShape_, states);
}

void SplashNode::initializeTextures()
{
    // Fill default pixels to fill textures with black color (RGBA32)
    std::array<sf::Uint8, Configuration::BACKGROUND_TEX_SIZE_X * Configuration::BACKGROUND_TEX_SIZE_Y * 4> pixels{};
    std::ranges::fill(pixels, 0 /*BLACK*/);

    // Create default black textures
    for(size_t i = 0; i < 4; i++)
    {
        textures_[i] = std::make_unique<sf::Texture>();
        textures_[i]->create(Configuration::BACKGROUND_TEX_SIZE_X, Configuration::BACKGROUND_TEX_SIZE_Y);
        textures_[i]->update(pixels.data());
    }

    // Load mask
    maskTexture_->loadFromFile("splash/splashMask.png");

    std::ranges::fill(texturesIntensities, 0.f);
    std::ranges::fill(texturesIntensitiesTargets, 1.f);


    // std::array<const char*, 5> files = {
    //     "splash/splash00.jpg",
    //     "splash/splash01.jpg",
    //     "splash/splash02.jpg",
    //     "splash/splash03.jpg",
    //     "splash/splashMask.png"
    // };

    // for(size_t i = 0; i < files.size(); i++)
    // {
    //     textures_[i] = std::make_unique<sf::Texture>();
    //     initializeTexture(textures_[i].get(), files[i]);
    // }

    // for(size_t i = 0; i < texturesBackBuffer_.size(); i++)
    // {
    //     texturesBackBuffer_[i] = std::make_unique<sf::Texture>();
    // }
}

void SplashNode::initializeShader()
{
    shader_->loadFromFile("splash/splash.frag", sf::Shader::Fragment);

    shader_->setUniform("mask_texture", *maskTexture_);
    
    shader_->setUniform("pan_velocity[0]", 0.05f);
    shader_->setUniform("pan_velocity[1]", 0.025f);
    shader_->setUniform("pan_velocity[2]", 0.07f);
    shader_->setUniform("pan_velocity[3]", 0.065f);

    shader_->setUniform("textures_intensity[0]", 1.f);
    shader_->setUniform("textures_intensity[1]", 1.f);
    shader_->setUniform("textures_intensity[2]", 1.f);
    shader_->setUniform("textures_intensity[3]", 1.f);

     shader_->setUniform("textures[0]", *textures_[0]);
     shader_->setUniform("textures[1]", *textures_[1]);
     shader_->setUniform("textures[2]", *textures_[2]);
     shader_->setUniform("textures[3]", *textures_[3]);
}

void SplashNode::initializeTexture(sf::Texture* texture, std::string_view textureName)
{
    texture->loadFromFile(textureName.data());
    texture->setSrgb(false);
    texture->setRepeated(true);
    texture->setSmooth(true);
}