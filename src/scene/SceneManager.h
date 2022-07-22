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

#include <unordered_map>
#include <string>
#include <functional>
#include "Scene.h"

class scene_exception final : public std::exception
{
};

class SceneManager
{
    using ScenePtr = std::unique_ptr<Scene>;

public:
    template<typename SceneType> requires std::is_base_of_v<class Scene, SceneType>
    void registerScene(std::string_view name)
    {
        scenes_.insert({name, []() -> ScenePtr {
            return std::make_unique<SceneType>();
        }});
    }

    ScenePtr findScene(std::string_view name)
    {
        if(!scenes_.contains(name.data()))
            throw scene_exception();

        return scenes_[name.data()]();
    }

private:
    std::unordered_map<std::string, std::function<ScenePtr()>> scenes_;
};
