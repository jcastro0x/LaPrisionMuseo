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

#include <scene/SceneNode.h>

#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


class LoginSceneNode : public SceneNode
{
public:
    LoginSceneNode();
    ~LoginSceneNode() override;
    
protected:
    void init() override;
    void tick(float deltaTime) override;
    void destroy() override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
    std::unique_ptr<class QuadAspectRatio> background_;


    sf::Sprite sprite;
    sf::Texture texture;



    enum class EAspectRatioRule : uint8_t
    {
        Width,
        FitToParent
    };
    float aspectRatio_ = 640.f/480.f;
    EAspectRatioRule aspectRatioRule_ = EAspectRatioRule::FitToParent;
    struct QuadSize
    {
        float texWidth;
        float texHeight;
        float heightGap;
        float widthGap;
    };
    QuadSize getQuadSize(sf::RenderTarget& target) const;
};
