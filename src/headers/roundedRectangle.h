// Thanks, Claude

#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape
{
public:
    explicit RoundedRectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0),
                                   const float radius = 0,
                                   const unsigned int cornerPointCount = 8)
    {
        mySize = size;
        myRadius = radius;
        myCornerPointCount = cornerPointCount;
        update();
    }

    void setSize(const sf::Vector2f& size)
    {
        mySize = size;
        update();
    }

    void setRadius(const float radius)
    {
        myRadius = radius;
        update();
    }

    std::size_t getPointCount() const override
    {
        return myCornerPointCount * 4;
    }

    sf::Vector2f getPoint(const std::size_t index) const override
    {
        if(index >= myCornerPointCount * 4)
            return {0,0};

        float deltaAngle = 90.0f / (myCornerPointCount - 1);
        sf::Vector2f center;
        unsigned int centerIndex = index / myCornerPointCount;
        unsigned int pointIndex = index % myCornerPointCount;

        switch(centerIndex)
        {
            case 0: center.x = mySize.x - myRadius; center.y = myRadius; break;
            case 1: center.x = myRadius; center.y = myRadius; break;
            case 2: center.x = myRadius; center.y = mySize.y - myRadius; break;
            case 3: center.x = mySize.x - myRadius; center.y = mySize.y - myRadius; break;
            default: ;
        }

        const float angle = deltaAngle * pointIndex + 90.0f * centerIndex;
        const float x = myRadius * std::cos(angle * 3.14159f / 180.0f);
        const float y = -myRadius * std::sin(angle * 3.14159f / 180.0f);

        return {center.x + x, center.y + y};
    }

private:
    sf::Vector2f mySize;
    float myRadius;
    unsigned int myCornerPointCount;
};