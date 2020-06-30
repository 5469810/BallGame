#include "Box.h"
#include "Box2dConverter.h"
#include <iostream>








Box::Box(b2World & l_world, int l_posX, int l_posY, int l_sizeX, int l_sizeY):
	m_world(l_world),m_break(false),m_broken(false),m_visual(sf::Vector2f( l_sizeX, l_sizeY)), m_visual2(sf::Vector2f(l_sizeX, l_sizeY)), m_visual3(sf::Vector2f(l_sizeX, l_sizeY)),m_exist(true)
{
	if (!m_soundBuff.loadFromFile("Assets/Audio/Sound/Explosion.wav"))
	{
		throw std::runtime_error("Failed to load Explosion.wav");
	}
	m_sound.setBuffer(m_soundBuff);

	m_bodyDef.type = b2_dynamicBody;
	m_bodyDef.position.Set(pixelsToMeters(l_posX), pixelsToMeters(l_posY));
	m_polygonShape.SetAsBox(pixelsToMeters(l_sizeX / 2), pixelsToMeters(l_sizeY / 2), b2Vec2(pixelsToMeters( (l_sizeX/2 )), pixelsToMeters((l_sizeY / 2))), 0);
	m_body = m_world.CreateBody(&m_bodyDef);
	m_fixtureDef.shape = &m_polygonShape;
	m_fixtureDef.density = 15;

	m_fixture1=m_body->CreateFixture(&m_fixtureDef);
	m_polygonShape2.SetAsBox(pixelsToMeters(l_sizeX / 2), pixelsToMeters(l_sizeY / 2), b2Vec2(pixelsToMeters((l_sizeX / 2)), pixelsToMeters(-(l_sizeY / 2))), 0);
	m_fixtureDef2.shape = &m_polygonShape2;
	m_fixtureDef2.density = 15;
	m_fixture2 = m_body->CreateFixture(&m_fixtureDef2);
	m_polygonShape3.SetAsBox(pixelsToMeters(l_sizeX / 2), pixelsToMeters(l_sizeY / 2), b2Vec2(pixelsToMeters((l_sizeX / 2)), pixelsToMeters(-( (l_sizeY *3)/2 ))), 0);
	m_fixtureDef3.shape = &m_polygonShape3;
	m_fixtureDef3.density = 15;
	m_fixture3 = m_body->CreateFixture(&m_fixtureDef3);
	m_body->SetUserData(this);
	m_visual.setOrigin( l_sizeX,0);
	m_visual2.setOrigin(l_sizeX,0);
	m_visual3.setOrigin(l_sizeX,0);
	m_visual.setFillColor(sf::Color::Yellow);
	m_visual2.setFillColor(sf::Color::Green);
	m_visual3.setFillColor(sf::Color::Red);

}

Box::~Box()
{
	if (m_body)
	{
		m_world.DestroyBody(m_body);
		m_body = nullptr;
	}
	if (m_body2)
	{
		m_world.DestroyBody(m_body2);
		m_body2 = nullptr;
	}
	if (m_body3)
	{
		m_world.DestroyBody(m_body3);
		m_body3 = nullptr;
	}
}

