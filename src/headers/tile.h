// controls each individual tile
#pragma once

#include <iostream>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#ifndef TILE_H
#define TILE_H

class Tile {
  public:
    int value = 0;
    [[nodiscard]] bool isNumber() const {
      return this->value != 0;
    }

    void setValue(const int number) {
      this->value = number;
    }

    int getValue() const {
      return this->value;
    }

    sf::Color getColor() const {
      return this->tileBackgrounds.at(this->value);
    }

    sf::Color getFontColor() const {
      if (this->value < 8) {
        return {0x77, 0x6e, 0x65}; // #776e65
      } else {
        return {0xf9, 0xf6, 0xf2}; // #f9f6f2
      }
    }

    // Allows printing (thanks claude)
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile) {
      os << tile.value;
      return os;
    }

    void createTile(const int startingValue) {
      if (startingValue == 3) {
        this->setValue(2);
      } else {
        this->setValue(startingValue);
      }
    }

    void reset() {
      this->value = 0;
    }

  private:
    // Thanks Claude!
    std::unordered_map<int, sf::Color> tileBackgrounds = {
      {0, sf::Color(0xcc, 0xc0, 0xb4)},    // #ccc0b4
      {2, sf::Color(0xee, 0xe4, 0xda)},      // #776e65
      {4, sf::Color(0xed, 0xe0, 0xc8)},      // #776e65
      {8, sf::Color(0xf2, 0xb1, 0x79)},      // #f2b179
      {16, sf::Color(0xf5, 0x95, 0x63)},     // #f59563
      {32, sf::Color(0xf6, 0x7c, 0x5f)},     // #f67c5f
      {64, sf::Color(0xf6, 0x5e, 0x3b)},     // #f65e3b
      {128, sf::Color(0xed, 0xcf, 0x72)},    // #edcf72
      {256, sf::Color(0xed, 0xcc, 0x61)},    // #edcc61
      {512, sf::Color(0xed, 0xc8, 0x50)},    // #edc850
      {1024, sf::Color(0xed, 0xc5, 0x3f)},   // #edc53f
      {2048, sf::Color(0xed, 0xc2, 0x2e)}    // #edc22e
    };


};

#endif 