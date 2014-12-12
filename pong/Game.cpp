//
//  Game.cpp
//
//  Created by Marco Conte on 04/12/14.
//  Copyright (c) 2014 Marco Conte. All rights reserved.
//

#include "Game.h"



Game::Game(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
	:	window(mode, title, style, settings),
		TimePerFrame(seconds(1.f / 60.f)),
		IsPaused(false)
{
}


void Game::run()
{
	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;
	
	// start the main application loop
	while (window.isOpen())
	{
		processEvents();
		
		timeSinceLastUpdate += clock.restart();
		
		// fixed time steps
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			
			if (!IsPaused)
				update(TimePerFrame);
		}
		
		render();
	}
}


void Game::processEvents()
{
	Event e;
	
	// loop while popping the event on top of the event queue
	while (window.pollEvent(e))
	{
		// handle the window closing event
		if (e.type == Event::Closed)
			window.close();
	}
}

