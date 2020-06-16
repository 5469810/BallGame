#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>



const double PIXELS_PER_METERS = 32.0;
const double PI = 3.14159265358979323846;
const double DEGTORAD = 0.0174532925199432957f;
const double RADTODEG = 57.295779513082320876f;

double pixelsToMeters(const double l_x);


double metersToPixels(const double l_x);


double degToRad(const double l_x);


double radToDeg(const double l_x);


b2Vec2 sfVecTob2Vec(const sf::Vector2i l_vec);
sf::Vector2f b2VecTosfVec(const b2Vec2 &l_vec);

