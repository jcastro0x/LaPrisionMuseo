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

#pragma once

#include <scene/SceneNode.hpp>
#include <memory>
#include <array>
#include <functional>

namespace sf
{
    class Shader;
    class Texture;
    class RectangleShape;

    class RenderTarget;
    class RenderStates;
}

namespace lpm
{
    /**
     * @brief Child node of SplashScene to draw splash.
     * 
     * SplashNode draw 4 differente textures with padding effect and blended together 
     * with mask texture. Can change textures at fly with fade-in and fade-out effect.
     */
    class SplashNode : public SceneNode
    {
    public:
        static constexpr size_t TEXTURE_0 = 0;
        static constexpr size_t TEXTURE_1 = 1;
        static constexpr size_t TEXTURE_2 = 2;
        static constexpr size_t TEXTURE_3 = 3;

    public:
        SplashNode();
        ~SplashNode();

    protected:
        void init() override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void tick(float deltaTime) override;

    private:
        void initializeShader();


    private:
        sf::Shader* shader_ = nullptr;
        std::unique_ptr<sf::RectangleShape> rectangleShape_;
        sf::Texture* maskTexture_ = nullptr;
        sf::Texture* topMaskTexture_ = nullptr;

        float texturesIntensitiesVelocity = 1.95f;
        std::array<sf::Texture*, 4> textures_;
        std::array<float, 4> texturesIntensities;
        std::array<float, 4> texturesIntensitiesTargets;
        std::array<std::function<void()>, 4> texturesIntensitiesCallbacks;


    };
}
