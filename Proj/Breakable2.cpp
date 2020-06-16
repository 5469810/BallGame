#include "Breakable2.h"
#include "Box2dConverter.h"
#include <iostream>






Breakable2::Breakable2(b2World &l_world , int l_posX, int l_posY):
	m_world(l_world)
{
	m_bdef.position.Set( pixelsToMeters( l_posX),pixelsToMeters( l_posY));
	m_bdef.type = b2_dynamicBody;
	m_body = m_world.CreateBody(&m_bdef);
	m_shape.SetAsBox(pixelsToMeters(BREAKABLE2SIZE.x/2), pixelsToMeters(BREAKABLE2SIZE.y/2),b2Vec2(pixelsToMeters(BREAKABLE2SIZE.x/2) ,pixelsToMeters(BREAKABLE2SIZE.y/2 ) ) ,0 );
	m_fdef.shape = &m_shape;
	m_fdef.density = 20;
	m_fixture= m_body->CreateFixture(&m_fdef);
	float offsetX = sfdd::SCALE*2;
	float offsetY = sfdd::SCALE;
	double sizeX = BREAKABLE2SIZE.x / 2;
	double sizeY = BREAKABLE2SIZE.y / 2;
	m_shape2.SetAsBox(pixelsToMeters(BREAKABLE2SIZE.x / 2), pixelsToMeters(BREAKABLE2SIZE.y / 2), b2Vec2(pixelsToMeters( sizeX-offsetX), pixelsToMeters(sizeY-offsetY )), degToRad(90));
	m_fdef2.shape = &m_shape2;
	m_fdef2.density = 20;
	m_fixture2= m_body->CreateFixture(&m_fdef2);

	float offsetX2 = sfdd::SCALE * 4;
	m_shape3.SetAsBox(pixelsToMeters(BREAKABLE2SIZE.x / 2), pixelsToMeters(BREAKABLE2SIZE.y / 2), b2Vec2(pixelsToMeters(sizeX - offsetX2), pixelsToMeters(sizeY )), degToRad(0));

	m_fdef3.shape = &m_shape3;
	m_fdef3.density = 20;
	m_fixture3 = m_body->CreateFixture(&m_fdef3);
	m_body->SetUserData(this);
}

Breakable2::~Breakable2()
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

void Breakable2::collapse()
{
	m_body->DestroyFixture(m_fixture2);
	m_fixture2 = nullptr;
	m_body->DestroyFixture(m_fixture3);
	m_fixture3 = nullptr;
	m_bdef.position = m_body->GetPosition();
	m_bdef.angle =m_body->GetAngle();
	
	m_body3 = m_world.CreateBody(&m_bdef);
	m_body3->CreateFixture(&m_fdef3);

	m_body2 = m_world.CreateBody(&m_bdef);
	m_body2->CreateFixture(&m_fdef2);

	b2Vec2 center = m_body->GetWorldCenter();
	b2Vec2 center2 = m_body2->GetWorldCenter();
	b2Vec2 center1 = m_body->GetWorldCenter();
	b2Vec2 center3 = m_body3->GetWorldCenter();
	b2Vec2 velocity = m_linVel + b2Cross(m_angVel, center1 - center);
	b2Vec2 velocity2 = m_linVel + b2Cross(m_angVel, center2 - center);
	b2Vec2 velocity3 = m_linVel + b2Cross(m_angVel, center3 - center);
	m_body->SetLinearVelocity(velocity);
	m_body2->SetLinearVelocity(velocity2);
	m_body->SetAngularVelocity(m_angVel);
	m_body2->SetAngularVelocity(m_angVel);
	m_body3->SetLinearVelocity(velocity3);
	m_body3->SetAngularVelocity(m_angVel);
}

void Breakable2::update()
{
	if (m_break)
	{
		collapse();
		m_break = false;
		m_broken = true;
	}
	if (m_broken)
	{
		m_linVel=m_body->GetLinearVelocity();
		m_angVel= m_body->GetAngularVelocity();
	}

}

bool Breakable2::getBroken() const
{
	return m_broken;
}

void Breakable2::startBreak()
{
	m_break = true;

}
