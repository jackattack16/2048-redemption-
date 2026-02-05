// Holds all the information about the state of the board and the tiles. Takes inputs from the game class and validates them
#pragma once

#include <random>
#include <vector>
#include "tile.h"
#ifndef BOARD_H
#define BOARD_H

class Board {


public:
  static void moveTiles(int direction) {}

  void resetBoard() {
    // Reset all the tiles back to zero
    for ( const auto row: gameBoard ) {
      for ( int col = 0; col < 4; col++ ) {
        row[col].reset();
      }
    }
    this->generateStartingBoard();
  }

  bool isPreformingAction{};
  int movementDirection{}; // 0 - left, 1 - right, 2 - up, 3 - down, 4 - none
  bool isAnimating{};
  int animationFrame = 0;

  void endAnimation() {
    this->isAnimating = false;
    this->movementDirection = 4;
    this->animationFrame = 0;
    this->isPreformingAction = false;
  }

  void generateStartingBoard() { // Generates 2 tiles in random locations with 2/3 chance to be 2 and 1/3 to be 4
    // Generate one tile and set its location.
    const std::pair<int, int> firstLocation = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
    // gameBoard[firstLocation.first][firstLocation.second].createTile(Board::generateRandomNumber(2,4));

    // Generate another tile location and loop until it does not overlap with the first one.
    std::pair<int, int> secondLocation = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
    while ( gameBoard[secondLocation.first][secondLocation.second].isNumber() ) {
      secondLocation = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
    }

    // gameBoard[secondLocation.first][secondLocation.second].setValue(2);
    gameBoard[0][0].createTile(4);
    gameBoard[0][1].createTile(2);
    gameBoard[0][2].createTile(2);
    gameBoard[0][3].createTile(4);
    gameBoard[1][0].createTile(2);
    gameBoard[1][2].createTile(2);
    gameBoard[1][3].createTile(4);
  }


  static int generateRandomNumber(const int min, const int max) { // Generates a random integer given a min and a max
    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    const unsigned long output = dist(rng);
    return static_cast<int>(output);
  }

  void renderGame() const { // Prints the game to the console in a array form
    for ( const auto row: gameBoard ) {
      for ( int col = 0; col < 4; col++ ) {
        std::cout << row[col] << ", ";
      }
      std::cout << std::endl;
    }
  }

  [[nodiscard]] Tile getTile(const int row, const int col) const { return gameBoard[row][col]; }

  float moveAmounts[10] = {-18.0f, -14.0f, -10.0f, -6.0f, -3.0f, -1.5f, 1.0f, 0.6f, 0.3f, 0.1f};


  void moveTilesRight() {
    int rowIncrementer = 0;

    // Merge tiles first
    for ( const auto row: gameBoard ) {

      std::vector<Tile> numbersInRow; // Create a vector to store the tiles that are not zero
      for ( int col = 0; col < 4; col++ ) { // Loop through each column left to right
        if ( row[col].isNumber() ) { // Check if the tile has a value
          numbersInRow.push_back(row[col]); // Add that tile to a vector of pointers to the tiles
        }
        gameBoard[rowIncrementer][col].reset(); // Set the tile to zero
      }

      // Merge tiles in the numbersInRow vector
      std::vector<Tile> mergedNumbers;
      for ( int col = 0; col < numbersInRow.size(); col++ ) {// Loop through the vector
        try { // To protect from out of bounds checks at the end of vectors and arrays
          if ( numbersInRow.at(col).getValue() == numbersInRow.at(col + 1).getValue()) { // Check if the tile to the right is the same number
            Tile tempTile; // Create a temporary tile to push to the vector
            tempTile.setValue(numbersInRow.at(col).getValue() + numbersInRow.at(col+ 1).getValue()); // Calculate the new value
            mergedNumbers.push_back(tempTile);
            col++; // Skip the next tile
          } else {
            mergedNumbers.push_back(numbersInRow.at(col)); // Add in the tile if it does not merge
          }
        } catch (std::out_of_range &e) {
          std::cout << "end of row reached" << std::endl;
          mergedNumbers.push_back(numbersInRow.at(col)); // Add in the tile at the end of the row
        }
      }

      int startingCol = 4 - mergedNumbers.size(); // The starting column to put the value back in to the array
      for (const auto& tile : mergedNumbers) { // Loop through all the numbers that are going to be put into the final board
        gameBoard[rowIncrementer][startingCol] = tile; // Set the value of the tile in the actual board
        startingCol++; // Change the column index
      }
      rowIncrementer++;
    }
  }

private:
  Tile gameBoard[4][4];
};

#endif
