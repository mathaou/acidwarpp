#pragma once

#include <stdint.h>
#include <vector>

#include "state.hpp"

#define RED 0
#define GREEN 1
#define BLUE 2
#define DIRECTN_CHANGE_PERIOD_IN_TICKS 256

struct Palette
{
private:
	Palette() = default;
	~Palette() = default;
public:
	enum FadeMode
	{
		White,
		Black,
		Target,
		Random,
		NUM_MODE
	};

	enum Type
	{
		RGBW,
		W,
		W_HALF,
		PASTEL,
		RGBW_LIGHTNING,
		W_LIGHTNING,
		W_HALF_LIGHTNING,
		PASTEL_LIGHTNING,
		NUM_PALETTE_TYPES // this should always be last
	};

	static Palette& instance()
	{
		static Palette instance;
		return instance;
	}

	void create(int target = -1);
	void fade_in_and_rotate();
	void begin_fade_out(int to_black);
	int fade_out();
	void init(int logo);
	void init_palette(uint8_t* palette, enum Type type);
	void apply();

	const char* get_current_palette_name(int target = -1)
	{
		static const char* names[] = {
			"RGBW",
			"W",
			"W_HALF",
			"PASTEL",
			"RGBW_LIGHTNING",
			"W_LIGHTNING",
			"W_HALF_LIGHTNING",
			"PASTEL_LIGHTNING",
		};

		if( target != -1 )
			return names[target];

		return names[state.current];
	}

	const char* getCurrentFadeMode()
	{
		static const char* names[] = {
			"White",
			"Black",
			"Target",
			"Random",
		};

		return names[state.current_mode];
	}

	void rotate_forward(int color, uint8_t* pal);
	void rotate_backward(int color, uint8_t* pal);

	void roll_rgb_palette(uint8_t* current);
	void maybe_invert_sub_palette_roll_direction();
	int fade_palette_array_to_white(uint8_t* current);
	int fade_palette_array_to_black(uint8_t* current);
	int fade_palette_array_to_target(uint8_t* current, uint8_t* target);

	void roll_primary_random(uint8_t* current,
										 uint8_t* target);
	void roll_primary_palette(uint8_t* current);
	void roll_fade_primary_palette(enum FadeMode mode, uint8_t* current, uint8_t* target = nullptr);

	// 3 channels for RGB
	uint8_t current[NUM_COLORS * 3];
	uint8_t target[NUM_COLORS * 3];

	PaletteState state;
};