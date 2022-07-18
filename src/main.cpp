#include <SFML/Graphics.hpp>
#include <sio_client.h>

#include <iostream>


/**
 [0]----[1]
  .      .
  .      .
 [3]----[2]
 */
void resize(sf::VertexArray& va, sf::Vector2i winSize, float aspectRatio)
{
    const auto left   = 0.f;
    const auto right  = static_cast<float>(winSize.y) * aspectRatio;
    const auto top    = 0.f;
    const auto bottom = static_cast<float>(winSize.y);

    va[0].position = sf::Vector2f(left, top);
    va[1].position = sf::Vector2f(right, top);
    va[2].position = sf::Vector2f(right, bottom);
    va[3].position = sf::Vector2f(left, bottom);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "La Prision - Museo");

    sf::Texture texture;
    texture.loadFromFile("AL_Almacen1_Mask.png");
    texture.setSmooth(false);
    texture.setSrgb(false);
    texture.setRepeated(false);

    const auto textureAspectRationWidth = static_cast<float>(texture.getSize().x)
                                        / static_cast<float>(texture.getSize().y);

    std::cout << "textureAspectRationWidth: " << textureAspectRationWidth << "\n";
    std::cout << "texture.width: "  << texture.getSize().x << "\n";
    std::cout << "texture.height: " << texture.getSize().y << "\n";
    std::cout << "window.height: "  << window.getSize().y << "\n";
    std::cout << "window.width: "   << window.getSize().x << "\n";


    sf::VertexArray background(sf::Quads, 4);
    background[0].position = sf::Vector2f(0, 0);
    background[1].position = sf::Vector2f(window.getSize().x, 0);
    background[2].position = sf::Vector2f(window.getSize().x, window.getSize().y);
    background[3].position = sf::Vector2f(0, window.getSize().y);

    background[0].color = sf::Color::Red;
    background[1].color = sf::Color::Green;
    background[2].color = sf::Color::Blue;
    background[3].color = sf::Color::Yellow;


    sf::Event event {};
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type) {
                default:break;

                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::Resized:
                    resize(background, sf::Vector2i(event.size.width, event.size.height), textureAspectRationWidth);
                    break;
            }

        }

        window.clear();
        window.draw(background);
        window.display();
    }

    return 0;
}
