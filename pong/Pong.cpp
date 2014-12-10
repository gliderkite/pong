//
//  Pong.cpp
//  pong
//
//  Created by Marco Conte on 04/12/14.
//  Copyright (c) 2014 Marco Conte. All rights reserved.
//

#include "Pong.h"
#include "ResourcePath.hpp"

#include <stdexcept>
#include <string>
#include <cmath>
using namespace std;


namespace pong {


Pong::Pong(const ContextSettings& settings)
	:	Game(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, Style::Close, settings),
		ball(Vector2f(BALL_SIZE, BALL_SIZE)),
		player1(Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)),
		player2(Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)),
		ply1score(0),
		ply2score(0),
		mt(rd()),
		Yrnd(WINDOW_HEIGHT / 5, WINDOW_HEIGHT - WINDOW_HEIGHT / 5),
		hitrnd(-PLAYER_HEIGHT / 2.5f, PLAYER_HEIGHT / 2.5f),
		state(State::Splash),
		ballDestYply1(-1), ballDestYply2(-1),
		aidelayrnd(AI_MIN_DELAY, AI_MAX_DELAY)
{
	window.setFramerateLimit(FRAME_RATE);
	
	// init the font
	if (!font.loadFromFile(resourcePath() + FONT_FILENAME))
		throw runtime_error("Unable to load the font");
	// init sounds buffers
	if (!bufferPlop.loadFromFile(resourcePath() + PLOP_SOUND_FILENAME))
		throw runtime_error("Unable to load the plop sound");
	if (!bufferBeep.loadFromFile(resourcePath() + BEEP_SOUND_FILENAME))
		throw runtime_error("Unable to load the peep sound");
	
	// inti ai delay
	aiDelay = aidelayrnd(mt);
	
	// init the separator
	for (size_t i = 0; i < separator.size(); i++)
	{
		separator[i].setSize(Vector2f(SEPARATOR_WIDTH, SEPARATOR_HEIGHT));
		separator[i].setFillColor(Color::White);
		separator[i].setPosition(WINDOW_WIDTH / 2 - SEPARATOR_WIDTH / 2, i * SEPARATOR_DIST + 10);
	}
	
	// init scores
	initScore(ply1Score);
	ply1Score.setPosition(WINDOW_WIDTH / 2 - SCORE_ABSCISSA - ply1Score.getLocalBounds().width, SCORE_ORDINATE);
	initScore(ply2Score);
	ply2Score.setPosition(WINDOW_WIDTH / 2 + SCORE_ABSCISSA, SCORE_ORDINATE);
	
	// init sounds
	soundPlop.setBuffer(bufferPlop);
	soundBeep.setBuffer(bufferBeep);
	
	// init title
	title.setFont(font);
	title.setString("PONG");
	title.setCharacterSize(FONT_TITLE_SIZE);
	title.setColor(Color::White);
	title.setPosition(WINDOW_WIDTH / 2 - title.getLocalBounds().width / 2, TITLE_ORDINATE);
	
	// init start "button"
	start.setFont(font);
	start.setString("Press ENTER to start the game");
	start.setCharacterSize(FONT_START_SIZE);
	start.setColor(Color::White);
	start.setPosition(WINDOW_WIDTH / 2 - start.getLocalBounds().width / 2, START_ORDINATE);
	
	// init tribute
	tribute.setFont(font);
	tribute.setString("A tribute to the original Atari PONG\n\t\tFrom Marco Conte 2014");
	tribute.setCharacterSize(FONT_TRIBUTE_SIZE);
	tribute.setColor(Color::White);
	tribute.setPosition(WINDOW_WIDTH / 2 - tribute.getLocalBounds().width / 2, TRIBUTE_ORDINATE);
	
	// init demo
	demo.setFont(font);
	demo.setString("Space Bar for Demo");
	demo.setCharacterSize(FONT_TRIBUTE_SIZE);
	demo.setColor(Color::White);
	demo.setPosition(WINDOW_WIDTH / 2 - demo.getLocalBounds().width / 2, DEMO_ORDINATE);
	
	// init win
	win.setFont(font);
	win.setString("WIN");
	win.setCharacterSize(FONT_WIN_SIZE);
	win.setColor(Color::White);
	
	// init new game
	newGame.setFont(font);
	newGame.setString("Press Enter to start\n\t   a new game");
	newGame.setCharacterSize(FONT_TRIBUTE_SIZE);
	newGame.setColor(Color::White);
	
	initEntities();
}


