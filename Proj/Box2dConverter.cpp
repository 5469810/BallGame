#include "Box2dConverter.h"



double pixelsToMeters(const double l_x)
{
	return l_x / PIXELS_PER_METERS;
}

double metersToPixels(const double l_x)
{
	return l_x * PIXELS_PER_METERS;
}

double degToRad(const double l_x)
{
	return PI * l_x / 180.0;
}

double radToDeg(const double l_x)
{
	return 180.0*l_x / PI;
}


b2Vec2 sfVecTob2Vec(const sf::Vector2i l_vec)
{
	return b2Vec2(l_vec.x / PIXELS_PER_METERS, l_vec.y / PIXELS_PER_METERS);
}
sf::Vector2f b2VecTosfVec(const b2Vec2 & l_vec)
{

	return sf::Vector2f(l_vec.x *PIXELS_PER_METERS, l_vec.y *PIXELS_PER_METERS);

}