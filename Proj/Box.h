#pragma once
#include "Box2D/Box2D.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



const sf::Vector2f BOXSIZE = sf::Vector2f(16, 24);



class Box:public sf::Drawable
{
public:
	Box(b2World &l_world, int l_posX, int l_posY, int l_sizeX, int l_sizeY);
	~Box();
	void update();
	void collapse();
	void startBreak();
	void playSound();
	b2Body *getBody()const;
	bool getBroken()const;
	bool exist()const;
private:
	void draw(sf::RenderTarget &l_target, sf::RenderStates l_states)const override;
	b2Body *m_body;
	b2Body *m_body2;
	b2Body *m_body3;
	b2World &m_world;
	b2Fixture *m_fixture1;
	b2Fixture *m_fixture2;
	b2Fixture *m_fixture3;
	b2BodyDef m_bodyDef;
	b2FixtureDef m_fixtureDef;
	b2FixtureDef m_fixtureDef2;
	b2FixtureDef m_fixtureDef3;
	b2PolygonShape m_polygonShape;
	b2PolygonShape m_polygonShape2;
	b2PolygonShape m_polygonShape3;
	float m_angVel;
	b2Vec2 m_linVel;
	bool m_broken;
	bool m_exist;
	bool m_break;
	sf::RectangleShape m_visual;
	sf::RectangleShape m_visual2;
	sf::RectangleShape m_visual3;
	std::vector<b2Vec2> m_vertices;
	sf::Sound m_sound;
	sf::SoundBuffer m_soundBuff;
};

