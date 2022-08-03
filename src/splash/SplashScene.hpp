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

#include <scene/Scene.hpp>

#include <vector>

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
    class SplashScene : public Scene
    {
    public:
        SplashScene(class Engine* engine);
        ~SplashScene() override;

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void tick(float deltaTime) override;

    private:
        void initializeTextures();
        void initializeShader();

    private:
        std::unique_ptr<sf::Shader> shader_;
        std::unique_ptr<sf::RectangleShape> rectangleShape_;
        std::vector<std::unique_ptr<sf::Texture>> textures_;
    };
}
