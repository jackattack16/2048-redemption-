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

    if ( !blurShader.loadFromFile("../src/headers/box-blur.frag", sf::Shader::Type::Fragment) ) {
      std::cerr << "Failed to load shaders" << std::endl;
    }
    if ( !restartTexture.loadFromFile("../src/headers/restartIcon.png", false, sf::IntRect({0, 0}, {256, 256}))) {
      std::cerr << "Failed to load restart" << std::endl;
    }

    // if (



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
  void checkLose(sf::Time dTime) {
    for ( int row = 0; row < 4; row++ ) {
      for ( int col = 0; col < 4; col++ ) {
        if ( !theBoard.getTile(row, col).isNumber() ) {
          return;
        }
      }
    }
    if ( theBoard.checkIfLost() && inGame ) {
      if (waitForLose < sf::seconds(1.5f)) {
        waitForLose += dTime;
      } else {
        std::cout << "lost" << std::endl;
        this->inGame = false;
        this->hasLost = true;
      }
    }
  }
  // Handles inputs and send it to the Board subclass
  void pullInputs(const sf::RenderWindow &window) {
    // if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
    //   this->theBoard.resetBoard();
    // }
    if ( this->inGame ) {
      if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))) && !this->theBoard.isPreformingAction ) {
        // std::cout << "left" << std::endl;
        this->theBoard.isPreformingAction = true;
        this->theBoard.movementDirection = 0;
        this->theBoard.merge('l');
      }
      if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))) && !this->theBoard.isPreformingAction ) {
        // std::cout << "right" << std::endl;
        this->theBoard.isPreformingAction = true;
        this->theBoard.movementDirection = 1;
        this->theBoard.merge('r');
      }
      if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))) && !this->theBoard.isPreformingAction ) {
        // std::cout << "up" << std::endl;
        this->theBoard.isPreformingAction = true;
        this->theBoard.movementDirection = 2;
        this->theBoard.merge('u');
      }
      if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))) && !this->theBoard.isPreformingAction ) {
        // std::cout << "down" << std::endl;
        this->theBoard.isPreformingAction = true;
        this->theBoard.movementDirection = 3;
        this->theBoard.merge('d');
      }
    } else {
      // Make the playbutton but dont render it for collision checks
      sf::Vector2f gameSize(static_cast<sf::Vector2f>(window.getSize()));
      RoundedRectangleShape playButton({96.f, 96.f}, 24.0f, 12);
      playButton.setOrigin(playButton.getLocalBounds().getCenter());
      playButton.setPosition({gameSize.x / 2.f, (gameSize.y / 2.5f) + (gameSize.y / 5.f)});

      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      if ( sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && playButton.getGlobalBounds().contains(sf::Vector2<float>(mousePosition)) ) {
        if (!hasLost) {
          std::cout << "start game" << std::endl;
          this->inGame = true;
        } else {
          theBoard.resetBoard();
          this->inGame = true;
        }
      }
    }
  }

  void start() { this->theBoard.setUpGameBoard(); }

  sf::Font &getFont() { return this->font; }

  Board theBoard;
  sf::Font font;
  sf::Texture boardLayoutTexture;
  sf::Texture scoreTexture;
  sf::Texture tileTexture;
  sf::Texture restartTexture;

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
    sf::RenderTexture gameTexture(window.getSize());
    gameTexture.clear();

    const sf::Vector2f tileSize = {(gameSize.x / 5.0f) * 0.9f, (gameSize.y / 5.0f) * 0.9f};

    sf::Text scoreNumber(getFont());
    scoreNumber.setString(std::to_string(theBoard.getScore()));
    scoreNumber.setCharacterSize(30);
    scoreNumber.setOrigin(scoreNumber.getLocalBounds().getCenter());
    scoreNumber.setScale({0.9f, 0.9f});
    sf::Vector2f spriteCenter = {scoreSprite->getPosition().x + (scoreSprite->getGlobalBounds().size.x / 2.f), scoreSprite->getPosition().y + (scoreSprite->getGlobalBounds().size.y / 2.f) + 10.f};
    scoreNumber.setPosition(spriteCenter);

    if ( inGame ) {
      window.setView(mainView);
      window.draw(*scoreSprite);
      window.draw(scoreNumber);
      window.draw(*boardSprite);
    } else {
      gameTexture.setView(mainView);
      gameTexture.draw(*scoreSprite);
      gameTexture.draw(scoreNumber);
      gameTexture.draw(*boardSprite);
    }


    const sf::Vector2f tileGutterSize = {tileSize.x / 5.f, tileSize.y / 5.f};
    sf::View  tileRender(sf::FloatRect({0.f, 0.f}, sf::Vector2<float>(window.getSize())));
    tileRender.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));

    const sf::Time animationDuration = sf::milliseconds(200);
    const float maxOffset = 10.f;

    if ( theBoard.isPreformingAction ) {
      theBoard.animationElapsedTime += dTime;

      float progress = std::clamp(theBoard.animationElapsedTime.asSeconds() / animationDuration.asSeconds(), 0.0f, 1.0f);

      if ( progress >= 1.0f ) {
        theBoard.endAnimation();
      } else {
        float easedProgress = static_cast<float>(easeSmoothOut.Solve(progress));
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

    if ( inGame ) {
      window.setView(tileRender);
    } else {
      gameTexture.setView(tileRender);
    }

    for ( int row = 0; row < 4; row++ ) {
      for ( int col = 0; col < 4; col++ ) {
        RoundedRectangleShape tile(tileSize, radius, 4);
        tile.setOrigin(tile.getLocalBounds().getCenter());

        // tile.setPosition({tileGutterSize.x * static_cast<float>(col + 1) + (tileSize.x * static_cast<float>(col)), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f});
        tile.setPosition({tileGutterSize.x * static_cast<float>(col) + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + tileGutterSize.x + (tileSize.x * static_cast<float>(col)) + (tile.getGlobalBounds().size.x / 2.f), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + (tile.getGlobalBounds().size.y / 2.f)});
        tile.setFillColor(theBoard.getTile(row, col).getColor()); // #ccc0b4
        if ( theBoard.getTile(row, col).isNumber() ) {
          float progress = theBoard.getTile(row, col).getAnimationProgress();
          float easedProgress = static_cast<float>(tileScaleEase.Solve(progress));
          float scale = 0.5f + 0.5f * easedProgress;
          tile.setScale({scale, scale});
          theBoard.getTile(row, col).increaseAnimation();
          if ( inGame ) {
            window.draw(tile);
          } else {
            gameTexture.draw(tile);
          }
        }

        // theBoard.getTile(row, col).getIndex()

        if ( theBoard.getTile(row, col).isNumber() ) {
          sf::Text number = textVector[theBoard.getTile(row, col).getIndex()];
          number.setOrigin(number.getLocalBounds().getCenter());
          number.setPosition({tileGutterSize.x * static_cast<float>(col) + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + tileGutterSize.x + (tileSize.x * static_cast<float>(col)) + (tileSize.x / 2.f), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + (tileSize.y / 2.f)});
          number.setScale(theBoard.getTile(row, col).getSize());
          if ( inGame ) {
            window.draw(number);
          } else {
            gameTexture.draw(number);
          }
        }
      }
    }
    if ( inGame ) {
      window.setView(mainView);
    } else {
      gameTexture.setView(mainView);
    }

    for ( int row = 0; row < 4; row++ ) {
      for ( int col = 0; col < 4; col++ ) {
        if ( !theBoard.getTile(row, col).isNumber() ) {
          RoundedRectangleShape tile(tileSize, radius, 4);
          tile.setOrigin(tile.getLocalBounds().getCenter());
          tile.setPosition({tileGutterSize.x * static_cast<float>(col) + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + tileGutterSize.x + (tileSize.x * static_cast<float>(col)) + (tile.getGlobalBounds().size.x / 2.f), tileGutterSize.y * static_cast<float>(row + 1) + (tileSize.y * static_cast<float>(row)) + 64.f + (((1.0f - boardPercentSize) / 2.0f) * 512.f) + (tile.getGlobalBounds().size.y / 2.f)});
          tile.setFillColor(theBoard.getTile(row, col).getColor()); // #ccc0b4
          if ( inGame ) {
            window.draw(tile);
          } else {
            gameTexture.draw(tile);
          }
        }
      }
    }

    // Thanks Claude
    for ( auto it = theBoard.scoreEffects.begin(); it != theBoard.scoreEffects.end(); ) {
      if ( it->updatePosition(dTime) ) {
        it = theBoard.scoreEffects.erase(it); // erase returns next valid iterator
        // std::cout << "deleted" << std::endl;
      } else {
        window.draw(it->getText());
        ++it;
      }
    }

    if ( !inGame ) {
      sf::RectangleShape background(static_cast<sf::Vector2f>(window.getSize()));
      background.setFillColor(sf::Color(0x00, 0x00, 0x00, 150));
      gameTexture.draw(background);
      gameTexture.display();

      renderMenu(gameTexture, dTime, window);
    } else {
      menuProgress = sf::seconds(0);
    }
  }
  sf::Texture restartArrow;

  void renderMenu(const sf::RenderTexture &gameTexture, sf::Time dTime, sf::RenderWindow &window) {
    if (menuProgress < sf::seconds(2)) {
      menuProgress += dTime;
    }

    float blurProgress = static_cast<float>(linear.Solve(menuProgress.asSeconds() / 5.f));
    float blurAmount = 0.f + 5.f * blurProgress;
    // Make the background blured
    sf::Sprite gameSprite(gameTexture.getTexture());
    blurShader.setUniform("colorTexture", sf::Shader::CurrentTexture);
    blurShader.setUniform("parameters", sf::Glsl::Vec2(blurAmount, blurAmount));
    window.draw(gameSprite, &blurShader);

    sf::View  menuView(sf::FloatRect({0.f, 0.f}, sf::Vector2<float>(window.getSize())));
    menuView.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
    float zoomProgress = static_cast<float>(tileScaleEase.Solve(menuProgress.asSeconds() / 5.f));
    float zoomAmount = 0.f + 1.f * zoomProgress;
    menuView.zoom(zoomAmount);
    window.setView(menuView);

    if ( !hasLost ) {
      // Draw the title text
      sf::Vector2f gameSize(static_cast<sf::Vector2f>(gameTexture.getSize()));
      sf::Text title(getFont());
      title.setString("2048");
      title.setCharacterSize(64);

      // Position and draw it
      title.setOrigin(title.getLocalBounds().getCenter());
      title.setPosition({gameSize.x / 2.f, gameSize.y / 2.5f});
      window.draw(title);


      // Make the play button
      RoundedRectangleShape playButton({96.f, 96.f}, 24.0f, 12);
      playButton.setOrigin(playButton.getLocalBounds().getCenter());
      playButton.setPosition({gameSize.x / 2.f, (gameSize.y / 2.5f) + (gameSize.y / 5.f)});

      // Animate Button
      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      float easedProgress = 0;
      float progress = 0;
      if ( animationProgress + dTime < sf::seconds(1.f) && growing ) {
        animationProgress += dTime;
        easedProgress = static_cast<float>(buttonScaleIn.Solve(animationProgress.asSeconds()));
      } else if ( animationProgress - dTime > sf::seconds(0.f) ) {
        growing = false;
        animationProgress -= dTime;
        easedProgress = static_cast<float>(buttonScaleIn.Solve(animationProgress.asSeconds()));
      } else {
        growing = true;
      }

      float scaleAmount = 1.f + 0.25f * easedProgress;

      // Finish drawing the button
      playButton.setScale({scaleAmount, scaleAmount});
      playButton.setFillColor(sf::Color(0xf6, 0x7c, 0x5f)); // #f67c5f
      window.draw(playButton);

      // Make the triangle
      const float padding = 24.f;
      sf::ConvexShape triangle(3);
      triangle.setPoint(0, {0.f, 0.f});
      triangle.setPoint(1, {playButton.getLocalBounds().size.x - padding, 0.f});
      triangle.setPoint(2, {((playButton.getLocalBounds().size.x - padding) / 2.f), ((playButton.getLocalBounds().size.y - padding) / 2.f)});
      triangle.setFillColor(sf::Color(0xf9, 0xf6, 0xf2)); // #f9f6f2

      // Position the triangle
      triangle.setOrigin(triangle.getLocalBounds().getCenter());
      triangle.setPosition({(gameSize.x / 2.f), (gameSize.y / 2.5f) + (gameSize.y / 5.f)});
      triangle.setRotation(sf::degrees(-90.f));
      triangle.setScale({scaleAmount, scaleAmount});
      window.draw(triangle);
    } else {
      sf::Vector2f gameSize(static_cast<sf::Vector2f>(gameTexture.getSize()));
      sf::Text title(getFont());
      title.setString("You Lost.");
      title.setCharacterSize(64);
      title.setOrigin(title.getLocalBounds().getCenter());
      title.setPosition({gameSize.x / 2.f, gameSize.y / 2.5f});
      window.draw(title);

      sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
      float easedProgress = 0;
      if ( animationProgress + dTime < sf::seconds(1.f) && growing ) {
        animationProgress += dTime;
        easedProgress = static_cast<float>(buttonScaleIn.Solve(animationProgress.asSeconds()));
      } else if ( animationProgress - dTime > sf::seconds(0.f) ) {
        growing = false;
        animationProgress -= dTime;
        easedProgress = static_cast<float>(buttonScaleIn.Solve(animationProgress.asSeconds()));
      } else {
        growing = true;
      }
      float scaleAmount = 1.f + 0.25f * easedProgress;

      RoundedRectangleShape restartButton({96.f, 96.f}, 24.0f, 12);
      restartButton.setOrigin(restartButton.getLocalBounds().getCenter());
      restartButton.setPosition({gameSize.x / 2.f, (gameSize.y / 2.5f) + (gameSize.y / 5.f)});
      restartButton.setScale({scaleAmount, scaleAmount});
      restartButton.setFillColor(sf::Color(0xf6, 0x7c, 0x5f));
      window.draw(restartButton);


      sf::Sprite restart(restartTexture);
      restart.setOrigin(restart.getLocalBounds().getCenter());
      restart.setPosition({gameSize.x / 2.f, (gameSize.y / 2.5f) + (gameSize.y / 5.f)});
      restart.setScale({scaleAmount, scaleAmount});

      sf::Vector2f targetSize(80.f, 80.f);
      sf::Vector2f textureSize(restartTexture.getSize());
      restart.setScale({(targetSize.x + scaleAmount)/ textureSize.x, (targetSize.y + scaleAmount) / textureSize.y});
      restart.scale({scaleAmount, scaleAmount});


      window.draw(restart);
    }
  }


  gfx::CubicBezier easeSmoothOut{0.25, 0.1, 0.25, 1.0};
  gfx::CubicBezier tileScaleEase{0.33f, 1.0f, 0.68f, 1.0f};

  // gfx::CubicBezier buttonScaleIn{0.f, 0.36f, 0.1f, 1.0f};
  // gfx::CubicBezier buttonScaleIn{0.f, 0.1f, 0.36f, 1.0f};
  gfx::CubicBezier buttonScaleIn{0.79f, 0.16f, 0.13f, 0.95f};
  gfx::CubicBezier buttonScaleOut{0.f, 0.1f, 1.f, 0.37f};
  gfx::CubicBezier linear{0.07f, 0.7f, 0.0f, 1.0f};

private:
  bool inGame = false;
  bool hasLost = false;
  sf::Shader blurShader;
  sf::Time animationProgress;
  bool growing = true;
  sf::Time menuProgress;
  sf::Time waitForLose;
};


#endif
