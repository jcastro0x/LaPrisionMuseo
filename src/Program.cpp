#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>


#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

#include <widgets/QuadAspectRatio.h>
#include <widgets/Cursor.h>


#include <network/DebugNetwork.h>

#include <scene/Scene.h>
#include <scene/SceneFactory.h>

#include <login/LoginSceneNode.h>

#include <memory>

#define DEFAULT_WINDOW_SIZE_X 640u
#define DEFAULT_WINDOW_SIZE_Y 480u



class Engine
{
public:
    Engine();

    void run();
    void destroy();

private:
    void processEvents();
    void drawFPS(float deltaSeconds);

private:
    std::unique_ptr<INetwork> network_;
    int binaryReader_;
    Cursor cursor_;
    std::unique_ptr<Scene> scene_;
    sf::RenderWindow window_;
    std::unique_ptr<sf::Clock> clock_;
};


Engine::Engine()
: network_(std::make_unique<DebugNetwork>())
, binaryReader_(NULL)
, scene_(std::make_unique<Scene>())
, window_(sf::VideoMode(DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y), "La Prision - Museo")
, clock_(std::make_unique<sf::Clock>())
{
    window_.setFramerateLimit(30);

    ImGui::SFML::Init(window_);
    ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_::ImGuiConfigFlags_NoMouseCursorChange;

    window_.setMouseCursorVisible(false);
}

void Engine::run()
{
    auto loginSceneNode = SceneFactory::createScene<LoginSceneNode>(scene_.get());
    scene_->addSceneNode(std::move(loginSceneNode));

    clock_->restart();
    while (window_.isOpen())
    {
        
        const auto time = clock_->restart();
        
        processEvents();
        ImGui::SFML::Update(window_, time);

        window_.clear();
        if(scene_)
        {
            scene_->tick(time.asSeconds());
        }
        
        #ifndef NDEBUG
        drawFPS(time.asSeconds());
        #endif

        //ImGui::ShowDemoWindow();
        
        ImGui::SFML::Render(window_);

        const float mouseX = static_cast<float>(sf::Mouse::getPosition(window_).x);
        const float mouseY = static_cast<float>(sf::Mouse::getPosition(window_).y);
        cursor_.setPosition(mouseX, mouseY);
        cursor_.tick(time.asSeconds());

        window_.draw(cursor_);
        
        window_.display();
    }

    if(scene_)
        scene_->destroy();
    
    ImGui::SFML::Shutdown();
}

void Engine::processEvents()
{
    sf::Event event {};
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
                //event.size.width = std::max(DEFAULT_WINDOW_SIZE_X, event.size.width);
                //event.size.height = std::max(DEFAULT_WINDOW_SIZE_X, event.size.height);
                //window_.setSize({event.size.width, event.size.height});

                // Take account that some desktop envs don't resize as requested
                // for example tilled envs like i3.
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

void Engine::drawFPS(float deltaSeconds)
{
    #ifndef NDEBUG
    ImGui::Begin("Debug - FPS");
    ImGui::LabelText("FPS", "%d", static_cast<unsigned>(1.f / deltaSeconds));
    ImGui::End();
    #endif
}

int main(const int, const char**)
{
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
