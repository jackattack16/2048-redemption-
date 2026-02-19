// controls each individual tile
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

#include "cubicBezier.h"
#ifndef TILE_H
#define TILE_H

class Tile {
public:
  int value = 0;
  [[nodiscard]] bool isNumber() const { return this->value != 0; }

  void setValue(const int number, bool scale) {
    if ( scale ) {
      this->scaleAnimation = 0;
      this->tileSize = sf::Vector2f(1.25f, 1.25f);
    }
    this->value = number;
  }

  int getValue() const { return this->value; }


  sf::Color getColor() const { return this->tileBackgrounds.at(this->value); }

  sf::Color getFontColor() const {
    if ( this->value < 4 ) {
      return {0x77, 0x6e, 0x65}; // #776e65
    } else {
      return {0xf9, 0xf6, 0xf2}; // #f9f6f2
    }
  }

  bool isAnimating() const {
    return scaleAnimation < 9;
  }

  float getAnimationProgress() const {
    return static_cast<float>(scaleAnimation) / 9.f;
  }

  void increaseAnimation() {
    if (scaleAnimation < 9) {
      scaleAnimation++;
    }
  }

  // Allows printing (thanks claude)
  friend std::ostream &operator<<(std::ostream &os, const Tile &tile) {
    os << tile.value;
    return os;
  }

  Tile(int value = 0) {
    if ( value == 3 ) {
      this->setValue(2, true);
    } else {
      this->setValue(value, true);
    }

    this->scaleAnimation = 0;
    this->tileSize = sf::Vector2f(0.5f, 0.5f);
  }

  void reset() { this->value = 0; }
  // Thanks Claude!
  bool operator==(const Tile &other) const { return this->getValue() == other.getValue(); }

  int getIndex() const { return this->numberToNumber.at(value); }

  void updateSize() {
    float progress = scaleAnimation / 9;
    if ( this->scaleAnimation < 9 ) {
      this->tileSize.x -= 0.025f;
      this->tileSize.y -= 0.025f;
      this->scaleAnimation++;
      std::cout << this->scaleAnimation << std::endl;
    }
  }

  sf::Vector2f getSize() { return tileSize; }


private:
  // Thanks Claude!
  std::unordered_map<int, sf::Color> tileBackgrounds = {
      {0, sf::Color(0xcc, 0xc0, 0xb4)}, // #ccc0b4
      {2, sf::Color(0xee, 0xe4, 0xda)}, // #776e65
      {4, sf::Color(0xed, 0xe0, 0xc8)}, // #776e65
      {8, sf::Color(0xf2, 0xb1, 0x79)}, // #f2b179
      {16, sf::Color(0xf5, 0x95, 0x63)}, // #f59563
      {32, sf::Color(0xf6, 0x7c, 0x5f)}, // #f67c5f
      {64, sf::Color(0xf6, 0x5e, 0x3b)}, // #f65e3b
      {128, sf::Color(0xed, 0xcf, 0x72)}, // #edcf72
      {256, sf::Color(0xed, 0xcc, 0x61)}, // #edcc61
      {512, sf::Color(0xed, 0xc8, 0x50)}, // #edc850
      {1024, sf::Color(0xed, 0xc5, 0x3f)}, // #edc53f
      {2048, sf::Color(0xed, 0xc2, 0x2e)} // #edc22e
  };


  std::unordered_map<int, int> numberToNumber = {{2, 0}, {4, 1}, {8, 2}, {16, 3}, {32, 4}, {64, 5}, {128, 6}, {256, 7}, {512, 8}, {1024, 9}, {2048, 10}};

  int scaleAnimation = 0;

  sf::Vector2f tileSize = sf::Vector2f(0.9f, 0.9f);

  // const static gfx::CubicBezier ease{0.33f, 1.0f, 0.68f, 1.0f};
};

#endif
