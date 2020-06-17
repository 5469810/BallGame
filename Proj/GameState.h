#pragma once
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "Box2D-SFML Debug Draw/SFMLDebugDraw.h"
#include "Box2dConverter.h"
#include <vector>
#include "Player.h"
#include "Box.h"
#include <memory>
#include "Walls.h"
#include "SFML/Audio.hpp"
#include <random>
#include <array>
#include "GameState.h"



class Game;

class GameState:public b2ContactListener
{
public:
	GameState(Game *l_game);
	enum  States
	{
		MainMenu,Playing,Count
	};
	Game* getGame()const;
	virtual void moveStick(sf::Vector2i l_dir) = 0;
	virtual void update(sf::Time l_time)=0;
	virtual void render(sf::RenderWindow &l_wind)=0;
	virtual void menu()=0;
	virtual void click() = 0;
	virtual void space()=0;
	void PostSolve(b2Contact *l_contact, const  b2ContactImpulse *l_impulse)override = 0;
private:
	Game *m_game;

};



class MainMenuState:public GameState
{
public:
	MainMenuState(Game *l_game,sf::Font &l_font);
	void render(sf::RenderWindow &l_wind)override;
	void update(sf::Time l_time)override;
	void moveStick(sf::Vector2i l_dir);
	void menu() {};
	void PostSolve(b2Contact *l_contact, const  b2ContactImpulse *l_impulse)override {};
	void click();
	void space() {};
private:
	sf::Text m_mainMenuText;
	sf::Font &m_font;
	sf::Vector2f m_buttonPos;
	sf::Vector2f m_mainMenuPos;
	sf::Vector2f m_buttonSize;
	float m_padding;
	sf::RectangleShape m_rect[2];
	sf::Text m_label[2];
};



class PlayingState:public GameState
{
public:
	PlayingState(Game *l_game, sf::Font &l_font);
	void render(sf::RenderWindow &l_wind)override;
	void update(sf::Time l_time)override;
	void moveStick(sf::Vector2i l_dir);
	void menu();
	void click() {};
	void space()
	{
		loadLevel();
	}
	void PostSolve(b2Contact *l_contact, const  b2ContactImpulse *l_impulse)override;
	void setBoundaries();
	void loadLevel();
	void setCooldownText(int l_offsetY);
private:
	bool m_cool;
	unsigned int m_level;
	b2World m_world;
	SFMLDebugDraw m_debug;
	sf::Vector2i m_worldSize;
	Player m_player;
	sf::Font& m_font;
	std::vector<std::unique_ptr<Box>>m_bodies;
	bool m_win;
	unsigned int m_nextLvl;
	sf::Time m_period;
	bool m_start;
	std::vector<std::unique_ptr< Walls>>m_platforms;
	sf::Time m_extraTime;
	sf::Time m_timeSinceJump;
	sf::Time m_timeBuff;
	sf::Text m_cooldownText;

	sf::Text m_startText;
	bool m_cooldown;
	sf::Text m_winText;
	sf::Text m_winText2;
	sf::Text m_winText3;
	sf::Vector2i m_playerPos;
	std::vector < std::unique_ptr<Walls>>m_boundaries;
};
