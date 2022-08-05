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
#include <SFML/Graphics/RenderWindow.hpp>

namespace tgui  { class BackendGui; }
namespace sf    { class Clock;      }

namespace lpm
{
    class Internationalization;
    class Cursor;
    class SceneManager;
    class INetwork;
    class Scene;

    class Engine final
    {
        template<typename T>
        using Pointer = std::unique_ptr<T>;

    public:
        Engine();
        ~Engine();

    public:
        void run();
        void stop();

        void loadScene(std::string_view name);

    public:
        [[nodiscard]] const Internationalization& getI18N() const;
        [[nodiscard]] Cursor& getCursor();
        [[nodiscard]] sf::Vector2i getMousePosition() const;
        [[nodiscard]] sf::Vector2u getWindowSize() const;

    private:
        void processEvents(sf::Event& event);

        #ifndef NDEBUG
        void drawFPS(float deltaSeconds);
        #endif

    private:
        sf::RenderWindow window_;                               //< SFML class to draw OS window

        Pointer<INetwork> network_;                             //< Network interface
        Pointer<sf::Clock> clock_;                              //< SFML clock
        Pointer<Cursor> cursor_;                                //< Cursor class
        Pointer<Internationalization> internationalization_;    //< i18n pointer
        Pointer<tgui::BackendGui> gui_;                         //< TGUI pointer
        Pointer<Scene> scene_;                                  //< Current scene drawn

        std::string scenePendingToLoad_;                        //< Pending scene to load
    };
}
