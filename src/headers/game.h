// Overall game class, handles inputs and sends actions to lower classes
#pragma once

#include "board.h"
#ifndef GAME_H
#define GAME_H

class Game {

  public:
    Game() {
      isAnimating = false;
      movementDirection = 0;
      isPreformingAction = false;
    }

    // Handles inputs and send it to the Board subclass
    void pullInputs(sf::Keyboard keyboard) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !this->isPreformingAction) {
        std::cout << "left" << std::endl;
        this->isPreformingAction = true;
        this->movementDirection = 0;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !this->isPreformingAction) {
        std::cout << "right" << std::endl;
        this->isPreformingAction = true;
        this->movementDirection = 1;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !this->isPreformingAction) {
        std::cout << "up" << std::endl;
        this->isPreformingAction = true;
        this->movementDirection = 2;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !this->isPreformingAction) {
        std::cout << "down" << std::endl;
        this->isPreformingAction = true;
        this->movementDirection = 3;
      }
    }

    void updateLogic() const {
      if (!this->isAnimating) {
        Board::moveTiles(this->movementDirection);
      }
    }

    void start() {
      this->board.generateStartingBoard();
    }


  private:
    bool isPreformingAction;
    int movementDirection; //0 - left, 1 - right, 2 - up, 3 - down
    bool isAnimating;
    Board board; 
    
};


#endif