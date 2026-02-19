// Overall game class, handles inputs and sends actions to lower classes
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"
#include "cubicBezier.h"
#include "roundedRectangle.h"
#include "scoreNumberEffect.h"
#ifndef GAME_H
#define GAME_H

class Game {
public:
  Game() {
    if ( !font.openFromFile("../clear-sans/ClearSans-Bold.ttf") ) {
      std::cerr << "Failed to load font (failed to create the font face)" << std::endl;
      std::cerr << "Provided path: \"Blue Winter.ttf\"" << std::endl;
    }

    makeBackgroundTexture();
    boardSprite.emplace(boardLayoutTexture);
    boardSprite->setOrigin(boardSprite->getLocalBounds().getCenter());
    boardSprite->setScale({0.9f, 0.9f});
    boardSprite->setPosition({((1.0f - boardPercentSize) / 2.0f) * 512.f + (boardSprite->getGlobalBounds().size.x / 2.f), ((1.0f - boardPercentSize) / 2.0f) * 512.f + 64.f + (boardSprite->getGlobalBounds().size.y / 2.f)});

    makeScoreTexture();
    scoreSprite.emplace(scoreTexture);
    scoreSprite->setScale({0.9f, 0.9f});
    scoreSprite->setPosition({((1.0f - boardPercentSize) / 2.0f) * 512.f + (scoreSprite->getGlobalBounds().size.x / 5.f), (scoreSprite->getGlobalBounds().size.x - 64.f) / 2.f});

    makeTextVector();
  }

