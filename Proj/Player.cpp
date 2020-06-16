#include "Player.h"
#include "Box2dConverter.h"
#include <iostream>





Player::Player(b2World & l_world, int l_posX , int l_posY):
	m_world(l_world),m_exist(true)
{
	if (!m_soundBuff.loadFromFile("Assets/Audio/Sound/Jump.wav"))
	{
		throw std::runtime_error("Failed to load Jump.wav");
	}
	m_sound.setBuffer(m_soundBuff);
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pixelsToMeters(l_posX), pixelsToMeters(l_posY));
	b2CircleShape shape;
	shape.m_radius = pixelsToMeters(PLAYERSIZE/2 );
	m_body=l_world.CreateBody(&bd);
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 0.5;
	fd.restitution = 0.9;

	m_body->CreateFixture(&fd);
	
	m_visual.setOrigin(PLAYERSIZE/2, PLAYERSIZE/2);
	m_visual.setRadius(PLAYERSIZE/2);
	m_visual.setFillColor(sf::Color::White);

	m_body->SetUserData(this);
}

Player::~Player()
{
	if (m_body)
	{
		m_world.DestroyBody(m_body);
		m_body = nullptr;
	}

}

void Player::move(b2Vec2 l_dir)
{
	m_velocity = l_dir;
	m_body->SetLinearVelocity(m_velocity);
}


void Player::update(sf::Time l_time)
{
	b2Vec2 pos=  m_body->GetPosition();
	m_visual.setPosition(metersToPixels ( pos.x), metersToPixels(pos.y));
}

void Player::setPos(int l_posX , int l_posY)
{
	m_body->SetTransform(b2Vec2(pixelsToMeters(l_posX) ,pixelsToMeters(l_posY)), 0);
}

void Player::playSound()
{
	m_sound.play();
}

bool Player::exist() const
{
	return m_exist;
}

sf::Sound Player::getSound() const
{
	return m_sound;
}

void Player::draw(sf::RenderTarget & l_target, sf::RenderStates l_states) const
{
	l_target.draw(m_visual, l_states);
}
