#pragma once

#include <vector>

#include "SDL/include/SDL.h"
#include "state.hpp"

class DrawState;

struct AcidwarpDrawingContext
{
	static AcidwarpDrawingContext& instance()
	{
		static AcidwarpDrawingContext _instance;
		return _instance;
	}

	const std::vector<const char*> pattern_names {
		"Rays + 2D Waves", // 0
		"Rays + 2D Waves Alt.", // 1
		"Untitled 1", // 2
		"Peacock", // 3
		"Untitled 2", // 4
		"2D Wave + Spiral", // 5
		"Peacock, 3 Centers var. 1", // 6
		"Peacock, 3 Centers var. 2", // 7
		"Peacock, 3 Centers var. 3", // 8
		"Five Arm Star", // 9
		"2D Wave var. 1", // 10
		"2D Wave var. 2", // 11
		"Simple Concentric Rings", // 12
		"Simple Rays", // 13
		"Toothed Spiral Sharp", // 14
		"Rings with Sine", // 15
		"Rings with Sine + Sliding Inner Rings", // 16
		"Untitled 3", // 17
		"2D Wave var. 3", // 18
		"2D Wave var. 4", // 19
		"2D Wave Interference var. 1", // 20
		"2D Wave Interference var. 2", // 21
		"2D Wave Interference var. 3", // 22
		"Untitled 4", // 23
		"Untitled 5", // 24
		"Untitled 6", // 25
		"Untitled 7", // 26
		"Untitled 8", // 27
		"Random Curtain of Rain (In Strong Wind)", // 28
		"Untitled 9", // 29
		"Untitled 10", // 30
		"Untitled 11", // 31
		"Untitled 12", // 32
		"Variation on Rain 1", // 33
		"Variation on Rain 2", // 34
		"Untitled 13", // 35
		"Untitled 14", // 36
		"Untitled 15", // 37
		"Screen Interlace", // 38
		"Untitled 16", // 39
		"Untitled 17", // 40
		"RANDOM" // should always be last
	};

	DrawState _draw_state;
	bool target = false;

	const char* get_pattern_name(int idx = -1)
	{
		if( idx != -1 )
			return pattern_names[idx];

		return pattern_names[_image_function_list_index];
	}

	int get_index_of_pattern_in_list(int target)
	{
		auto pos = std::find(_image_function_list.begin(), _image_function_list.end(), target);
		if( pos == _image_function_list.end() )
			return INT_MIN;

		return std::distance(_image_function_list.begin(), pos);
	}

	int _image_function_list_index = 0;

private:
	AcidwarpDrawingContext();
	~AcidwarpDrawingContext();
	static int _drawing_main(void* param);
	
public:
	void abort();
	void draw(int img);
	void draw_next();
	void draw_continue();
	void draw_same();
	void draw_advance(int idx = -1);
private:
	SDL_Thread* _drawing_thread = nullptr;

	std::vector<int> _image_function_list; // random list
};