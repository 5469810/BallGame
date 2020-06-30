#include "GameState.h"
#include "Game.h"
#include <iostream>





GameState::GameState(Game *l_game):
	m_game(l_game)
{
}

Game * GameState::getGame() const
{
	return m_game;
}

MainMenuState::MainMenuState(Game *l_game,sf::Font &l_font):
	GameState(l_game),m_font(l_font),m_buttonSize(sf::Vector2f(100,50)),m_padding(4)
{
	m_mainMenuPos = sf::Vector2f(getGame()->getWind().getSize().x / 2, 250);
	m_mainMenuText.setFont(m_font);
	m_mainMenuText.setPosition(m_mainMenuPos);
	m_mainMenuText.setString("Main Menu");
	sf::FloatRect rect = m_mainMenuText.getLocalBounds();
	m_mainMenuText.setOrigin((rect.left+ rect.width) / 2, (rect.top + rect.height) / 2);
	std::string labels[2];
	labels[0] = "Play";
	labels[1] = "Quit";
	m_buttonPos.x = m_mainMenuPos.x;
	m_buttonPos.y = m_mainMenuPos.y+50;
	for (int i = 0; i < 2; ++i)
	{
		m_rect[i].setSize(m_buttonSize);
		m_rect[i].setPosition(m_buttonPos.x, m_buttonPos.y+(i*(m_buttonSize.y + m_padding)));
		m_rect[i].setOrigin(m_buttonSize.x/ 2, m_buttonSize.y/2);
		m_rect[i].setFillColor(sf::Color::Yellow);
		m_label[i].setFont(m_font);
		m_label[i].setString(labels[i]);
		m_label[i].setPosition(m_buttonPos.x, m_buttonPos.y +(i*(m_buttonSize.y+ m_padding)));
		sf::FloatRect rect2 = m_label[i].getLocalBounds();
		m_label[i].setOrigin((rect2.left + rect2.width) /2 ,( rect2.top + rect2.height)/2 );
		m_label[i].setFillColor(sf::Color::Black);
	}

}

void MainMenuState::render(sf::RenderWindow &l_wind)
{
	l_wind.draw(m_mainMenuText);
	for (int i = 0; i < 2; ++i)
	{
		l_wind.draw(m_rect[i]);
	}
	for (int i = 0; i < 2; ++i)
	{
		l_wind.draw(m_label[i]);

	}
}

void MainMenuState::update(sf::Time l_time)
{
	
}

void MainMenuState::moveStick(sf::Vector2i l_dir)
{
}


void MainMenuState::click()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(getGame()->getWind());
	float halfX = m_buttonSize.x / 2;
	float halfY = m_buttonSize.y / 2;
	for (int i = 0; i < 2; ++i)
	{
		if (mousePos.x >= m_rect[i].getPosition().x - halfX && mousePos.x <= m_rect[i].getPosition().x + halfX &&
			mousePos.y >= m_rect[i].getPosition().y - halfY && mousePos.y <= m_rect[i].getPosition().y + halfY)
		{
			if (i == 0)
			{
				getGame()->changeGameState(GameState::States::Playing);
				getGame()->getWind().setMouseCursorVisible(false);
			}
			else if (i == 1)
			{
				getGame()->getWind().close();
			}
		}
	}
}




PlayingState::PlayingState(Game *l_game, sf::Font &l_font):
	GameState(l_game), m_font(l_font), m_world(b2Vec2(0, 15)),
	m_worldSize(getGame()->getWind().getSize().x / SCALE, getGame()->getWind().getSize().y / SCALE), m_player(m_world, m_playerPos.x, m_playerPos.y), m_start(true), m_period(sf::Time::Zero),
	m_timeBuff(sf::seconds(1)), m_playerPos(sf::Vector2i(getGame()->getWind().getSize().x / 2, getGame()->getWind().getSize().y - (SCALE / 2)))
{
	int offsetY = 50;
	int offsetY2 = 100;
	int offsetY3 = 20;
	setCooldownText(offsetY);
	m_world.SetContactListener(this);
	
	setBoundaries();
	loadLevel();
}

