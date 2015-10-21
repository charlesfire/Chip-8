#include <iostream>
#include <bitset>
#include <SFML/Graphics.hpp>
#include "Chip8.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    Chip8 chip;
    if (argc == 2)
    {
        string path(argv[1]);
        chip.loadProgram(path);

        sf::RenderWindow window(sf::VideoMode(1024, 512), "Chip8");
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);

        sf::Clock clock;

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Num1:
                            chip.pressKey(0);
                            break;
                        case sf::Keyboard::Num2:
                            chip.pressKey(1);
                            break;
                        case sf::Keyboard::Num3:
                            chip.pressKey(2);
                            break;
                        case sf::Keyboard::Num4:
                            chip.pressKey(3);
                            break;
                        case sf::Keyboard::Q:
                            chip.pressKey(4);
                            break;
                        case sf::Keyboard::W:
                            chip.pressKey(5);
                            break;
                        case sf::Keyboard::E:
                            chip.pressKey(6);
                            break;
                        case sf::Keyboard::R:
                            chip.pressKey(7);
                            break;
                        case sf::Keyboard::A:
                            chip.pressKey(8);
                            break;
                        case sf::Keyboard::S:
                            chip.pressKey(9);
                            break;
                        case sf::Keyboard::D:
                            chip.pressKey(10);
                            break;
                        case sf::Keyboard::F:
                            chip.pressKey(11);
                            break;
                        case sf::Keyboard::Z:
                            chip.pressKey(12);
                            break;
                        case sf::Keyboard::X:
                            chip.pressKey(13);
                            break;
                        case sf::Keyboard::C:
                            chip.pressKey(14);
                            break;
                        case sf::Keyboard::V:
                            chip.pressKey(15);
                            break;
                    }
                }
                else if (event.type == sf::Event::KeyReleased)
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Num1:
                            chip.releaseKey(0);
                            break;
                        case sf::Keyboard::Num2:
                            chip.releaseKey(1);
                            break;
                        case sf::Keyboard::Num3:
                            chip.releaseKey(2);
                            break;
                        case sf::Keyboard::Num4:
                            chip.releaseKey(3);
                            break;
                        case sf::Keyboard::Q:
                            chip.releaseKey(4);
                            break;
                        case sf::Keyboard::W:
                            chip.releaseKey(5);
                            break;
                        case sf::Keyboard::E:
                            chip.releaseKey(6);
                            break;
                        case sf::Keyboard::R:
                            chip.releaseKey(7);
                            break;
                        case sf::Keyboard::A:
                            chip.releaseKey(8);
                            break;
                        case sf::Keyboard::S:
                            chip.releaseKey(9);
                            break;
                        case sf::Keyboard::D:
                            chip.releaseKey(10);
                            break;
                        case sf::Keyboard::F:
                            chip.releaseKey(11);
                            break;
                        case sf::Keyboard::Z:
                            chip.releaseKey(12);
                            break;
                        case sf::Keyboard::X:
                            chip.releaseKey(13);
                            break;
                        case sf::Keyboard::C:
                            chip.releaseKey(14);
                            break;
                        case sf::Keyboard::V:
                            chip.releaseKey(15);
                            break;
                    }
                }
            }

            window.clear(sf::Color::Black);

            chip.update(clock.restart().asMilliseconds());

            window.draw(chip);

            window.display();
        }
    }
    return 0;
}
