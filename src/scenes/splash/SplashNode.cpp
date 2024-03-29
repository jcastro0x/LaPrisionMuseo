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
, topMaskTexture_(std::make_unique<sf::Texture>())
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
    setTextureIntensityTarget(index, 0.f, [this, index, textureName = std::string(textureName.data())](){
        initializeTexture(textures_[index].get(), textureName);
        setTextureIntensityTarget(index, 1.f);
    });
}



void SplashNode::tick(float deltaTime)
{
    SceneNode::tick(deltaTime);

    static float totalTime = 1988;
    totalTime += deltaTime;
    shader_->setUniform("time", totalTime);

    for(size_t i = 0; i < texturesIntensities.size(); i++)
    {
        auto& intensity = texturesIntensities[i];

        float direction = 0;
        if(intensity != texturesIntensitiesTargets[i])
        {
            direction = intensity > texturesIntensitiesTargets[i]
                      ? -1.f
                      : +1.f;
        }

        intensity = std::clamp(intensity + deltaTime * texturesIntensitiesVelocity * direction, 0.f, 1.f);

        static constexpr float ACCEPTANCE = 0.01f;
        if(std::abs(intensity - texturesIntensitiesTargets[i]) < ACCEPTANCE && texturesIntensitiesCallbacks[i])
        {
            texturesIntensitiesCallbacks[i]();
            texturesIntensitiesCallbacks[i] = {};
        }
    }

    shader_->setUniform("textures_intensity[0]", texturesIntensities[0]);
    shader_->setUniform("textures_intensity[1]", texturesIntensities[1]);
    shader_->setUniform("textures_intensity[2]", texturesIntensities[2]);
    shader_->setUniform("textures_intensity[3]", texturesIntensities[3]);
}

void SplashNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.shader = shader_.get();
    target.draw(*rectangleShape_, states);
}

void SplashNode::initializeTextures()
{
    // Create default textures
    for(size_t i = 0; i < 4; i++)
    {
        textures_[i] = std::make_unique<sf::Texture>();
        textures_[i]->create(Configuration::BACKGROUND_TEX_SIZE_X, Configuration::BACKGROUND_TEX_SIZE_Y);
    }

    // Load mask
    maskTexture_->loadFromFile("splash/splashMask.png");

    topMaskTexture_->loadFromFile("splash/topmask.png");

    std::ranges::fill(texturesIntensities, 0.f);
    std::ranges::fill(texturesIntensitiesTargets, 1.f);
}

void SplashNode::initializeShader()
{
    shader_->loadFromFile("splash/splash.frag", sf::Shader::Fragment);

    shader_->setUniform("mask_texture", *maskTexture_);
    shader_->setUniform("top_mask_texture", *topMaskTexture_);

    static constexpr auto quarter = 1 - 1/4.f;
    //static constexpr auto fade    = 1 - 1/40.f;

    // Vector3(offset, limit, velocity)
    shader_->setUniform("displacement[0]", sf::Vector3f(0,       quarter,  .10f));
    shader_->setUniform("displacement[1]", sf::Vector3f(-1/4.f,  quarter,  .07f));
    shader_->setUniform("displacement[2]", sf::Vector3f(-2/4.f,  quarter,  .09f));
    shader_->setUniform("displacement[3]", sf::Vector3f(-3/4.f,  quarter,  .06f));

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


void SplashNode::setTextureIntensityTarget(size_t index, float intensity, std::function<void()> const& callback)
{
    texturesIntensitiesTargets[index]   = intensity;
    texturesIntensitiesCallbacks[index] = callback;
}