Pong::Movement::Movement()
	: up(false), down(false)
{
}

	

void Pong::processEvents()
{
	Event e;
	
	// loop while popping the event on top of the event queue
	while (window.pollEvent(e))
	{
		// handle the event according to its type
		switch (e.type)
		{
			case Event::KeyPressed:
				handlePlayerInput(e.key.code, true);
				break;
			case Event::KeyReleased:
				handlePlayerInput(e.key.code, false);
				break;
			case Event::Closed:
				window.close();
				break;
			default:
				break;
		}
	}
}


void Pong::update(const Time& deltaTime)
{
	if (state == State::Splash)
		return;
	
	float ms = deltaTime.asMilliseconds();
	
	updateBall(ms);
	
	// in demo state AI plays instead of the user
	if (state == State::Demo)
		updateAI(player2, ms);
	else
		updatePlayer(player2, ms);
	
	updateAI(player1, ms);
}
	
	
void Pong::updateBall(float ms)
{
	Vector2f ballPos = ball.getPosition();
	Vector2f ply1Pos = player1.getPosition();
	Vector2f ply2Pos = player2.getPosition();
	
	// hit the roof or the floor
	if (ballPos.y <= 0 || ballPos.y >= WINDOW_HEIGHT - BALL_SIZE)
	{
		// reverse y component of velocity
		ballVelocity.y *= -1;
		soundPlop.play();
	}
	// player 2 tipping point
	else if (ballPos.x + BALL_SIZE >= ply2Pos.x)
	{
		// reset ball destination
		ballDestYply1 = -1;
		
		// hit player 2
		if (ballVelocity.x > 0 && ballPos.x < ply2Pos.x &&
			 ballPos.y + BALL_SIZE >= ply2Pos.y && ballPos.y <= ply2Pos.y + PLAYER_HEIGHT)
		{
			// compute speed rate according to the point of impact
			float dist = (ply2Pos.y + PLAYER_HEIGHT / 2) - (ballPos.y + BALL_SIZE / 2);
			float speedRate = 1 + abs(dist) / (PLAYER_HEIGHT / 2);
			float newSpeed = abs(ballVelocity.x * speedRate);
			
			// set speed lower and upper bounds
			if (newSpeed < BALL_MIN_SPEED)
				newSpeed = BALL_MIN_SPEED;
			else if (newSpeed > BALL_MAX_SPEED)
				newSpeed = BALL_MAX_SPEED;
			
			// set the new speed
			ballVelocity.x = -newSpeed;
			ballVelocity.y = abs(ballVelocity.x) * -dist / (PLAYER_HEIGHT / 2);
			
			soundPlop.play();
		}
		// player 1 goal
		else if (ballPos.x > WINDOW_WIDTH)
		{
			ply1score++;
			ply1Score.setString(to_string(ply1score));
			ball.setPosition(WINDOW_WIDTH / 2, Yrnd(mt));
			ballVelocity.x = -BALL_MIN_SPEED;
			ballVelocity.y = -BALL_MIN_SPEED / 2;
			
			// add randomness
			if (hitrnd(mt) < 0)
				ballVelocity.y *= -1;
			
			soundBeep.play();
			
			if (isGameOver())
			{
				IsPaused = true;
				win.setPosition(WINDOW_WIDTH / 4 - win.getLocalBounds().width / 2, WIN_ORDINATE);
				newGame.setPosition(WINDOW_WIDTH / 4 - newGame.getLocalBounds().width / 2, NEWGAME_ORDINATE);
			}
		}
	}
	// player 1 tipping point
	else if (ballPos.x <= ply1Pos.x + PLAYER_WIDTH)
	{
		// reset ball destination
		ballDestYply2 = -1;
		
		// hit player 1
		if (ballVelocity.x < 0 && ballPos.x > ply1Pos.x &&
			 ballPos.y + BALL_SIZE >= ply1Pos.y && ballPos.y <= ply1Pos.y + PLAYER_HEIGHT)
		{
			// compute speed rate according to the point of impact
			float dist = (ply1Pos.y + PLAYER_HEIGHT / 2) - (ballPos.y + BALL_SIZE / 2);
			float speedRate = 1 + abs(dist) / (PLAYER_HEIGHT / 2);
			float newSpeed = abs(ballVelocity.x * speedRate);
			
			// set speed lower and upper bounds
			if (newSpeed < BALL_MIN_SPEED)
				newSpeed = BALL_MIN_SPEED;
			else if (newSpeed > BALL_MAX_SPEED)
				newSpeed = BALL_MAX_SPEED;
			
			// set the new speed
			ballVelocity.x = newSpeed;
			ballVelocity.y = abs(ballVelocity.x) * -dist / (PLAYER_HEIGHT / 2);
			
			soundPlop.play();
		}
		// player 2 goal
		else if (ballPos.x < 0)
		{
			ply2score++;
			ply2Score.setString(to_string(ply2score));
			ball.setPosition(WINDOW_WIDTH / 2, Yrnd(mt));
			ballVelocity.x = BALL_MIN_SPEED;
			ballVelocity.y = BALL_MIN_SPEED / 2;
			
			// add randomness
			if (hitrnd(mt) < 0)
				ballVelocity.y *= -1;
			
			soundBeep.play();
			
			if (isGameOver())
			{
				IsPaused = true;
				win.setPosition(3 * WINDOW_WIDTH / 4 - win.getLocalBounds().width / 2, WIN_ORDINATE);
				newGame.setPosition(3 * WINDOW_WIDTH / 4 - newGame.getLocalBounds().width / 2, NEWGAME_ORDINATE);
			}
		}
	}
	
	ball.move(ballVelocity * ms);
}
	

