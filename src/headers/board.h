// Holds all the information about the state of the board and the tiles. Takes inputs from the game class and validates them
#pragma once

#include <algorithm>
#include <random>
#include <vector>
#include "tile.h"
#ifndef BOARD_H
#define BOARD_H

class Board {


public:

  void resetBoard() {
    // Reset all the tiles back to zero
    for ( auto &row: gameBoard ) {
      for ( int col = 0; col < 4; col++ ) {
        row[col].reset();
      }
    }
    this->generateStartingBoard();
  }

  bool isPreformingAction{};
  bool isAnimating{};
  int animationFrame = 0;
  int movementDirection;

  void endAnimation() {
    this->isAnimating = false;
    this->animationFrame = 0;
    this->isPreformingAction = false;
  }

  void generateStartingBoard() { // Generates 2 tiles in random locations with 2/3 chance to be 2 and 1/3 to be 4
    std::pair<int, int> location = generateRandomLocation();
    int startingValue = generateRandomNumber(2, 4);
    startingValue = startingValue == 3 ? 2 : startingValue; // Turns the 3 into a 2;
    gameBoard[location.first][location.second].setValue(startingValue);

    location = generateRandomLocation();
    startingValue = generateRandomNumber(2, 4);
    startingValue = startingValue == 3 ? 2 : startingValue;
    gameBoard[location.first][location.second].setValue(startingValue);
  }


  std::pair<int, int> generateRandomLocation() const { // Generates a unique location
    std::pair<int, int> location = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
    while ( gameBoard[location.first][location.second].isNumber() ) {
      location = {Board::generateRandomNumber(0, 3), Board::generateRandomNumber(0, 3)};
    }
    return location;
  }


  static int generateRandomNumber(const int min, const int max) { // Generates a random integer given a min and a max
    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    const unsigned long output = dist(rng);
    return static_cast<int>(output);
  }

  void renderGame() const { // Prints the game to the console in a array form
    for ( const auto &row: gameBoard ) {
      for ( int col = 0; col < 4; col++ ) {
        std::cout << row[col] << ", ";
      }
      std::cout << std::endl;
    }
  }

  [[nodiscard]] Tile getTile(const int row, const int col) const { return gameBoard[row][col]; }

  float moveAmounts[10] = {-18.0f, -14.0f, -10.0f, -6.0f, -3.0f, -1.5f, 1.0f, 0.6f, 0.3f, 0.1f};


  static std::array<Tile, 4> mergeTiles(Tile inputRow[4]) {
    std::vector<Tile> numbersFromRow; // Stores all the non-zero tiles from the inputRow
    for ( int x = 0; x < 4; x++ ) {
      if ( inputRow[x].isNumber() ) { // Check if the tile is a number, then adds it to the vector
        numbersFromRow.push_back(inputRow[x]);
      }
    }

    std::vector<Tile> mergedNumbers; // Stores the results of the merges
    for ( int col = 0; col < numbersFromRow.size(); col++ ) {
      // Loop through the vector
      if ( col + 1 < numbersFromRow.size() && numbersFromRow.at(col).getValue() == numbersFromRow.at(col + 1).getValue() ) { // Check if the tile to the right is the same number
        Tile tempTile; // Create a temporary tile to push to the vector
        tempTile.setValue(numbersFromRow.at(col).getValue() + numbersFromRow.at(col + 1).getValue()); // Calculate the new value
        mergedNumbers.push_back(tempTile);
        col++; // Skip the next tile
      } else {
        mergedNumbers.push_back(numbersFromRow.at(col)); // Add in the tile if it does not merge
      }
    }

    const std::size_t startingCol = 4 - mergedNumbers.size(); // The starting column to put the value back in to the array
    std::array<Tile, 4> returnedRow; // The final array that has all fo the zeros and merged numbers back in
    int mergedNumbersIndex = 0;
    for ( int y = 0; y < 4; y++ ) {
      if ( y >= startingCol ) { // Check if the index in the array is larger than the number of values after merge
        returnedRow[y].setValue(mergedNumbers[mergedNumbersIndex].getValue()); // Set that tile to the correct value
        mergedNumbersIndex++;
      }
    }

    for ( const Tile &tile: returnedRow ) {
      std::cout << tile << std::endl;
    }

    return returnedRow;
  }

  void merge(const char direction) {
    switch (direction) {
      case static_cast<char>(108): // l
        this->mergeLeft();
        this->addRandomTile();
        break;
      case static_cast<char>(114): // r
        this->mergeRight();
        this->addRandomTile();
        break;
      case static_cast<char>(117): // u
        this->mergeUp();
        this->addRandomTile();
        break;
      case static_cast<char>(100): // d
        this->mergeDown();
        this->addRandomTile();
        break;


      default:;
    }
  }

private:
  std::array<std::array<Tile, 4>, 4> gameBoard;

  bool rowHasNumbers(const int row) const {
    int sum = 0;
    for ( const auto &tile: gameBoard[row] ) {
      sum += tile.getValue();
    }
    return sum > 0;
  }

  static bool rowHasNumbers(const std::array<Tile, 4>& mergedArray) {
    int sum = 0;
    for (const Tile& tile : mergedArray) {
      sum += tile.getValue();
    }
    return sum > 0;
  }

  void mergeRight() {
    std::array<Tile, 4> mergedArray;
    for ( int row = 0; row < 4; row++ ) {
      if ( this->rowHasNumbers(row) ) { // Check if the row has numbers, otherwise skip
        gameBoard[row] = mergeTiles(gameBoard[row].data());
      }
    }
  }

  void mergeLeft() {
    for ( int row = 0; row < 4; row++ ) {
      std::array<Tile, 4> reverseRow = gameBoard[row];
      if ( rowHasNumbers(row) ) { // Check if the row has numbers, otherwise skip
        std::ranges::reverse(reverseRow);
        reverseRow = mergeTiles(reverseRow.data());
        std::ranges::reverse(reverseRow);
        gameBoard[row] = reverseRow;
      }
    }
  }

  void mergeUp() {
    std::array<Tile, 4> mergedArray;
    for ( int col = 0; col < 4; col++ ) {
      // Collect column
      for ( int row = 0; row < 4; row++ ) {
        mergedArray[row] = gameBoard[row][col];
      }

      if ( Board::rowHasNumbers(mergedArray)) { // Check if row is empty
        // Check if the row has numbers, otherwise skip

        // Reverse, merge, reverse back
        std::ranges::reverse(mergedArray);
        mergedArray = mergeTiles(mergedArray.data());
        std::ranges::reverse(mergedArray);

        // Put back in board
        for ( int row = 0; row < 4; row++ ) {
          gameBoard[row][col] = mergedArray[row];
        }
      }
    }
  }

  void mergeDown() {
    std::array<Tile, 4> mergedArray;
    for ( int col = 0; col < 4; col++ ) {
      for ( int row = 0; row < 4; row++ ) {
        mergedArray[row] = gameBoard[row][col];
      }

      if ( Board::rowHasNumbers(mergedArray)) { // Check if row is empty

        mergedArray = mergeTiles(mergedArray.data());
        for ( int row = 0; row < 4; row++ ) {
          gameBoard[row][col] = mergedArray[row];
        }
      }
    }
  }

  void addRandomTile() {
    std::pair<int, int> location = generateRandomLocation();
    int startingValue = generateRandomNumber(2, 4);
    startingValue = startingValue == 3 ? 2 : startingValue; // Turns the 3 into a 2;
    gameBoard[location.first][location.second].setValue(startingValue);
  }
};

#endif