void Box::update()
{
	if (m_break)
	{
		collapse();

		m_break = false;
		m_broken = true;
	}
	if (m_broken==false)
	{
		m_angVel = m_body->GetAngularVelocity();
		m_linVel = m_body->GetLinearVelocity();
	}
	if (!m_break)
	{
		const b2Transform &transform = m_body->GetTransform();
		int32 count = m_polygonShape.GetChildCount();
		b2Vec2 vertex = b2Mul(transform, m_polygonShape.m_vertices[count]);
		m_visual.setPosition(metersToPixels(vertex.x), metersToPixels(vertex.y));

		int32 count2 = m_polygonShape2.GetChildCount();
		b2Vec2 vertex2 = b2Mul(transform, m_polygonShape2.m_vertices[count2]);
		m_visual2.setPosition(metersToPixels(vertex2.x), metersToPixels(vertex2.y));
		int32 count3 = m_polygonShape3.GetChildCount();
		b2Vec2 vertex3 = b2Mul(transform, m_polygonShape3.m_vertices[count3]);
		m_visual3.setPosition(metersToPixels(vertex3.x), metersToPixels(vertex3.y));
		float angle = m_body->GetAngle();
		m_visual.setRotation(radToDeg(angle));
		m_visual2.setRotation(radToDeg(angle));
		m_visual3.setRotation(radToDeg(angle));
	}
	if(m_broken)
	{
		const b2Transform &transform = m_body->GetTransform();
		const b2Transform &transform2 = m_body2->GetTransform();
		const b2Transform &transform3 = m_body3->GetTransform();
		int32 count = m_polygonShape.GetChildCount();
		b2Vec2 vertex = b2Mul(transform, m_polygonShape.m_vertices[count]);
		m_visual.setPosition(metersToPixels(vertex.x), metersToPixels(vertex.y));

		int32 count2 = m_polygonShape2.GetChildCount();
		b2Vec2 vertex2 = b2Mul(transform2, m_polygonShape2.m_vertices[count2]);
		m_visual2.setPosition(metersToPixels(vertex2.x), metersToPixels(vertex2.y));

		int32 count3 = m_polygonShape3.GetChildCount();
		b2Vec2 vertex3 = b2Mul(transform3, m_polygonShape3.m_vertices[count3]);
		m_visual3.setPosition(metersToPixels(vertex3.x), metersToPixels(vertex3.y));

		float angle = m_body->GetAngle();
		float angle2 = m_body2->GetAngle();
		float angle3 = m_body3->GetAngle();
		m_visual.setRotation(radToDeg(angle));
		m_visual2.setRotation(radToDeg(angle2));
		m_visual3.setRotation(radToDeg(angle3));
		
		

	}
	
	
}

void Box::collapse()
{
	m_body->DestroyFixture(m_fixture2);
	m_fixture2 = nullptr;
	m_body->DestroyFixture(m_fixture3);
	m_fixture3 = nullptr;
	m_bodyDef.type = m_body->GetType();
	m_bodyDef.position = m_body->GetPosition();
	m_bodyDef.angle = m_body->GetAngle();
	m_body2 = m_world.CreateBody(&m_bodyDef);
	m_body2->SetUserData(this);
	m_fixture2=m_body2->CreateFixture(&m_fixtureDef2);
	m_body3 = m_world.CreateBody(&m_bodyDef);
	m_fixture3=m_body3->CreateFixture(&m_fixtureDef3);
	m_body3->SetUserData(this);
	b2Vec2 center2 = m_body2->GetWorldCenter();
	b2Vec2 center1 = m_body->GetWorldCenter();
	b2Vec2 center3 = m_body3->GetWorldCenter();
	b2Vec2 velocity = m_linVel + b2Cross(m_angVel, center1 - center1);
	b2Vec2 velocity2 = m_linVel + b2Cross(m_angVel, center2 - center1);
	b2Vec2 velocity3 = m_linVel + b2Cross(m_angVel, center3 - center1);
	m_body->SetLinearVelocity(velocity);
	m_body2->SetLinearVelocity(velocity2);
	m_body->SetAngularVelocity(m_angVel);
	m_body2->SetAngularVelocity(m_angVel);
	m_body3->SetLinearVelocity(velocity3);
	m_body3->SetAngularVelocity(m_angVel);

}

void Box::startBreak()
{
	m_break = true;
}

void Box::playSound()
{
	m_sound.play();
}

b2Body * Box::getBody() const
{
	return m_body;
}

bool Box::getBroken() const
{
	return m_broken;
}

bool Box::exist() const
{
	return m_exist;
}

void Box::draw(sf::RenderTarget & l_target, sf::RenderStates l_states) const
{
	l_target.draw(m_visual, l_states);
	l_target.draw(m_visual2, l_states);
	l_target.draw(m_visual3, l_states);
}
