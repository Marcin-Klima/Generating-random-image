//PROGRAM TESTUJĄCY SERIALIZACJĘ KLASY, LUB STRUKTURY, DO PLIKU BINARNEGO

#include <iostream>
#include <fstream>
#include <random>
#include <SFML/Graphics.hpp>

using byte = unsigned char;

int main()
{
    const short screenWidth = 1024;
    const short screenHeight = 768;

    sf::RenderWindow * window = nullptr;
    byte randomByte;
    int selection;
    std::ifstream inputFile;
    std::ofstream outputFile;
    std::random_device rd;
    std::mt19937 randEng(rd());
    std::uniform_int_distribution<int> distribution(0, 255);

    std::cout << "1 - wygeneruj obraz z losowych pikseli, 2 - odczytaj obraz "
                 "z pliku";

    std::cin >> selection;
    switch(selection)
    {
        case 1:
            outputFile.open("test_file.w4", std::ios::binary);
            if(outputFile.is_open())
            {
                std::cout << "\n\nPozycja w pliku przed zapisem: "
                          << outputFile.tellp() << "\n";
                for(int i = 0; i < (screenWidth * screenHeight * 3); ++i)
                {
                    randomByte = distribution(randEng);
                    outputFile.write(reinterpret_cast<char *>(&randomByte), 1);
                }
                std::cout << "Pozycja w pliku po zapisie: "
                          << outputFile.tellp() << "\n";
            }
            break;
        case 2:
            inputFile.open("test_file.w4", std::ios::binary);
            if(inputFile.is_open())
            {
                inputFile.seekg(0, std::ios::end);
                int fileSize = inputFile.tellg();
                //byte * image = new byte[fileSize];
                inputFile.seekg(0, std::ios::beg);
                byte color[3];
                sf::Vertex singlePixel;

                std::vector<sf::VertexArray> image(screenHeight);
                for(int i = 0; i < screenWidth; ++i)
                {
                    image.emplace_back(sf::VertexArray());
                }
                for(int i = 0; i < screenHeight; ++i)
                {
                    for(int j = 0; j < screenWidth; ++j)
                    {
                        inputFile.read(reinterpret_cast<char *>(&color), 3);
                        singlePixel.color.r = color[0];
                        singlePixel.color.g = color[1];
                        singlePixel.color.b = color[2];
                        singlePixel.position.x = j;
                        singlePixel.position.y = i;
                        image[j].append(singlePixel);
                    }
                }

                window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight),
                        "Creative Coding");
                while (window->isOpen())
                {
                    sf::Event event;
                    while (window->pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                        {
                            window->close();
                        }
                    }
                    window->clear();
                    for(int i = 0; i < screenHeight; ++i)
                    {
                        for(int j = 0; j < screenWidth; ++j)
                        {
                            window->draw(image[i]);
                        }
                    }
                    window->display();
                }
            }
            break;
        default:

            break;
    }

    if(window != nullptr)
    {
        delete window;
    }

    return 0;
}