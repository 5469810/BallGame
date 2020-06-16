#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "GameState.h"




const sf::Vector2f PLATFORMSIZE = sf::Vector2f(64, 32);
int random(int l_min, int l_max);



class Game
{
public:
	Game(int l_windSizeX, int l_windSizeY);
	~Game();
	void processEvents();
	void run();
	
	sf::RenderWindow& getWind();
	
	//function helpers
	void changeGameState(GameState::States l_state);
	
private:
	//functions

	//vars
	
	sf::RenderWindow m_wind;
	std::array<std::unique_ptr<GameState>, GameState::Count> m_gameStates;
	GameState *m_currentState;
	sf::Font m_font;

};

