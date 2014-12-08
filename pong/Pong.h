//
//  Pong.h
//  pong
//
//  Created by Marco Conte on 04/12/14.
//  Copyright (c) 2014 Marco Conte. All rights reserved.
//

#ifndef __pong__Pong__
#define __pong__Pong__


#include "Game.h"
#include "Settings.h"

#include <array>
#include <random>
using namespace std;

#include <SFML/Audio.hpp>
using namespace sf;


namespace pong
{


/** Pong game class. */
class Pong : public Game
{
	
public:
	
	
	/** Initializes a new instance of the class. */
	Pong(const ContextSettings& settings = ContextSettings());
	
	
	
protected:
	
	
	/** Process events. */
	void processEvents();
	
	/** Update game status. */
	void update(const Time& deltaTime);
	
	/** Render graphics. */
	void render();
	
	
	
private:
	
	
	/** Player movement direction */
	typedef struct Movement
	{
		/** Default consructor. */
		Movement();
		
		bool up;
		bool down;
		
	} Movement;
	
	
	/** Current game state. */
	enum class State
	{
		Splash,
		Play,
		Demo
	};
	
	
	/** Handle player input. */
	void handlePlayerInput(Keyboard::Key key, bool isPressed);
	
	/** Initializes score */
	void initScore(Text& plyScore);
	
	/** Initializes entities:players and ball. */
	void initEntities();
	
	/** Returns true is the game is over. */
	bool isGameOver();
	
	/** Reset score and initializes entities */
	void reset();
	
	/** Update the ball position. */
	void updateBall(float ms);
	
	/** Update the player position. */
	void updatePlayer(RectangleShape& player, float ms);
	
	/** Update AI position. */
	void updateAI(RectangleShape& ai, float ms);
	
	/** Gets the ball destination for the given player. */
	float getBallDest(const RectangleShape& player);
	
	/** Sets the ball destination for the given player. */
	void setBallDest(const RectangleShape& player, float value);
	
	/** Returns the given player surface ordinate. */
	float getPlayerSurface(const RectangleShape& player);
	
	/** Returns true if the ball is approaching the given player. */
	bool isBallApproaching(const RectangleShape& player);
	
	
	
	/* The ball */
	RectangleShape ball;
	Vector2f ballVelocity;
	float ballDestYply1;
	float ballDestYply2;
	
	/* Separator */
	array<RectangleShape, SEPARATORS_NUMBER> separator;
	
	/* Players */
	RectangleShape player1;
	RectangleShape player2;
	
	/* Player movement */
	Movement isMoving;
	
	/* Scores */
	Font font;
	Text ply1Score;
	Text ply2Score;
	unsigned ply1score;
	unsigned ply2score;
	
	/* Splash screen */
	Text title;
	Text start;
	Text tribute;
	Text demo;
	
	/* Game Over */
	Text win;
	Text newGame;
	
	/* Sounds */
	SoundBuffer bufferPlop;
	Sound soundPlop;
	SoundBuffer bufferBeep;
	Sound soundBeep;
	
	/* Random generator */
	random_device rd;
	mt19937 mt;
	uniform_real_distribution<float> Xrnd;
	uniform_real_distribution<float> Yrnd;
	uniform_real_distribution<float> hitrnd;
	
	/* Game state */
	State state;
};

	
}


#endif
