#pragma once

#include "timer.hpp"
#include "SDL/include/SDL.h"

#define DRAW_LOGO 1
#define DRAW_FLOAT 2
#define DRAW_SCALED 4

enum display_state
{
	STATE_INITIAL,
	STATE_NEXT,
	STATE_DISPLAY,
	STATE_FADEOUT
};

class AcidwarpState
{
public:
	void set_flag(int flag)
	{
		_state_flags |= 1UL << flag;
	}

	void clear_flag(int flag)
	{
		_state_flags &= ~(1UL << flag);
	}

	void toggle_flag(int flag)
	{
		_state_flags ^= 1UL << flag;
	}

	bool check_flag(int flag)
	{
		return (_state_flags >> flag) & 1UL;
	}

	int _state_flags = 0;
};

class PaletteState : public AcidwarpState
{
public:
	enum Flag
	{
		RedRollDirection,
		GreenRollDirection,
		BlueRollDirection,
		FadeComplete
	};

	int current;
	int current_mode;
	bool fade_direction;
	bool freeze;
};

class DrawState : public AcidwarpState
{
public:
	enum Flag
	{
		Abort,
		Quit,
		Redraw
	};

	SDL_bool next, done;
	SDL_cond* next_condition, * done_condition;
	SDL_mutex* mutex;
};

class DisplayState : public AcidwarpState
{
public:
	enum Flag
	{
		Go,
		Skip,
		NextPalette,
		Lock,
		Resize
	};

	int rotation_delay = 30000;
	int show_logo = 0;
	int image_time = 10;
	int display_flags = 0;

	bool show_gui;

	enum display_state state = STATE_INITIAL;
};