void PlayingState::render(sf::RenderWindow & l_wind)
{
	m_world.DrawDebugData();
	if (m_cooldown)
	{
		l_wind.draw(m_cooldownText);
	}
	l_wind.draw(m_player);
	for (auto &itr : m_boundaries)
	{
		l_wind.draw(*itr);
	}
	for (auto &itr : m_platforms)
	{
		l_wind.draw(*itr);
	}
	for (auto &itr : m_bodies)
	{
		l_wind.draw(*itr);
	}
}

void PlayingState::update(sf::Time l_time)
{
	m_timeSinceJump += l_time;
	if (m_timeSinceJump > m_timeBuff)
	{
		m_cooldown = true;
	}
	else if (m_timeSinceJump < m_timeBuff)
	{
		m_cooldown = false;
	}
	float32 timestep = 1 / 60.0f;
	int32 posIter = 3;
	int32 velIter = 8;
	m_world.Step(timestep, velIter, posIter);
	m_player.update(l_time);
	for (auto &itr : m_bodies)
	{
		itr->update();
	}
	for (auto &itr : m_platforms)
	{
		itr->update();
	}

	for (auto itr = 0; itr < m_bodies.size(); ++itr)
	{
		if (!m_bodies.at(itr)->getBroken())
		{

			return;
		}
		if (itr == m_bodies.size() - 1)
		{
			m_extraTime += l_time;
			if (m_extraTime > sf::seconds(5))
			{

				loadLevel();
				m_extraTime = sf::Time::Zero;
			}
		}
	}
}

void PlayingState::moveStick(sf::Vector2i l_dir)
{
	if (l_dir == sf::Vector2i(1, 0))
	{
		if (m_timeSinceJump > m_timeBuff)
		{
			m_player.move(b2Vec2(14, -14));
			m_timeSinceJump = sf::Time::Zero;
		}
	}
	else if (l_dir == sf::Vector2i(-1, 0))
	{
		if (m_timeSinceJump > m_timeBuff)
					{
						m_player.move(b2Vec2(-14, -14));
						m_timeSinceJump = sf::Time::Zero;
		}
	}
	else if (l_dir == sf::Vector2i(0, -1)) 
	{
		if (m_timeSinceJump > m_timeBuff)
					{
						m_player.move(b2Vec2(-14, 14));
						m_timeSinceJump = sf::Time::Zero;
					}
	}
	else if (l_dir == sf::Vector2i(0, 1))
	{
		if (m_timeSinceJump > m_timeBuff)
			{
				m_player.move(b2Vec2(14, 14));
				m_timeSinceJump = sf::Time::Zero;
			}
	}
}


void PlayingState::menu()
{
	getGame()->getWind().setMouseCursorVisible(true);
	getGame()->changeGameState(GameState::MainMenu);
}

