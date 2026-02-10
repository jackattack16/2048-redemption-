#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"
#include "roundedRectangle.h"
#include "tile.h"

inline void createBackground(sf::RenderWindow &window, const float radius, const sf::Vector2f tileSidePadding, const sf::Vector2u windowSize, const sf::Vector2f gameSize) {
  RoundedRectangleShape boardBackground(gameSize, radius, 4);
  boardBackground.setPosition({tileSidePadding.x, tileSidePadding.y});
  boardBackground.setFillColor(sf::Color{0x77, 0x6e, 0x65}); // #776e65
  window.draw(boardBackground);
}


inline void drawTile(sf::RenderWindow &window, const float radius, const sf::Vector2f sidePadding, const sf::Vector2f tileSize, const sf::Vector2f tileGutterSize, int row, int col, const Board &inputBoard) {
  RoundedRectangleShape tile(tileSize, radius, 4);
  tile.setPosition({tileGutterSize.x * (col + 1) + sidePadding.x + (tileSize.x * col), tileGutterSize.y * (row + 1) + sidePadding.y + (tileSize.y * row)});

  tile.setFillColor(inputBoard.getTile(row, col).getColor());

  window.draw(tile);
}

inline void drawTileNumber(Board &inputBoard, sf::RenderWindow &window, const sf::Font &font, const sf::Vector2f sidePadding, const sf::Vector2f tileSize, const sf::Vector2f tileGutterSize, int row, int col) {
  sf::Text text(font);
  text.setString(std::to_string(inputBoard.getTile(row, col).value));
  text.setCharacterSize(50);
  text.setFillColor(inputBoard.getTile(row, col).getFontColor());
  text.setPosition({tileGutterSize.x * (col + 1) + sidePadding.x + (tileSize.x * col), tileGutterSize.y * (row + 1) + sidePadding.y + (tileSize.y * row)});
  window.draw(text);
}


inline void renderGame(Board &inputBoard, sf::RenderWindow &window, const sf::Font &font) {
  constexpr float boardPrecentSize = 0.9f;
  constexpr float radius = 5.0f;
  constexpr float boardPadding = (1.0f - boardPrecentSize) / 2.0f;
  sf::View mainView = window.getDefaultView();
  const sf::Vector2u windowSize = window.getSize();
  const sf::Vector2f gameSize = {static_cast<float>(windowSize.x * boardPrecentSize), static_cast<float>(windowSize.y * boardPrecentSize)};
  const sf::Vector2f sidePadding = {boardPadding * windowSize.x, boardPadding * windowSize.y};


  const sf::Vector2f tileSize = {gameSize.x / 5.0f, gameSize.y / 5.0f};
  const sf::Vector2f tileGutterSize = {tileSize.x / 5.f, tileSize.y / 5.f};

  sf::View tileRender(sf::FloatRect({0.f, 0.f}, sf::Vector2<float>(window.getSize())));
  tileRender.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
  // Create Board
  window.setView(mainView);
  createBackground(window, radius, sidePadding, windowSize, gameSize);

  // Move the tiles if a input direction is pressed
  if (inputBoard.isPreformingAction) {
    if (inputBoard.animationFrame == 15) {
      inputBoard.endAnimation();
      return;
    }
    switch (inputBoard.movementDirection) { // Move the board in the direction of the press to add emphasis
      case 0: // Left
        tileRender.move({ inputBoard.moveAmounts[inputBoard.animationFrame] * -1.f ,0.f});
        break;
      case 1: // Right
        tileRender.move({ inputBoard.moveAmounts[inputBoard.animationFrame],0.f});
        break;
      case 2: // Up
        tileRender.move({ 0.f,inputBoard.moveAmounts[inputBoard.animationFrame] * -1.f});
        break;
      case 3: // Down
        tileRender.move({ 0.f,inputBoard.moveAmounts[inputBoard.animationFrame]});
        break;
      default:;
    }

    inputBoard.animationFrame++;

  }
  window.setView(tileRender);




  // Create Tiles
  for ( int row = 0; row < 4; row++ ) {
    for ( int col = 0; col < 4; col++ ) {
      drawTile(window, radius, sidePadding, tileSize, tileGutterSize, row, col, inputBoard);
      if (inputBoard.getTile(row, col).isNumber()) {
        drawTileNumber(inputBoard, window, font, sidePadding, tileSize, tileGutterSize, row, col);
      }
    }
  }

  window.display();
}