  // Handles inputs and send it to the Board subclass
  void pullInputs() {
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !this->theBoard.isPreformingAction ) {
      // std::cout << "left" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 0;
      this->theBoard.merge('l');
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !this->theBoard.isPreformingAction ) {
      // std::cout << "right" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 1;
      this->theBoard.merge('r');
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && !this->theBoard.isPreformingAction ) {
      // std::cout << "up" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 2;
      this->theBoard.merge('u');
    }
    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !this->theBoard.isPreformingAction ) {
      // std::cout << "down" << std::endl;
      this->theBoard.isPreformingAction = true;
      this->theBoard.movementDirection = 3;
      this->theBoard.merge('d');
    }

    // Easing selection with number keys 1-7
    bool anyEasingKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6) ||
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7);

    if (!anyEasingKey) {
      easingKeyHeld = false;
    }

    if (!easingKeyHeld) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) { currentEasingIndex = 0; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) { currentEasingIndex = 1; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) { currentEasingIndex = 2; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) { currentEasingIndex = 3; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) { currentEasingIndex = 4; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) { currentEasingIndex = 5; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) { currentEasingIndex = 6; easingKeyHeld = true; std::cout << "Easing: " << getCurrentEasingName() << std::endl; }
    }
  }

  void start() { this->theBoard.setUpGameBoard(); }

  sf::Font &getFont() { return this->font; }

  Board theBoard;
  sf::Font font;
  sf::Texture boardLayoutTexture;
  sf::Texture scoreTexture;
  sf::Texture tileTexture;

  float boardPercentSize = 0.9f;
  std::optional<sf::Sprite> boardSprite;
  std::optional<sf::Sprite> scoreSprite;
  float radius = 20.f;
  const sf::Vector2f gameSize = {512.f, 512.f};

  std::vector<sf::Text> textVector;

  void makeBackgroundTexture() {

    sf::RenderTexture render_texture({512, 512});


    // Set up the background
    RoundedRectangleShape boardBackground(gameSize, radius, 4);
    boardBackground.setFillColor(sf::Color{0x77, 0x6e, 0x65}); // #776e65
    // render_texture.clear(sf::Color{0xcf, 0xc2, 0xb6}); //#cfc2b6
    render_texture.clear();
    render_texture.draw(boardBackground);
    //
    // // Set up the empty tiles
    // sf::Vector2f tileSize = {gameSize.x / 5.0f, gameSize.y / 5.0f};
    // sf::Vector2f tileGutterSize = {tileSize.x / 5.f, tileSize.y / 5.f};
    // for (int row = 0; row < 4; row++) {
    //   for (int col = 0; col < 4; col++) {
    //     RoundedRectangleShape tile(tileSize, radius, 4);
    //     tile.setPosition({tileGutterSize.x * static_cast<float>(col + 1) + (tileSize.x * static_cast<float>(col)), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row))});
    //
    //     tile.setFillColor(sf::Color(238, 228, 218, 125)); // #ccc0b4
    //
    //     render_texture.draw(tile);
    //   }
    // }

    // Create the texture
    render_texture.display();
    boardLayoutTexture = render_texture.getTexture();
    boardLayoutTexture.setSmooth(true);
  }

  void makeScoreTexture() {
    constexpr sf::Vector2f tileSize = {512.f / 5.0f, 512.f / 7.5f};
    sf::RenderTexture render_texture({static_cast<unsigned>(tileSize.x), static_cast<unsigned>(tileSize.y)});

    // Set up the score element
    RoundedRectangleShape scoreBoard(tileSize, radius / 2.f, 4);
    scoreBoard.setFillColor(sf::Color{0xbb, 0xad, 0xa0}); // #bbada0
    render_texture.draw(scoreBoard);

    // Text
    sf::Text scoreText(font);
    scoreText.setString("Score");
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(24);
    const float offset = (tileSize.x - scoreText.getGlobalBounds().size.x) / 2.f;
    scoreText.setPosition({offset, 0.f});
    render_texture.draw(scoreText);

    // Create Texture
    render_texture.display();
    scoreTexture = render_texture.getTexture();
    scoreTexture.setSmooth(true);
  }

  void makeTextVector() {
    std::string numbers[11] = {"2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048"};

    for ( int number = 0; number < 11; number++ ) {
      sf::Text text(getFont());
      text.setString(numbers[number]);

      if ( number > 1 ) {
        text.setFillColor(sf::Color{0xf9, 0xf6, 0xf2}); // #f9f6f2
      } else {
        text.setFillColor(sf::Color{0x77, 0x6e, 0x65}); // #776e65
      }
      text.setCharacterSize(36);
      text.setOrigin(text.getLocalBounds().getCenter());

      textVector.push_back(text);
    }
  }


  void renderGame(sf::RenderWindow &window, sf::Time dTime) {
    const sf::View mainView = window.getDefaultView();
    window.setView(mainView);
    window.draw(*scoreSprite);
    const sf::Vector2f tileSize = {(gameSize.x / 5.0f) * 0.9f, (gameSize.y / 5.0f) * 0.9f};

    sf::Text scoreNumber(getFont());
    scoreNumber.setString(std::to_string(theBoard.getScore()));
    scoreNumber.setCharacterSize(30);
    scoreNumber.setOrigin(scoreNumber.getLocalBounds().getCenter());
    scoreNumber.setScale({0.9f, 0.9f});
    sf::Vector2f spriteCenter = {scoreSprite->getPosition().x + (scoreSprite->getGlobalBounds().size.x / 2.f), scoreSprite->getPosition().y + (scoreSprite->getGlobalBounds().size.y / 2.f) + 10.f};
    scoreNumber.setPosition(spriteCenter);


    window.draw(scoreNumber);
    window.draw(*boardSprite);


    const sf::Vector2f tileGutterSize = {tileSize.x / 5.f, tileSize.y / 5.f};
    sf::View tileRender(sf::FloatRect({0.f, 0.f}, sf::Vector2<float>(window.getSize())));
    tileRender.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));

    const sf::Time animationDuration = sf::milliseconds(300);
    const float maxOffset = 10.f;

    if ( theBoard.isPreformingAction ) {
      theBoard.animationElapsedTime += dTime;
      
      float progress = std::clamp(
        theBoard.animationElapsedTime.asSeconds() / animationDuration.asSeconds(),
        0.0f, 1.0f
      );
      
      if ( progress >= 1.0f ) {
        theBoard.endAnimation();
      } else {
        float easedProgress = static_cast<float>(getCurrentEasing().Solve(progress));
        float offset = easedProgress * maxOffset;
        
        switch ( theBoard.movementDirection ) {
          case 0: // Left
            tileRender.move({maxOffset - offset, 0.f});
            break;
          case 1: // Right
            tileRender.move({offset - maxOffset, 0.f});
            break;
          case 2: // Up
            tileRender.move({0.f, maxOffset - offset});
            break;
          case 3: // Down
            tileRender.move({0.f, offset - maxOffset});
            break;
          default:;
        }
      }
    }

    window.setView(tileRender);
    for ( int row = 0; row < 4; row++ ) {
      for ( int col = 0; col < 4; col++ ) {
        RoundedRectangleShape tile(tileSize, radius, 4);
        tile.setOrigin(tile.getLocalBounds().getCenter());

        // tile.setPosition({tileGutterSize.x * static_cast<float>(col + 1) + (tileSize.x * static_cast<float>(col)), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f});
        tile.setPosition({tileGutterSize.x * static_cast<float>(col) + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + tileGutterSize.x + (tileSize.x * static_cast<float>(col)) + (tile.getGlobalBounds().size.x / 2.f), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + (tile.getGlobalBounds().size.y / 2.f)});
        tile.setFillColor(theBoard.getTile(row, col).getColor()); // #ccc0b4
        if (theBoard.getTile(row, col).isNumber()) {
          float progress = theBoard.getTile(row, col).getAnimationProgress();
          float easedProgress = static_cast<float>(tileScaleEase.Solve(progress));
          float scale = 0.5f + 0.5f * easedProgress;
          tile.setScale({scale, scale});
          theBoard.getTile(row, col).increaseAnimation();
          window.draw(tile);
        }

        // theBoard.getTile(row, col).getIndex()

        if (theBoard.getTile(row, col).isNumber()) {
          sf::Text number = textVector[theBoard.getTile(row, col).getIndex()];
          number.setOrigin(number.getLocalBounds().getCenter());
          number.setPosition({tileGutterSize.x * static_cast<float>(col) + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + tileGutterSize.x + (tileSize.x * static_cast<float>(col)) + (tileSize.x / 2.f), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + (tileSize.y / 2.f)});
          number.setScale(theBoard.getTile(row, col).getSize());
          window.draw(number);
        }
      }
    }
    window.setView(mainView);
    for ( int row = 0; row < 4; row++ ) {
      for ( int col = 0; col < 4; col++ ) {
        if (!theBoard.getTile(row, col).isNumber()) {
          RoundedRectangleShape tile(tileSize, radius, 4);
          tile.setOrigin(tile.getLocalBounds().getCenter());
          tile.setPosition({tileGutterSize.x * static_cast<float>(col) + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + tileGutterSize.x + (tileSize.x * static_cast<float>(col)) + (tile.getGlobalBounds().size.x / 2.f), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + (tile.getGlobalBounds().size.y / 2.f)});
          tile.setFillColor(theBoard.getTile(row, col).getColor()); // #ccc0b4
          window.draw(tile);
        }
      }
    }

    // Thanks Claude
    for (auto it = theBoard.scoreEffects.begin(); it != theBoard.scoreEffects.end(); ) {
      if (it->updatePosition(dTime)) {
        it = theBoard.scoreEffects.erase(it); // erase returns next valid iterator
        // std::cout << "deleted" << std::endl;
      } else {
        window.draw(it->getText());
        ++it;
      }
    }
  }


  gfx::CubicBezier easeSmoothOut{0.25, 0.1, 0.25, 1.0};
  gfx::CubicBezier easeOutCubic{0.33, 1.0, 0.68, 1.0};
  gfx::CubicBezier easeOutQuad{0.25, 1.0, 0.5, 1.0};
  gfx::CubicBezier easeOutExpo{0.16, 1.0, 0.3, 1.0};
  gfx::CubicBezier easeOutCirc{0.0, 0.55, 0.45, 1.0};
  gfx::CubicBezier easeLinear{0.0, 0.0, 1.0, 1.0};
  gfx::CubicBezier easeInOut{0.45, 0.0, 0.55, 1.0};

  int currentEasingIndex = 0;
  bool easingKeyHeld = false;

  gfx::CubicBezier& getCurrentEasing() {
    switch (currentEasingIndex) {
      case 0: return easeSmoothOut;
      case 1: return easeOutCubic;
      case 2: return easeOutQuad;
      case 3: return easeOutExpo;
      case 4: return easeOutCirc;
      case 5: return easeLinear;
      case 6: return easeInOut;
      default: return easeSmoothOut;
    }
  }

  const char* getCurrentEasingName() {
    switch (currentEasingIndex) {
      case 0: return "Smooth ease-out";
      case 1: return "Ease-out cubic";
      case 2: return "Ease-out quad";
      case 3: return "Ease-out expo";
      case 4: return "Ease-out circ";
      case 5: return "Linear";
      case 6: return "Ease-in-out";
      default: return "Unknown";
    }
  }

  gfx::CubicBezier tileScaleEase{0.33f, 1.0f, 0.68f, 1.0f};
};

#endif
