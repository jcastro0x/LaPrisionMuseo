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

#include "Animator.h"

#include <fstream>
#include <nlohmann/json.hpp>

void Animator::tick(float deltaTime)
{
    currentTime_ += deltaTime;
}

void Animator::loadAnimations(std::string_view fileName)
{
    std::ifstream f(fileName.data());
    auto json = nlohmann::json::parse(f);
    for(auto anim : json)
    {
        Animation animation;
        animation.name = anim["name"];
        animation.rate = anim["rate"];
        for(auto frame : anim["frames"])
        {
            animation.frames.emplace_back(sf::IntRect{
                frame["x"],
                frame["y"],
                frame["w"],
                frame["h"]
            });
        }

        animations_.emplace_back(animation);
    }
}

sf::IntRect Animator::getCurrentRect(std::string_view animation) const
{
    if(auto it = std::find_if(animations_.begin(), animations_.end(), [&](auto& anim){
        return anim.name == animation.data();
    }); it != animations_.end())
    {
        return (*it).frames[static_cast<size_t>(std::fmod((currentTime_ / (*it).rate), (*it).frames.size()))];
    }

    return {};
}

const std::vector<Animator::Animation>& Animator::getAnimations() const
{
    return animations_;
}