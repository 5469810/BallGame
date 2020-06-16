#include "Walls.h"
#include "Box2dConverter.h"





Walls::Walls(b2World &l_world, int l_posX, int l_posY, int l_sizeX, int l_sizeY):
	m_world(l_world),m_visual(sf::Vector2f(l_sizeX,l_sizeY))
{
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(pixelsToMeters(l_posX), pixelsToMeters(l_posY));
	b2PolygonShape shape;
	shape.SetAsBox(pixelsToMeters(l_sizeX / 2), pixelsToMeters(l_sizeY / 2), b2Vec2(pixelsToMeters(l_sizeX / 2), pixelsToMeters(l_sizeY / 2)), 0);
	m_body = l_world.CreateBody(&bd);
	m_body->CreateFixture(&shape, 1);
	b2Vec2 pos= m_body->GetPosition();
	m_visual.setPosition(metersToPixels(pos.x), metersToPixels(pos.y));
	m_visual.setFillColor(sf::Color(128, 128, 128));
	

}

void Walls::setColor(sf::Color  l_color)
{
	m_visual.setFillColor(l_color);
}

b2Body * Walls::getBody()
{
	return m_body;
}

void Walls::update()
{
	b2Vec2 pos = m_body->GetPosition();
	m_visual.setPosition(metersToPixels(pos.x), metersToPixels(pos.y));
}

Walls::~Walls()
{
	if (m_body)
	{
		m_world.DestroyBody(m_body);
		m_body = nullptr;
	}
}



void Walls::draw(sf::RenderTarget & l_target, sf::RenderStates l_states) const
{
	l_target.draw(m_visual, l_states);
}
