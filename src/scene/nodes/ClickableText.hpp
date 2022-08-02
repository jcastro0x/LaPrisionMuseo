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
#include <functional>

#include <SFML/System/String.hpp>

namespace sf
{
    class Text;
    class Sound;
}

namespace lpm
{
    class ClickableText final : public SceneNode
    {
    public:
        ClickableText(const sf::String& string, class Scene* owner);
        ~ClickableText() override;

    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void tick(float deltaTime) override;

        void bindOnClick(const std::function<void()>& onClicked);

    protected:
        void onMouseEnter() const;
        void onMouseExit() const;
        void onMouseClickDown() const;
        void onMouseClickUp() const;

    private:
        std::unique_ptr<sf::Text> text_;
        std::unique_ptr<sf::Sound> soundHover_;
        std::unique_ptr<sf::Sound> soundClick_;

        mutable bool bMouseEnter_     = false;
        mutable bool bMouseClickDown_ = false;

        std::function<void()> onClicked_;
    };
}
