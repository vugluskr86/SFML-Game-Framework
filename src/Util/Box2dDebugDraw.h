#pragma once

#include <cmath>

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Box2dDebugDraw : public b2Draw {
  private:
    sf::RenderWindow& window;
    float scale;

  public:
    Box2dDebugDraw(const sf::RenderWindow& window, float scale)
        : window(const_cast<sf::RenderWindow&>(window))
        , scale(scale)
    {
    }

    /// Convert Box2D's OpenGL style color definition[0-1] to SFML's color
    /// definition[0-255], with optional alpha byte[Default - opaque]
    static sf::Color GLColorToSFML(const b2Color& color, sf::Uint8 alpha = 255)
    {
        return sf::Color(static_cast<sf::Uint8>(color.r * 255),
                         static_cast<sf::Uint8>(color.g * 255),
                         static_cast<sf::Uint8>(color.b * 255), alpha);
    }

    /// Convert Box2D's vector to SFML vector [Default - scales the vector up by SCALE
    /// constants amount]
    sf::Vector2f B2VecToSFVec(const b2Vec2& vector, bool scaleToPixels = true)
    {
        return sf::Vector2f(vector.x * (scaleToPixels ? scale : 1.f),
                            vector.y * (scaleToPixels ? scale : 1.f));
    }

    /// Draw a closed polygon provided in CCW order.
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        sf::ConvexShape polygon(vertexCount);
        sf::Vector2f center;
        for (int i = 0; i < vertexCount; i++) {
            // polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
            sf::Vector2f transformedVec = B2VecToSFVec(vertices[i]);
            polygon.setPoint(
                i, sf::Vector2f(
                       std::floor(transformedVec.x),
                       std::floor(transformedVec.y))); // flooring the coords to fix
                                                       // distorted lines on flat surfaces
        } // they still show up though.. but less frequently
        polygon.setOutlineThickness(-1.f);
        polygon.setFillColor(sf::Color::Transparent);
        polygon.setOutlineColor(Box2dDebugDraw::GLColorToSFML(color));

        window.draw(polygon);
    }

    /// Draw a solid closed polygon provided in CCW order.
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        sf::ConvexShape polygon(vertexCount);
        for (int i = 0; i < vertexCount; i++) {
            // polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
            sf::Vector2f transformedVec = B2VecToSFVec(vertices[i]);
            polygon.setPoint(
                i, sf::Vector2f(
                       std::floor(transformedVec.x),
                       std::floor(transformedVec.y))); // flooring the coords to fix
                                                       // distorted lines on flat surfaces
        } // they still show up though.. but less frequently
        polygon.setOutlineThickness(-1.f);
        polygon.setFillColor(Box2dDebugDraw::GLColorToSFML(color, 60));
        polygon.setOutlineColor(Box2dDebugDraw::GLColorToSFML(color));

        window.draw(polygon);
    }

    /// Draw a circle.
    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
    {
        sf::CircleShape circle(radius * scale);
        circle.setOrigin(radius * scale, radius * scale);
        circle.setPosition(Box2dDebugDraw::B2VecToSFVec(center));
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(-1.f);
        circle.setOutlineColor(Box2dDebugDraw::GLColorToSFML(color));

        window.draw(circle);
    }

    /// Draw a solid circle.
    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis,
                         const b2Color& color)
    {
        sf::CircleShape circle(radius * scale);
        circle.setOrigin(radius * scale, radius * scale);
        circle.setPosition(Box2dDebugDraw::B2VecToSFVec(center));
        circle.setFillColor(Box2dDebugDraw::GLColorToSFML(color, 60));
        circle.setOutlineThickness(1.f);
        circle.setOutlineColor(Box2dDebugDraw::GLColorToSFML(color));

        b2Vec2 endPoint = center + radius * axis;
        sf::Vertex line[2] = {
            sf::Vertex(Box2dDebugDraw::B2VecToSFVec(center),
                       Box2dDebugDraw::GLColorToSFML(color)),
            sf::Vertex(Box2dDebugDraw::B2VecToSFVec(endPoint),
                       Box2dDebugDraw::GLColorToSFML(color)),
        };

        window.draw(circle);
        window.draw(line, 2, sf::Lines);
    }

    /// Draw a line segment.
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {
        sf::Vertex line[] = {sf::Vertex(Box2dDebugDraw::B2VecToSFVec(p1),
                                        Box2dDebugDraw::GLColorToSFML(color)),
                             sf::Vertex(Box2dDebugDraw::B2VecToSFVec(p2),
                                        Box2dDebugDraw::GLColorToSFML(color))};

        window.draw(line, 2, sf::Lines);
    }

    /// Draw a transform. Choose your own length scale.
    void DrawTransform(const b2Transform& xf)
    {
        float lineLength = 0.4f;

        /*b2Vec2 xAxis(b2Vec2(xf.p.x + (lineLength * xf.q.c), xf.p.y + (lineLength *
         * xf.q.s)));*/
        b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
        sf::Vertex redLine[] = {
            sf::Vertex(Box2dDebugDraw::B2VecToSFVec(xf.p), sf::Color::Red),
            sf::Vertex(Box2dDebugDraw::B2VecToSFVec(xAxis), sf::Color::Red)};

        // You might notice that the ordinate(Y axis) points downward unlike the one in
        // Box2D testbed That's because the ordinate in SFML coordinate system points
        // downward while the OpenGL(testbed) points upward
        /*b2Vec2 yAxis(b2Vec2(xf.p.x + (lineLength * -xf.q.s), xf.p.y + (lineLength *
         * xf.q.c)));*/
        b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
        sf::Vertex greenLine[] = {
            sf::Vertex(Box2dDebugDraw::B2VecToSFVec(xf.p), sf::Color::Green),
            sf::Vertex(Box2dDebugDraw::B2VecToSFVec(yAxis), sf::Color::Green)};

        window.draw(redLine, 2, sf::Lines);
        window.draw(greenLine, 2, sf::Lines);
    }

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color)
    {
    }
};
