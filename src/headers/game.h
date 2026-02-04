// Overall game class, handles inputs and sends actions to lower classes
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"
#ifndef GAME_H
#define GAME_H

class Game {

public:
  Game() {
    if ( !font.openFromFile("../clear-sans/ClearSans-Bold.ttf") ) {
      std::cerr << "Failed to load font (failed to create the font face)" << std::endl;
      std::cerr << "Provided path: \"Blue Winter.ttf\"" << std::endl;
    }
  }

  // Handles inputs and send it to the Board subclass
  void pullInputs() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !this->theBoard.isPreformingAction) {
      std::cout << "left" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !this->theBoard.isPreformingAction) {
      std::cout << "right" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !this->theBoard.isPreformingAction) {
      std::cout << "up" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !this->theBoard.isPreformingAction) {
      std::cout << "down" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 3;
    }
  }

  void start() { this->theBoard.generateStartingBoard(); }

  sf::Font &getFont() { return this->font; }

  Board theBoard;
  sf::Font font;

private:

};


#endif
