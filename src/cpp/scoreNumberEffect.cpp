//
// Created by jackson-diekmann on 2/17/26.
//

#include "../headers/scoreNumberEffect.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <random>
sf::Font scoreNumberEffect::font;


scoreNumberEffect::scoreNumberEffect(int value) : text(getFont()) {
  if ( !font.openFromFile("../clear-sans/ClearSans-Bold.ttf") ) {
    std::cerr << "Failed to load font (failed to create the font face)" << std::endl;
    std::cerr << "Provided path: \"Blue Winter.ttf\"" << std::endl;
  }

  this->value = value;
  // Set initial velocities
  this->velocity.x = generateRandomNumber(10, 30);
  this->velocity.y = generateRandomNumber(50, 100) * -1.f;

  // Set acceleration
  this->acceleration.x = this->velocity.x * 0.5f;
  this->acceleration.y = 175;

  // Set angle
  this->angle = sf::degrees(generateRandomNumber(0, 2));


  // Set how long the text stays alive
  this->lifetime = static_cast<int>(generateRandomNumber(75, 125));

  // Create the sprite
  text.setString("+ " + std::to_string(this->value));
  // text.setFillColor(sf::Color(0x57, 0x57, 0x57)); // #575757
  text.setFillColor(sf::Color::White);
  text.setCharacterSize(24);
  text.setOrigin(text.getLocalBounds().getCenter());
  text.rotate(this->angle);
}

float scoreNumberEffect::generateRandomNumber(const int min, const int max) { // Generates a random integer given a min and a max
  static std::random_device dev;
  static std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
  const unsigned long output = dist(rng);
  return static_cast<float>(output);
}

bool scoreNumberEffect::updatePosition(sf::Time dTime) {
  // First update velocities
  this->velocity.x += this->acceleration.x * dTime.asMilliseconds() * 0.001f;

  this->velocity.y += this->acceleration.y * dTime.asMilliseconds() * 0.001f;

  // Now update Positions
  this->position.x += this->velocity.x * dTime.asMilliseconds() * 0.001f;
  this->position.y += this->velocity.y * dTime.asMilliseconds() * 0.001f;
  this->text.setPosition(this->position);

  // Rotate
  this->text.rotate(sf::degrees(dTime.asMilliseconds() * 0.0075f));
  sf::Vector2f temp = this->text.getScale();
  temp.x += 0.01f;
  temp.y += 0.01f;
  this->text.setScale(temp);

  // Fade over time
  sf::Color color = text.getFillColor();
  color.a -= (color.a / this->lifetime) + 1;
  this->text.setFillColor(color);

  // this->duration++;
  return !(static_cast<int>(color.a) > 20);
}
