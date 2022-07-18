#include <SFML/Graphics.hpp>
#include <sio_client.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "La Prision - Museo");

    sf::Texture texture;
    texture.loadFromFile("AL_Almacen1_Mask.png");
    texture.setSmooth(false);
    texture.setSrgb(false);
    texture.setRepeated(false);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sio::client client;
    client.set_open_listener([](){

    });
    client.connect(" wss://testserv.prisonserver.net:5000");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
