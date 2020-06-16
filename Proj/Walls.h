#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>






class Walls: public sf::Drawable
{
public:
	Walls(b2World &l_world, int l_posX, int l_posY, int l_sizeX, int l_sizeY);
	~Walls();
	void setColor(sf::Color l_color);
	b2Body* getBody();
	void update();

private:
	void draw(sf::RenderTarget& l_target, sf::RenderStates l_states)const override;
	b2Body *m_body;
	b2World &m_world;
	sf::RectangleShape m_visual;
	
};

