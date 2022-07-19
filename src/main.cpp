#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <sio_client.h>

#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

#include <widgets/QuadAspectRatio.h>




int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 580), "La Prision - Museo");
    //window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    QuadAspectRatio quadAspectRatio("checker640x480.png", QuadAspectRatio::EAspectRatioRule::FitToParent);

//    sio::client client;
//
//    client.set_open_listener([](){
//        std::cout << "Connected\n";
//    });
//    client.set_close_listener([](sio::client::close_reason const& reason){
//        std::cout << "Disconnected\n";
//    });
//    client.set_fail_listener([](){
//        std::cout << "Fail Connected\n";
//    });
//    client.connect("wss://testserv.prisonserver.net:5000");

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



        window.clear();
        window.draw(quadAspectRatio);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return EXIT_SUCCESS;
}