void Pong::updatePlayer(RectangleShape& player, float ms)
{
	Vector2f plyMovement;
	Vector2f plyPos = player.getPosition();
	
	if (isMoving.up && plyPos.y > 0)
		plyMovement.y -= 1.f;
	if (isMoving.down && plyPos.y < WINDOW_HEIGHT - PLAYER_HEIGHT)
		plyMovement.y += 1.f;
	
	player.move(plyMovement * ms * PLAYER_SPEED_FACTOR);
}


void Pong::updateAI(RectangleShape& ai, float ms)
{
	Vector2f aiMovement;
	Vector2f ballPos = ball.getPosition();
	Vector2f aiPos = ai.getPosition();
	
	auto getOtherPlayerScore = [this, &ai]() -> unsigned
	{
		// returns the current store of the opponet player
		return (&ai == &player1 ? ply2score : ply1score);
	};
	
	/* update player 1 position */
	if (isBallApproaching(ai))
	{
		// time that the ball needs to arrive at the same abscissa of player 1
		float tx = abs((getPlayerSurface(ai) - ballPos.x) / ballVelocity.x);
		float ty;
		
		// time that the ball needs to arrive at the same ordinate of roof/floor
		if (ballVelocity.y < 0)
			ty = abs(ballPos.y / ballVelocity.y);
		else
			ty = abs((WINDOW_HEIGHT - ballPos.y - BALL_SIZE) / ballVelocity.y);
		
		// if the ball will hit the roof/floor before to arrive to player 1
		if (tx > ty)
		{
			float displacement = (getOtherPlayerScore() == GOAL - 1 ? 1.f : 0.5f);
			
			if (ballVelocity.y > 0)
			{
				if (aiPos.y < WINDOW_HEIGHT - PLAYER_HEIGHT)
					aiMovement.y += displacement;
			}
			else if (aiPos.y > 0)
				aiMovement.y -= displacement;
		}
		else
		{
			if (getBallDest(ai) < 0)
			{
				// add delay to ai reaction (only if this is not a mach point)
				if (getOtherPlayerScore() < GOAL - 1)
				{
					if (aiDelay > 0)
					{
						aiDelay -= ms;
						return;
					}
					else
						aiDelay = aidelayrnd(mt);
				}
				
				// ball ordinate when it will have the same abscissa of ai
				float destY = ballPos.y + ballVelocity.y * tx;
				
				// add randomness
				if (ballPos.x > PLAYER_POS_UNCERTAINTY && ballPos.x < WINDOW_HEIGHT - PLAYER_POS_UNCERTAINTY)
					destY += hitrnd(mt);
				
				setBallDest(ai, destY);
			}
			
			// middle ai ordinate
			int h = aiPos.y + PLAYER_HEIGHT / 2;
			
			// avoids fickers
			if (abs(getBallDest(ai) - h) > PLAYER_POS_UNCERTAINTY)
			{
				if (getBallDest(ai) > h)
				{
					if (aiPos.y < WINDOW_HEIGHT - PLAYER_HEIGHT)
						aiMovement.y += 1.f;
				}
				else if (getBallDest(ai) < h)
				{
					if (aiPos.y > 0)
						aiMovement.y -= 1.f;
				}
			}
		}
	}
	else
	{
		// move ai towards the center
		int dist = (aiPos.y + PLAYER_HEIGHT / 2) - (WINDOW_HEIGHT / 2);
		
		if (dist > PLAYER_POS_UNCERTAINTY)
			aiMovement.y -= 0.25f;
		else if (dist < PLAYER_POS_UNCERTAINTY)
			aiMovement.y += 0.25f;
	}
	
	ai.move(aiMovement * ms * PLAYER_SPEED_FACTOR);
}


