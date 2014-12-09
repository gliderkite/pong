//
//  Settings.h
//  pong
//
//  Created by Marco Conte on 06/12/14.
//  Copyright (c) 2014 Marco Conte. All rights reserved.
//

#ifndef pong_Settings_h
#define pong_Settings_h


namespace pong {
	
	static constexpr unsigned FRAME_RATE = 60;
	
	static constexpr unsigned WINDOW_WIDTH = 640;
	static constexpr unsigned WINDOW_HEIGHT = 480;
	
	static constexpr const char* WINDOW_TITLE = "PONG";
	
	static constexpr unsigned GOAL = 5;
	
	static constexpr float AI_MIN_DELAY = 200.f;
	static constexpr float AI_MAX_DELAY = 250.f;
	
	static constexpr float BALL_SIZE = 15.f;
	
	static constexpr float PLAYER_WIDTH = 15.f;
	static constexpr float PLAYER_HEIGHT = 70.f;
	static constexpr float PLAYER_ABSCISSA = 10.f;
	static constexpr float PLAYER_SPEED_FACTOR = 0.5f;
	static constexpr float PLAYER_POS_UNCERTAINTY = 5.f;
	
	static constexpr const char* FONT_FILENAME = "minecrafter.ttf";
	static constexpr const char* PLOP_SOUND_FILENAME = "plop.ogg";
	static constexpr const char* BEEP_SOUND_FILENAME = "beep.ogg";
	
	static constexpr float BALL_MAX_SPEED = 0.65f;
	static constexpr float BALL_MIN_SPEED = 0.35f;
	
	static constexpr unsigned SEPARATORS_NUMBER = 10;
	static constexpr float SEPARATOR_WIDTH = 6.f;
	static constexpr float SEPARATOR_HEIGHT = 24.f;
	static constexpr float SEPARATOR_DIST = 64.f;
	
	static constexpr unsigned FONT_SCORE_SIZE = 36;
	static constexpr unsigned FONT_TITLE_SIZE = 128;
	static constexpr unsigned FONT_START_SIZE = 30;
	static constexpr unsigned FONT_TRIBUTE_SIZE = 20;
	static constexpr unsigned FONT_WIN_SIZE = 58;
	
	static constexpr float SCORE_ABSCISSA = 70.f;
	static constexpr float SCORE_ORDINATE = 20.f;
	static constexpr float TITLE_ORDINATE = 40.f;
	static constexpr float START_ORDINATE = 250.f;
	static constexpr float DEMO_ORDINATE = 300.f;
	static constexpr float TRIBUTE_ORDINATE = 400.f;
	static constexpr float WIN_ORDINATE = 100.f;
	static constexpr float NEWGAME_ORDINATE = 200.f;

}


#endif
