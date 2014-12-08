//
//  Game.h
//
//  Created by Marco Conte on 04/12/14.
//  Copyright (c) 2014 Marco Conte. All rights reserved.
//


#ifndef SFML__Game__H
#define SFML__Game__H


#include "IGame.h"



/** Game abstract class. */
class Game : public IGame
{
public:
	
	/** Initializes a new instance of the class. */
	Game(VideoMode mode, const String& title, Uint32 style = Style::Default,
		  const ContextSettings& settings = ContextSettings());
	
	/** Runs the game. */
	virtual void run();
	
	
protected:
	
	/* frame rate: 60 frames per seconds */
	const Time TimePerFrame = seconds(1.f / 60.f);
	
	
	/** Process events. */
	virtual void processEvents();
	
	
	/* Main window */
	RenderWindow window;
	
	/* Pause state */
	bool IsPaused;
	
};




#endif
