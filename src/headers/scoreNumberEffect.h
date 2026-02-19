//
// Created by jackson-diekmann on 2/17/26.
//

#ifndef UNTITLED_SCORENUMBEREFFECT_H
#define UNTITLED_SCORENUMBEREFFECT_H
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


class scoreNumberEffect {
  public:
    scoreNumberEffect(int value);
    bool updatePosition(sf::Time dTime);
    sf::Text getText() {
      return text;
    };
  private:
    int value;
    static float generateRandomNumber(int min, int max);


    sf::Vector2f velocity;
    sf::Angle angle;
    sf::Vector2f position = {100.f, 14.f};
    int lifetime;
    sf::Vector2f acceleration;
    float angularVelocity;
    int duration = 0;

    sf::Text text;
    static sf::Font font;

    static sf::Font &getFont() { return scoreNumberEffect::font; }




};


#endif // UNTITLED_SCORENUMBEREFFECT_H
