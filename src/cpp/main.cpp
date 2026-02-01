#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>
#include "../headers/game.h"
#include "../headers/render.h"

int main() {
  sf::ContextSettings settings;
  sf::Clock clock;

  sf::RenderWindow window(sf::VideoMode({512, 512}), "My first window", sf::State::Windowed, settings);
  window.setVerticalSyncEnabled(true); 	
  window.setFramerateLimit(60);
  Game my_game{};
  my_game.start();
  while (window.isOpen()) 
  {
    window.clear();
    // sf::Time elapsed = clock.restart();

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    my_game.pullInputs();
    my_game.updateLogic();
    renderGame(my_game.getBoard(), window, my_game.getFont());

    window.display();
  }
  return 1;
}
