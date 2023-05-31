#include "timer.hpp"
#include "display.hpp"

#define TIMER_INTERVAL (gDisplay._state.rotation_delay / 1000)

TimerState::TimerState()
{
	_log("creating timer!");
	timer_data.mutex = SDL_CreateMutex();

	if( timer_data.mutex == NULL )
	{
		_errlog("creating timer mutex");
		exit(-1);
	}

	timer_data.cond = SDL_CreateCond();

	if( timer_data.cond == NULL )
	{
		_errlog("creating timer condition variable");
		exit(-1);
	}

	timer_data.timer_id = SDL_AddTimer(TIMER_INTERVAL, process,
										this);
	if( timer_data.timer_id == 0 )
	{
		_errlog("adding timer");
		exit(-1);
	}
}

TimerState::~TimerState()
{
	if( timer_data.timer_id != 0 )
	{
		SDL_RemoveTimer(timer_data.timer_id);
		timer_data.timer_id = 0;
	}
	if( timer_data.cond != NULL )
	{
		SDL_DestroyCond(timer_data.cond);
		timer_data.cond = 0;
	}
	if( timer_data.mutex != NULL )
	{
		SDL_DestroyMutex(timer_data.mutex);
		timer_data.mutex = NULL;
	}
}

uint32_t TimerState::process(uint32_t interval, void* param)
{
	TimerState* t = static_cast<TimerState*>(param);
	t->lock();

	t->timer_data.flag = SDL_TRUE;
	SDL_CondSignal(t->timer_data.cond);

	t->unlock();

	auto time = TIMER_INTERVAL;

	return time ? time : 1;
}


void TimerState::lock()
{
	if( SDL_LockMutex(timer_data.mutex) != 0 )
	{
		_errlog("locking timer mutex");
		exit(-1);
	}
}

void TimerState::unlock()
{
	if( SDL_UnlockMutex(timer_data.mutex) != 0 )
	{
		_errlog("unlocking timer mutex");
		exit(-1);
	}
}

void TimerState::wait()
{
	lock();

	while( !timer_data.flag )
	{
		if( SDL_CondWait(timer_data.cond, timer_data.mutex) != 0 )
		{
			_errlog("waiting on condition");
			exit(-1);
		}
	}

	timer_data.flag = SDL_FALSE;
	unlock();
}