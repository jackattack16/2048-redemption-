#include <iostream>
#include <SFML/Graphics.hpp>
#include "../headers/game.h"

int main() {
  sf::ContextSettings settings;
  sf::Clock clock;

  sf::RenderWindow window(sf::VideoMode(512, 512), "My first window", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true); 	
  window.setFramerateLimit(60);
  Game my_game{};
  my_game.start();
  while (window.isOpen()) 
  {
    sf::Keyboard keyboard;
    window.clear();
    sf::Event e;
    // sf::Time elapsed = clock.restart();

    while (window.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        window.close();
      }
    }

    my_game.pullInputs(keyboard);
    my_game.updateLogic();
    

    window.display();
  }
  return 1;
}
