#pragma once

#include <stdint.h>

#include "handy.hpp"
#include "SDL/include/SDL.h"

struct timer_data
{
	SDL_cond* cond;
	SDL_mutex* mutex;
	SDL_TimerID timer_id;
	SDL_bool flag;
};

struct TimerState
{
private:
	TimerState();
	~TimerState();
public:

	static TimerState& instance()
	{
		static TimerState instance;
		return instance;
	}

	struct timer_data timer_data = { NULL, NULL, 0, SDL_FALSE };

	void lock();
	void unlock();

	static uint32_t process(uint32_t interval, void* param);

	void wait();
};