float Pong::getBallDest(const RectangleShape& player)
{
	if (&player == &player1)
		return ballDestYply1;
	
	return ballDestYply2;
}


void Pong::setBallDest(const RectangleShape& player, float value)
{
	if (&player == &player1)
		ballDestYply1 = value;
	
	ballDestYply2 = value;
}


float Pong::getPlayerSurface(const RectangleShape& player)
{
	Vector2f pos = player.getPosition();
	
	if (&player == &player1)
		return pos.x + PLAYER_WIDTH;
	
	return pos.x;
}


bool Pong::isBallApproaching(const RectangleShape& player)
{
	if (&player == &player1)
		return ballVelocity.x < 0;
	
	return ballVelocity.x > 0;
}


void Pong::render()
{
	window.clear();
	
	switch (state)
	{
		case State::Splash:
			window.draw(title);
			window.draw(start);
			window.draw(tribute);
			window.draw(demo);
			break;
			
		case State::Play:
		case State::Demo:
			// draw separator
			for (size_t i = 0; i < separator.size(); i++)
				window.draw(separator[i]);
			
			// draw scores
			window.draw(ply1Score);
			window.draw(ply2Score);
			
			// draw players
			window.draw(player1);
			window.draw(player2);
			
			if (isGameOver())
			{
				window.draw(win);
				window.draw(newGame);
			}
			else
				window.draw(ball);
			
			break;
	}
	
	window.display();
}
	
	
void Pong::handlePlayerInput(Keyboard::Key key, bool isPressed)
{
	// update player motion
	if (key == Keyboard::Up)
		isMoving.up = isPressed;
	else if (key == Keyboard::Down)
		isMoving.down = isPressed;
	// update game state
	else if (!isPressed)
	{
		if (!isGameOver())
		{
			if (key == Keyboard::Escape)
			{
				state = State::Splash;
				reset();
			}
			else if (key == Keyboard::P)
				IsPaused = !IsPaused;
			else if (key == Keyboard::Space)
			{
				state = State::Demo;
				IsPaused = false;
			}
		}
	}
	else if (key == Keyboard::Return)
	{
		state = (state == State::Demo ? State::Demo : State::Play);
		IsPaused = false;
		
		if (isGameOver())
			reset();
	}
}

	
bool Pong::isGameOver()
{
	return ply1score == GOAL || ply2score == GOAL;
}

	
void Pong::reset()
{
	// reset players scores and entities
	ply1score = ply2score = 0;
	ballDestYply1 = ballDestYply2 = -1;
	ply1Score.setString("0");
	ply2Score.setString("0");
	initEntities();
}
	
	
void Pong::initEntities()
{
	// init the ball
	ball.setFillColor(Color::White);
	ball.setPosition(WINDOW_WIDTH / 2, Yrnd(mt));
	ballVelocity.x = -BALL_MIN_SPEED;
	ballVelocity.y = -BALL_MIN_SPEED / 2;
	
	// init players
	player1.setFillColor(Color::White);
	player1.setPosition(PLAYER_ABSCISSA, Yrnd(mt));
	player2.setFillColor(Color::White);
	player2.setPosition(WINDOW_WIDTH - PLAYER_ABSCISSA - PLAYER_WIDTH, Yrnd(mt));
}


void Pong::initScore(Text& plyScore)
{
	plyScore.setFont(font);
	plyScore.setString("0");
	plyScore.setCharacterSize(FONT_SCORE_SIZE);
	plyScore.setColor(Color::White);
}

	
	
}
