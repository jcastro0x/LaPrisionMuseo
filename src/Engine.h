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

#include <memory>

#include <widgets/Cursor.h>
#include <network/DebugNetwork.h>
#include <scene/Scene.h>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Resources.h>
#include <components/Internationalization.h>

class Engine
{
public:
    Engine();

public:
    void run();
    void stop();

    // template<typename T> requires std::is_base_of_v<class Scene, T>
    // void createScene()
    // {
    //     scene_ = std::make_unique<T>(this);
    // }

    void loadScene(std::string_view name);

public:
    [[nodiscard]] const Resources& getResources() const;
    [[nodiscard]] const Internationalization& getI18N() const;
    [[nodiscard]] Cursor& getCursor();
    [[nodiscard]] sf::Vector2i getMousePosition() const;
    [[nodiscard]] sf::Vector2u getWindowSize() const;

private:
    void processEvents();

    #ifndef NDEBUG
    void drawFPS(float deltaSeconds);
    #endif

private:
    std::unique_ptr<INetwork> network_  { std::make_unique<DebugNetwork>() };
    std::unique_ptr<Scene> scene_;
    sf::RenderWindow window_;
    std::unique_ptr<sf::Clock> clock_   { std::make_unique<sf::Clock>() };

    Cursor cursor_;
    std::unique_ptr<Resources> resources_;
    Internationalization internationalization_;
    std::unique_ptr<class SceneManager> sceneManager_;

    std::string scenePendingToLoad_;
};
