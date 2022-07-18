#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <sio_client.h>

#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

#include <widgets/QuadAspectRatio.h>

float left;
float right;
float top;
float bottom;
/**
 [0]----[1]
  .      .
  .      .
 [3]----[2]
 */
void resize(sf::VertexArray& va, sf::Vector2u winSize, float aspectRatio)
{
    left   = 0.f;
    right  = static_cast<float>(winSize.y) * aspectRatio;
    top    = 0.f;
    bottom = static_cast<float>(winSize.y);



    //va[0].position = sf::Vector2f(left, top);
    //va[1].position = sf::Vector2f(right, top);
    //va[2].position = sf::Vector2f(right, bottom);
    //va[3].position = sf::Vector2f(left, bottom);

    va[0].position = sf::Vector2f(0,   0);
    va[1].position = sf::Vector2f(100, 0);
    va[2].position = sf::Vector2f(100, bottom);
    va[3].position = sf::Vector2f(0,   bottom);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "La Prision - Museo");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    QuadAspectRatio quadAspectRatio;

    sio::client client;

    client.set_open_listener([](){
        std::cout << "Connected\n";
    });
    client.set_close_listener([](sio::client::close_reason const& reason){
        std::cout << "Disconnected\n";
    });
    client.set_fail_listener([](){
        std::cout << "Fail Connected\n";
    });
    client.connect("wss://testserv.prisonserver.net:5000");

    /*
     Hi, if you want to use HTTPS you need to make sure that you built the TLS version of the
     socket.io C++ library. To do so you need to link against the OpenSSL libraries and pass the
     SIO_TLS preprocessor definition when compiling.
     */

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

                    sf::FloatRect visibleArea(
                        0.f,
                        0.f,
                        static_cast<float>(event.size.width),
                        static_cast<float>(event.size.height)
                    );
                    window.setView(sf::View(visibleArea));

                    //resize(background, sf::Vector2u(event.size.width, event.size.height), textureAspectRationWidth);
                    break;
            }

        }

        ImGui::SFML::Update(window, deltaClock.restart());

        //ImGui::ShowDemoWindow();

        ImGui::Begin("Redraw");
        ImGui::LabelText("Left",    "Left: %f",   left);
        ImGui::LabelText("Right",   "Right: %f",  right);
        ImGui::LabelText("Top",     "Top: %f",    top);
        ImGui::LabelText("Bottom",  "Bottom: %f", bottom);
        ImGui::Spacing();
        ImGui::LabelText("Window", "x: %d y: %d", window.getSize().x, window.getSize().y);
        ImGui::End();

        window.clear();
        window.draw(quadAspectRatio);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;
}
