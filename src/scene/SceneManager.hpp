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
#include "Scene.hpp"

#include <cassert>

namespace lpm
{
    class scene_exception final : public std::exception { };

    class SceneManager
    {
        using ScenePtr      = std::unique_ptr<Scene>;
        using SceneRegister = std::unordered_map<std::string, std::function<ScenePtr()>>;

    public:
        template<typename SceneType> requires std::is_base_of_v<class Scene, SceneType>
        static void registerScene(std::string_view name, class Engine* engine)
        {
            assert(!std::ranges::any_of(name.begin(), name.end(), ::isupper) && "registerScene only accepts lower strings");
            assert(!scenes_.contains(name.data()) && "trying to registerScene with name already registered");

            scenes_.try_emplace(name.data(), [engine](){
                return std::make_unique<SceneType>(engine);
            });
        }

        static std::function<ScenePtr()> findScene(std::string_view name)
        {
            assert(!std::ranges::any_of(name.begin(), name.end(), ::isupper) && "findScene only accepts lower strings");
            if(!scenes_.contains(name.data()))
                throw scene_exception();

            return scenes_[name.data()];
        }

    private:
        inline static SceneRegister scenes_;
    };
}
