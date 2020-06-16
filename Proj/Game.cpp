#include "Game.h"
#include <iostream>



Game::Game(int l_windSizeX, int l_windSizeY):
	m_wind(sf::VideoMode(l_windSizeX, l_windSizeY), "...", sf::Style::Default)
{
	if (!m_font.loadFromFile("Assets/Fonts/FranchiseBoldhinted.ttf"))
	{
		throw std::runtime_error("Failed to load FranchiseBoldhinted.ttf");
	}
	m_gameStates[GameState::MainMenu]=std::make_unique< MainMenuState>(this,m_font);
	m_gameStates[GameState::Playing] = std::make_unique<PlayingState>(this, m_font);
	changeGameState(GameState::MainMenu);
	m_wind.setFramerateLimit(60);
}

Game::~Game()
{

}


void Game::run()
{
	sf::Clock clock;
	//sdsad
	sf::Time mainTime = sf::Time::Zero;
	while (m_wind.isOpen())
	{
		mainTime = clock.restart();
		processEvents();
		m_currentState->update(mainTime);
		m_wind.clear();
		 
		m_currentState->render(m_wind);
		m_wind.display();
	}
}

sf::RenderWindow&  Game::getWind() 
{
	return m_wind;
}

void Game::processEvents()
{
	sf::Event sfEvent;
	while (m_wind.pollEvent(sfEvent))
	{

		if (sfEvent.type == sf::Event::Closed)
		{
			m_wind.close();
		}
		if (sfEvent.type == sf::Event::KeyPressed)
		{
			if (sfEvent.key.code == sf::Keyboard::Enter)
			{
			
			}
			else if (sfEvent.key.code == sf::Keyboard::Escape)
				{
				m_currentState->menu();
				}
			
				else if (sfEvent.key.code == sf::Keyboard::Right)
				{
					
				m_currentState->moveStick(sf::Vector2i(1,0));
				}
				else if (sfEvent.key.code == sf::Keyboard::Left)
				{

					
				m_currentState->moveStick(sf::Vector2i(-1, 0));
				}
				else if (sfEvent.key.code == sf::Keyboard::Up)
				{

					
				m_currentState->moveStick(sf::Vector2i(0, -1));
				}
				else if (sfEvent.key.code ==sf::Keyboard::Down)
				{
				m_currentState->moveStick(sf::Vector2i(0, 1));
				}
				else if (sfEvent.key.code == sf::Keyboard::Space)
				{
				//m_currentState->space();
				}
			
		}
		if (sfEvent.type == sf::Event::MouseButtonPressed)
		{
			if (sfEvent.key.code == sf::Mouse::Left)
			{
				m_currentState->click();
			}
		}
	}
	
}


int random(int l_min, int l_max)
{
	return rand() % (l_max - l_min + 1) + l_min;
}

void Game::changeGameState(GameState::States l_state)
{
	m_currentState = m_gameStates.at(l_state).get();
}
