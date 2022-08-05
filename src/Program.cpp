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

#include <Engine.hpp>

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <widgets/Cursor.hpp>
#include <network/DebugNetwork.hpp>

#include <system/Internationalization.hpp>
#include <system/ResourcesManager.hpp>
#include <system/Configuration.hpp>

#include <scene/SceneManager.hpp>
#include <scenes/login/LoginScene.hpp>
#include <scenes/world/WorldScene.hpp>
#include <scenes/splash/SplashScene.hpp>

using namespace lpm;



Engine::Engine()
: window_(sf::VideoMode(Configuration::WINDOW_SIZE_X, Configuration::WINDOW_SIZE_Y), Configuration::WINWDOW_TITLE)
, network_(std::make_unique<DebugNetwork>())
, clock_(std::make_unique<sf::Clock>())
, cursor_(std::make_unique<Cursor>())
, internationalization_(std::make_unique<Internationalization>())
, gui_(std::make_unique<tgui::Gui>())

{
    window_.setFramerateLimit(Configuration::FRAME_RATE);
    window_.setMouseCursorVisible(false);

    std::bit_cast<tgui::Gui*>(gui_.get())->setWindow(window_);

    //~===================================================================
    // REGISTER SCENES
    SceneManager::registerScene<LoginScene>("login", this);
    SceneManager::registerScene<WorldScene>("world", this);
    SceneManager::registerScene<SplashScene>("splash", this);
    //~===================================================================

    ImGui::SFML::Init(window_);
    ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_::ImGuiConfigFlags_NoMouseCursorChange;
}

Engine::~Engine() = default;


void Engine::run()
{
    loadScene("splash");

    auto menu = tgui::MenuBar::create();
    menu->setHeight(22.f);
    menu->addMenu("File");
    menu->addMenuItem("Load");
    menu->addMenuItem("Save");
    menu->addMenuItem("Exit");
    menu->addMenu("Edit");
    menu->addMenuItem("Copy");
    menu->addMenuItem("Paste");
    menu->addMenu("Help");
    menu->addMenuItem("About");
    gui_->add(menu);

    menu->connectMenuItem("File", "Exit", [&](){
        window_.close();
    });


    sf::Event event {};

    clock_->restart();
    while (window_.isOpen())
    {
        const auto time = clock_->restart();
        
        processEvents(event);
        std::bit_cast<tgui::Gui*>(gui_.get())->handleEvent(event);

        ImGui::SFML::Update(window_, time);

        scene_->tick(time.asSeconds());
        getCursor().tick(time.asSeconds(), window_);

        #ifndef NDEBUG
        drawFPS(time.asSeconds());
        #endif

        //ImGui::ShowDemoWindow();

        window_.clear();
        window_.draw(*scene_);
        gui_->draw();
        ImGui::SFML::Render(window_);
        window_.draw(getCursor());

        window_.display();

        if(scene_->isPendingToDestroy())
        {
            (void)scene_.release();
            if(!scenePendingToLoad_.empty())
            {
                try
                {
                    scene_ = SceneManager::findScene(scenePendingToLoad_)();
                    scenePendingToLoad_.clear();
                }
                catch(const scene_exception&)
                {
                    std::cerr << "Can't load scene \042" << scenePendingToLoad_ << "\042\n";
                }
            }
        }
    }

    ImGui::SFML::Shutdown();
}

void Engine::stop()
{
    window_.close();
}

void Engine::loadScene(std::string_view name)
{
    if(scene_)
    {
        scene_->destroy();
        scenePendingToLoad_ = std::string(name.data());
    }
    else
    {
        try
        {
            scene_ = SceneManager::findScene(name)();
        }
        catch(const scene_exception&)
        {
            std::cerr << "Can't load scene \042" << name << "\042\n";
        }
    }
}

void Engine::processEvents(sf::Event& event)
{
    while (window_.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(window_, event);

        switch (event.type) 
        {
            default:break;

            case sf::Event::Closed:
                window_.close();
                break;

            case sf::Event::Resized:
                window_.setView(sf::View({
                    0.f,
                    0.f,
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height)
                }));
                break;
        }
    }
}

#ifndef NDEBUG
void Engine::drawFPS(float deltaSeconds)
{
    ImGui::Begin("Debug - FPS");
    ImGui::LabelText("FPS", "%d", static_cast<unsigned>(1.f / deltaSeconds));
    ImGui::End();
}
#endif

const Internationalization& Engine::getI18N() const
{
    return *internationalization_;
}

Cursor& Engine::getCursor()
{
    return *cursor_;
}

sf::Vector2i Engine::getMousePosition() const
{
    return sf::Mouse::getPosition(window_);
}

sf::Vector2u Engine::getWindowSize() const
{
    return window_.getSize();
}







#include <csignal>

extern "C" void handle_signals(int signal_number)
{
    std::string signType;
    switch (signal_number) {
        case SIGILL:   signType = "SIGILL: illegal instruction - invalid function image ";  break;
        case SIGFPE:   signType = "SIGFPE: floating point exception ";                      break;
        case SIGSEGV:  signType = "SIGSEGV: segment violation ";                            break;
        case SIGTERM:  signType = "SIGTERM: Software termination signal from kill ";        break;
        case SIGABRT:  signType = "SIGABRT: abnormal termination triggered by abort call "; break;

        #ifdef _WIN32
        case SIGBREAK: signType = "SIGBREAK: Ctrl-Break sequence ";                         break;
        #endif

        default:       signType = "UNKNOWN";                                                break;
    }

    std::cerr << "Ungracefully exit: " << signType << '(' << signal_number << ')' << '\n';
}

#include <system/BinaryFormatter.hpp>
int main(const int, const char**)
{
    BinaryFormatter fmt;
    fmt.createFile("splash", 0);
    return 0;

    signal(SIGILL,   &handle_signals);
    signal(SIGFPE,   &handle_signals);
    signal(SIGSEGV,  &handle_signals);
    signal(SIGTERM,  &handle_signals);
    signal(SIGABRT,  &handle_signals);

    #ifdef _WIN32
    signal(SIGBREAK, &handle_signals);
    #endif


    Engine engine;
    engine.run();

    return EXIT_SUCCESS;
}
