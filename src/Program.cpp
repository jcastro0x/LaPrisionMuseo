#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <scene/SceneFactory.hpp>
#include <scene/SceneManager.hpp>

#include <Configuration.hpp>

#include <login/LoginScene.hpp>
#include <world/WorldScene.hpp>

#include <Engine.hpp>

#include <iostream>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

Engine::Engine()
: window_(sf::VideoMode(Configuration::WINDOW_SIZE_X, Configuration::WINDOW_SIZE_Y), Configuration::WINWDOW_TITLE)
, sceneManager_(std::make_unique<SceneManager>())
{
    window_.setFramerateLimit(Configuration::FRAME_RATE);
    window_.setMouseCursorVisible(false);

    gui_ = std::make_unique<tgui::Gui>(window_);


    // TODO: Create macro to automatize this
    sceneManager_->registerScene<LoginScene>("LoginScene", this);
    sceneManager_->registerScene<WorldScene>("WorldScene", this);

    try
    {
        resources_ = std::make_unique<Resources>();
    }
    catch(resource_exception const&)
    {
        std::cerr << "FATAL ERROR reading resources\n";
        throw;
    }

    ImGui::SFML::Init(window_);
    ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_::ImGuiConfigFlags_NoMouseCursorChange;
}

void Engine::run()
{
    //createScene<LoginScene>();
    loadScene("LoginScene");

    sf::Event event {};

    clock_->restart();
    while (window_.isOpen())
    {
        const auto time = clock_->restart();
        
        processEvents(event);
        gui_->handleEvent(event);
        ImGui::SFML::Update(window_, time);

        scene_->tick(time.asSeconds());
        cursor_.tick(time.asSeconds(), window_);

        #ifndef NDEBUG
        drawFPS(time.asSeconds());
        #endif

        //ImGui::ShowDemoWindow();

        window_.clear();
        window_.draw(*scene_);
        gui_->draw();
        ImGui::SFML::Render(window_);
        window_.draw(cursor_);

        window_.display();

        if(scene_->isPendingToDestroy())
        {
            scene_.release();
            if(!scenePendingToLoad_.empty())
            {
                try
                {
                    scene_ = sceneManager_->findScene(scenePendingToLoad_);
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
        scenePendingToLoad_ = name.data();
    }
    else
    {
        try
        {
            scene_ = sceneManager_->findScene(name);
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

const Resources& Engine::getResources() const
{
    return *resources_;
}

const Internationalization& Engine::getI18N() const
{
    return internationalization_;
}

Cursor& Engine::getCursor()
{
    return cursor_;
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

extern "C" void handle_aborts(int signal_number)
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

int main(const int, const char**)
{
    signal(SIGILL,   &handle_aborts);
    signal(SIGFPE,   &handle_aborts);
    signal(SIGSEGV,  &handle_aborts);
    signal(SIGTERM,  &handle_aborts);
    signal(SIGABRT,  &handle_aborts);

    #ifdef _WIN32
    signal(SIGBREAK, &handle_aborts);
    #endif


    Engine engine;
    engine.run();

    return EXIT_SUCCESS;

    #if 0
    sf::RenderWindow window(sf::VideoMode(640, 580), "La Prision - Museo");
    window.setFramerateLimit(60);
    
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    QuadAspectRatio quadAspectRatio("AL_Almacen1.jpg", QuadAspectRatio::EAspectRatioRule::FitToParent);

    sf::Texture maskTex;
    maskTex.loadFromFile("AL_Almacen1_Mask.png");
    maskTex.setRepeated(false);
    maskTex.setSrgb(false);
    maskTex.setSmooth(false);
    auto maskImg = maskTex.copyToImage();

    sf::Texture maskTexDbg;
    maskTexDbg.create(64, 64);
    sf::Sprite maskSprDbg(maskTexDbg);




    sf::Event event {};
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            switch (event.type) {
                default:break;

                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:

                    event.size.width = std::max(640u, event.size.width);
                    event.size.height = std::max(480u, event.size.height);
                    window.setSize({event.size.width, event.size.height});

                    window.setView(sf::View({
                        0.f,
                        0.f,
                        static_cast<float>(event.size.width),
                        static_cast<float>(event.size.height)
                    }));
                    break;
            }

        }

        ImGui::SFML::Update(window, deltaClock.restart());

        //ImGui::ShowDemoWindow();

        auto coord = quadAspectRatio.transformPointToTextureCoords(window, sf::Mouse::getPosition(window));
        sf::Color c = sf::Color::Transparent;
        if(coord)
        {
            c = maskImg.getPixel(coord->x, coord->y);
        }


        ImGui::Begin("Mask color");
        ImGui::ImageButton(maskSprDbg, 0, c);
        ImGui::LabelText("Mask" ,  "x: %d y: %d", maskImg.getSize().x, maskImg.getSize().y);
        if(coord)
        {
            ImGui::LabelText("Coord" , "x: %d y: %d", coord->x, coord->y);
        }
        ImGui::End();


        window.clear();
        window.draw(quadAspectRatio);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;
    #endif
}
