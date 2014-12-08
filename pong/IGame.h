//
//  IGame.h
//
//  Created by Marco Conte on 04/12/14.
//  Copyright (c) 2014 Marco Conte. All rights reserved.
//


#ifndef SFML__IGame__H
#define SFML__IGame__H


#include <SFML/Graphics.hpp>
using namespace sf;


/** Game interface. */
class IGame
{
public:
	
	/** Empty virtual destructor for proper cleanup. */
	virtual ~IGame()
	{	}
	
	/** Run the game. */
	virtual void run() = 0;
	
	
protected:
	
	/** Process events. */
	virtual void processEvents() = 0;
	
	/** Update game status. */
	virtual void update(const Time& deltaTime) = 0;
	
	/** Render graphics. */
	virtual void render() = 0;
  
};



#endif
