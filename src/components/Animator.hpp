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

#include <string>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

namespace lpm
{
    class Animator
    {
    public:
        struct Animation
        {
            std::string name;
            float rate = 0;
            std::vector<sf::IntRect> frames;
        };

    public:
        void tick(float deltaTime);
        void loadAnimations(std::string_view json);

        sf::IntRect getCurrentRect(std::string_view animation) const;


        const std::vector<Animation>& getAnimations() const;

    private:
        float currentTime_ = 0;
        std::vector<Animation> animations_;
    };
}