void PlayingState::PostSolve(b2Contact * l_contact, const b2ContactImpulse * l_impulse)
{
	void* userDataA = l_contact->GetFixtureA()->GetBody()->GetUserData();
	void* userDataB = l_contact->GetFixtureB()->GetBody()->GetUserData();
	if (userDataA)
	{
		Box* boxA = static_cast<Box*>(userDataA);

		int32 count = l_contact->GetManifold()->pointCount;
		float maxImpulse = 0;
		for (int i = 0; i < count; ++i)
		{
			maxImpulse = b2Max(maxImpulse, l_impulse->normalImpulses[i]);
		}
		if (boxA->getBroken())
		{
			if (maxImpulse > 15)
			{
				boxA->playSound();
			}
			return;
		}
		if (maxImpulse > 20)
		{
			if (boxA->exist())
			{
				boxA->startBreak();
				boxA->playSound();

			}
		}
		Player *playerA = static_cast<Player*>(userDataA);
		int32 count2 = l_contact->GetManifold()->pointCount;
		float maxImpulse2 = 0;
		for (int32 i = 0; i < count2; ++i)
		{
			maxImpulse2 = b2Max(maxImpulse2, l_impulse->normalImpulses[i]);
		}
		if (maxImpulse2 > 2)
		{
			if (playerA->exist())
			{
				if (!m_win)
				{
					playerA->playSound();
				}
			}
		}

	}
	if (userDataB)
	{
		Box* boxB = static_cast<Box*>(userDataB);
		if (boxB->getBroken())
		{

			return;
		}
		int32 count = l_contact->GetManifold()->pointCount;
		float maxImpulse = 0;
		for (int i = 0; i < count; ++i)
		{
			maxImpulse = b2Max(maxImpulse, l_impulse->normalImpulses[i]);
		}
		if (boxB->getBroken())
		{
			if (maxImpulse > 15)
			{
				boxB->playSound();
			}
			return;
		}
		if (maxImpulse > 20)
		{
			if (boxB->exist())
			{
				boxB->startBreak();
				boxB->playSound();
			}
		}

		Player *playerB = static_cast<Player*>(userDataB);
		int32 count2 = l_contact->GetManifold()->pointCount;
		float maxImpulse2 = 0;
		for (int32 i = 0; i < count2; ++i)
		{
			maxImpulse2 = b2Max(maxImpulse2, l_impulse->normalImpulses[i]);
		}
		if (maxImpulse2 > 2)
		{
			if (playerB->exist())
			{
				if (!m_win)
				{
					playerB->playSound();
				}
			}
		}

	}
}

void PlayingState::setBoundaries() 
{
	m_boundaries.push_back(std::make_unique<Walls>(m_world, 0, 0, SCALE, SCALE *m_worldSize.y));
	m_boundaries.push_back(std::make_unique<Walls>(m_world, 0, (m_worldSize.y - 1)* SCALE, m_worldSize.x * SCALE,SCALE));
	m_boundaries.push_back(std::make_unique<Walls>(m_world, (m_worldSize.x - 1)*SCALE, 0, SCALE, (m_worldSize.y - 1)*SCALE));
	m_boundaries.push_back(std::make_unique<Walls>(m_world, 0, 0, SCALE *m_worldSize.x, SCALE));
}

void PlayingState::loadLevel()
{
	m_level++;
	if (!m_platforms.empty())
	{
		m_platforms.clear();
	}
	if (!m_bodies.empty())
	{
		m_bodies.clear();
	}
	int offsetX = 20;
	int offsetY = 1;
	int offset = SCALE * 4;
	int offset2 = SCALE * 5;
	for (int i = 0; i < 3; ++i)
	{
		bool valid = false;
		int randomPosX = -1;
		int randomPosY = -1;
		while (valid == false)
		{
			valid = true;
			randomPosX = random(offset,getGame()->getWind().getSize().x - offset);
			randomPosY = random(offset, getGame()->getWind().getSize().y - offset);

			for (auto &itr : m_platforms)
			{
				sf::Vector2f itrPos = b2VecTosfVec(itr->getBody()->GetPosition());
				if (randomPosX <= itrPos.x + offset2 && randomPosX >= itrPos.x - offset2 &&
					randomPosY <= itrPos.y + offset2 && randomPosY >= itrPos.y - offset2)
				{
					//std::cout << "false";
					valid = false;
					break;
				}
			}
		}
		auto ptr = std::make_unique<Walls>(m_world, randomPosX, randomPosY, PLATFORMSIZE.x, PLATFORMSIZE.y);
		b2Vec2 pos = ptr->getBody()->GetPosition();
		m_platforms.push_back(std::move(ptr));
		m_bodies.push_back(std::make_unique<Box>(m_world, metersToPixels(pos.x) + offsetX, metersToPixels(pos.y) - offsetY, BOXSIZE.x, BOXSIZE.y));
	}
	m_player.setPos(m_playerPos.x, m_playerPos.y);
}

void PlayingState::setCooldownText(int l_offsetY)
{
	m_cooldownText.setFont(m_font);
	m_cooldownText.setString("GO!");
	m_cooldownText.setOrigin(m_cooldownText.getLocalBounds().width / 2, m_cooldownText.getLocalBounds().height / 2);
	m_cooldownText.setPosition(getGame()->getWind().getSize().x / 2, l_offsetY);
}






