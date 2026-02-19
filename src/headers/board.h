// Holds all the information about the state of the board and the tiles. Takes inputs from the game class and validates them
#pragma once

#include <algorithm>
#include <random>
#include <vector>
#include <SFML/System/Time.hpp>
#include "tile.h"
#include "scoreNumberEffect.h"
#ifndef BOARD_H
#define BOARD_H

class Board {


public:
  void resetBoard() {
    for ( auto &row: gameBoard ) {
      for ( int col = 0; col < 4; col++ ) {
        row[col].reset();
      }
    }
    this->generateStartingBoard();
  }

  bool isPreformingAction{};
  bool isAnimating{};
  sf::Time animationElapsedTime{};
  int movementDirection{};

  void endAnimation() {
    this->isAnimating = false;
    this->animationElapsedTime = sf::Time::Zero;
    this->isPreformingAction = false;
  }

  void setUpGameBoard() {
    this->createOpenSpacesVector();
    generateStartingBoard();
  }

  void generateStartingBoard() { // Generates 2 tiles in random locations with 2/3 chance to be 2 and 1/3 to be 4
    std::pair<int, int> location = generateRandomLocation();
    int startingValue = generateRandomNumber(2, 4);
    startingValue = startingValue == 3 ? 2 : startingValue; // Turns the 3 into a 2;
    gameBoard[location.first][location.second].setValue(startingValue, true);

    location = generateRandomLocation();
    startingValue = generateRandomNumber(2, 4);
    startingValue = startingValue == 3 ? 2 : startingValue;
    gameBoard[location.first][location.second].setValue(startingValue, true);
  }


  // Thanks Claude!
  std::pair<int, int> generateRandomLocation() {
    if (openSpaces.empty()) {
      throw std::runtime_error("No open spaces available!");
    }

    int index = generateRandomNumber(0, openSpaces.size() - 1);
    std::pair<int, int> location = openSpaces[index];

    // Swap with last element, then pop - O(1) instead of O(n)
    openSpaces[index] = openSpaces.back();
    openSpaces.pop_back();

    return location;
  }

  // After merging, rebuild the vector (Thanks Claude)
  void updateOpenSpaces() {
    openSpaces.clear();
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        if (!gameBoard[row][col].isNumber()) {
          openSpaces.emplace_back(row, col);
        }
      }
    }
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

  [[nodiscard]] Tile& getTile(int row, int col) { return gameBoard[row][col]; }

  // OPTION 1: Reduced amplitude (half the movement) - uncomment to use
  // float moveAmounts[10] = {-9.0f, -7.0f, -5.0f, -3.0f, -1.5f, -0.75f, 0.5f, 0.3f, 0.15f, 0.05f};

  // OPTION 2: Smoother easing curve - uncomment to use
  // float moveAmounts[10] = {-12.0f, -10.0f, -7.5f, -5.0f, -3.0f, -1.5f, -0.75f, 0.5f, 0.25f, 0.1f};

  // OPTION 3: Longer, smoother animation (15 frames) - uncomment to use
  // Also update animationFrame check in render.h from 15 to 16
  float moveAmounts[15] = {-10.0f, -9.0f, -8.0f, -6.5f, -5.0f, -3.5f, -2.5f, -1.5f, -0.75f, -0.5f, 0.3f, 0.2f, 0.15f, 0.1f, 0.05f};

  // ORIGINAL: Current animation curve
  // float moveAmounts[10] = {-18.0f, -14.0f, -10.0f, -6.0f, -3.0f, -1.5f, 1.0f, 0.6f, 0.3f, 0.1f};


  std::array<Tile, 4> mergeTiles(Tile inputRow[4]) {
    std::vector<Tile> numbersFromRow; // Stores all the non-zero tiles from the inputRow
    for ( int x = 0; x < 4; x++ ) {
      if ( inputRow[x].isNumber() ) { // Check if the tile is a number, then adds it to the vector
        numbersFromRow.push_back(inputRow[x]);
      }
    }
    int totalScore = 0;
    std::vector<Tile> mergedNumbers; // Stores the results of the merges
    for ( int col = 0; col < numbersFromRow.size(); col++ ) {
      // Loop through the vector
      if ( col + 1 < numbersFromRow.size() && numbersFromRow.at(col).getValue() == numbersFromRow.at(col + 1).getValue() ) { // Check if the tile to the right is the same number
        Tile tempTile; // Create a temporary tile to push to the vector
        const int newValue = numbersFromRow.at(col).getValue() + numbersFromRow.at(col + 1).getValue();
        tempTile.setValue(newValue, true); // Calculate the new value
        score += newValue;
        totalScore += newValue;
        mergedNumbers.push_back(tempTile);
        col++; // Skip the next tile
      } else {
        mergedNumbers.push_back(numbersFromRow.at(col)); // Add in the tile if it does not merge
      }
    }

    if (totalScore > 0) {
      scoreNumberEffect newNumber{totalScore};
      scoreEffects.push_back(newNumber);
    }

    const std::size_t startingCol = 4 - mergedNumbers.size(); // The starting column to put the value back in to the array
    std::array<Tile, 4> returnedRow; // The final array that has all fo the zeros and merged numbers back in
    int mergedNumbersIndex = 0;
    for ( int y = 0; y < 4; y++ ) {
      if ( y >= startingCol ) { // Check if the index in the array is larger than the number of values after merge
        returnedRow[y] = mergedNumbers[mergedNumbersIndex]; // Set that tile to the correct value
        mergedNumbersIndex++;
      }
    }

    return returnedRow;
  }

  void merge(const char direction) {
    bool boardChanged = false;
    switch ( direction ) {
      case 'l': // l
        this->mergeLeft();
        break;
      case 'r': // r
        this->mergeRight();
        break;
      case 'u': // u
        this->mergeUp();
        break;
      case 'd': // d
        this->mergeDown();
        break;
      default:
        break;
    }
    this->updateOpenSpaces();
    this->addRandomTile();
  }

  int getScore() const {
    return this->score;
  }
  std::vector<scoreNumberEffect> scoreEffects;
private:
  std::array<std::array<Tile, 4>, 4> gameBoard;

  bool rowHasNumbers(const int row) const {
    int sum = 0;
    for ( const auto &tile: gameBoard[row] ) {
      sum += tile.getValue();
    }
    return sum > 0;
  }

  static bool rowHasNumbers(const std::array<Tile, 4> &mergedArray) {
    int sum = 0;
    for ( const Tile &tile: mergedArray ) {
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

      if ( Board::rowHasNumbers(mergedArray) ) { // Check if row is empty


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

      const auto staticArray = mergedArray; // Declare an array that will not change during merge
      if ( Board::rowHasNumbers(mergedArray) ) { // Check if row is empty
        mergedArray = mergeTiles(mergedArray.data());

        // bool test = staticArray != mergedArray;
        // std::cout << test << std::endl;

        // if ( staticArray != mergedArray ) { // Check to see if the board has changed
          for ( int row = 0; row < 4; row++ ) {
            gameBoard[row][col] = mergedArray[row];
          }
          // return true;
        // } else {
        //   return false;
        // }
      }
    }
  }

  void addRandomTile() {
    std::pair<int, int> location = generateRandomLocation();
    int startingValue = generateRandomNumber(2, 4);
    startingValue = startingValue == 3 ? 2 : startingValue; // Turns the 3 into a 2;
    gameBoard[location.first][location.second].setValue(startingValue, true);
  }

  std::vector<std::pair<int, int>> openSpaces;

  void createOpenSpacesVector() {
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        openSpaces.emplace_back(row, col);
      }
    }
  }

  int score = 0;


};

#endif
