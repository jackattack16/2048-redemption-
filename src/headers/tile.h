// controls each individual tile
#pragma once

#include <iostream>
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
    
    

};

#endif 