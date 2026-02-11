#include <iostream>
#include <optional>
#include <thread>
#include <SFML/Graphics.hpp>
#include "../headers/game.h"
#include "../headers/render.h"

int main() {
  constexpr sf::ContextSettings settings;
  sf::Clock clock;

  sf::RenderWindow window(sf::VideoMode({512, 512}), "My first window", sf::State::Windowed, settings);
  window.setFramerateLimit(60);
  Game my_game{};
  my_game.start();


  while (window.isOpen()) 
  {
    window.clear(sf::Color{0x77, 0x6e, 0x65});
    // sf::Time elapsed = clock.restart();

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

    }

    my_game.pullInputs();
    renderGame(my_game.theBoard, window, my_game.getFont());

    window.display();
  }
  return 1;
}
