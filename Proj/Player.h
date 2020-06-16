#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const double PLAYERSIZE = 32;



class Player:public sf::Drawable
{
public:
	Player(b2World &l_world,int l_posX, int l_posY);
	~Player();
	void move(b2Vec2 l_dir);
	void update(sf::Time l_time);
	void setPos(int l_posX, int l_posY);
	void playSound();
	bool exist()const;
	sf::Sound getSound()const;
private:
	void draw(sf::RenderTarget &l_target, sf::RenderStates l_states)const override;
	b2Body *m_body;
	b2World &m_world;
	b2Vec2 m_velocity;
	sf::CircleShape m_visual;
	sf::Sound m_sound;
	sf::SoundBuffer m_soundBuff;
	bool m_exist;
};

