// Holds all the information about the state of the board and the tiles. Takes inputs from the game class and validates them
#pragma once

#include "tile.h"
#include <random>
#ifndef BOARD_H
#define BOARD_H

class Board {
  public:
    static void moveTiles(int direction) {

    }

    void resetBoard() {
      // Reset all the tiles back to zero
      for (const auto row : gameBoard) {
        for (int col = 0; col < 4; col++) {
          row[col].reset();
        }
      }
      this->generateStartingBoard();
    }

    void generateStartingBoard() { // Generates 2 tiles in random locations with 2/3 chance to be 2 and 1/3 to be 4
      // Generate one tile and set its location.
      const std::pair<int, int> firstLocation = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
      gameBoard[firstLocation.first][firstLocation.second].createTile(Board::generateRandomNumber(2,4));
      
      // Generate another tile location and loop until it does not overlap with the first one.
      std::pair<int, int> secondLocation = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
      while(gameBoard[secondLocation.first][secondLocation.second].isNumber()) {
        secondLocation = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
      }

      gameBoard[secondLocation.first][secondLocation.second].setValue(2);
    }
    

    static int generateRandomNumber(const int min, const int max) { // Generates a random integer given a min and a max
      static std::random_device dev;
      static std::mt19937 rng(dev());
      std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
      const unsigned long output =  dist(rng);
      return static_cast<int>(output);
    }

    void renderGame() const { // Prints the game to the console in a array form
      for (const auto row : gameBoard) {
        for (int col = 0; col < 4; col++) {
          std::cout << row[col] << ", ";
        }
        std::cout << std::endl;
      }
    }


  private:
    Tile gameBoard[4][4];

};

#endif 