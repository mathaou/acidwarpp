#include "palette.hpp"
#include "display.hpp"
#include "palinit.hpp"

void Palette::rotate_backward(int color, uint8_t* palette)
{
	int temp, x;

	temp = palette[((254) * 3) + 3 + color];

	for( x = (254); x >= 1; --x )
		palette[(x * 3) + 3 + color] = palette[(x * 3) + color];

	palette[(1 * 3) + color] = temp;
}


void Palette::rotate_forward(int color, uint8_t* palette)
{
	int temp;
	int x;

	temp = palette[(1 * 3) + color];
	for( x = 1; x < (256); ++x )
		palette[x * 3 + color] = palette[(x * 3) + 3 + color];

	palette[((256) * 3) - 3 + color] = temp;
}

int Palette::fade_palette_array_to_white(uint8_t* current)
{
	int palByteNum, num_white = 0;

	for( palByteNum = 3; palByteNum < 768; ++palByteNum )
	{
		if( current[palByteNum] < 63 )
			++current[palByteNum];
		else
			++num_white;
	}

	return ((num_white >= 765) ? DONE : NOT_DONE);
}

int Palette::fade_palette_array_to_black(uint8_t* current)
{
	int palByteNum, num_black = 0;

	for( palByteNum = 3; palByteNum < 768; ++palByteNum )
	{
		if( current[palByteNum] > 0 )
			--current[palByteNum];
		else
			++num_black;
	}

	return ((num_black >= 765) ? DONE : NOT_DONE);
}

int Palette::fade_palette_array_to_target(uint8_t* current, uint8_t* target)
{
	int palByteNum, num_equal = 0;

	for( palByteNum = 3; palByteNum < 768; ++palByteNum )
	{
		if( current[palByteNum] < target[palByteNum] )
			++current[palByteNum];
		else if( current[palByteNum] > target[palByteNum] )
			--current[palByteNum];
		else
			++num_equal;
	}

	return ((num_equal >= 765) ? DONE : NOT_DONE);
}

void Palette::roll_primary_palette(uint8_t* palette)
{
	maybe_invert_sub_palette_roll_direction();
	roll_rgb_palette(palette);
	apply();
}

void Palette::roll_fade_primary_palette(enum FadeMode mode, uint8_t* current, uint8_t* target)
{
	if( !state.check_flag(PaletteState::FadeComplete) )
	{
		bool success = false;
		switch( mode )
		{
			case Palette::Black:
				success = fade_palette_array_to_black(current) == DONE;
				break;
			case Palette::White:
				success = fade_palette_array_to_white(current) == DONE;
				break;
			case Palette::Target:
				success = fade_palette_array_to_target(current, target) == DONE;
				break;
			default:
				break; // do nothing
		}

		if (success ) state.set_flag(PaletteState::FadeComplete);

		if( mode != Palette::Target )
		{
			roll_primary_palette(current);
		}
		else
		{
			maybe_invert_sub_palette_roll_direction();
			roll_rgb_palette(current);
			roll_rgb_palette(target);
			apply();
		}
	}
	else if( mode == Palette::Target )
	{
		roll_primary_palette(current);
	}

	state.current_mode = (int) mode;
}

void Palette::roll_primary_random(uint8_t* current, uint8_t* target)
{
	roll_fade_primary_palette((FadeMode) RANDOM(FadeMode::NUM_MODE - 1), current, target);
}

void Palette::roll_rgb_palette(uint8_t* current)
{
	auto red = state.check_flag(PaletteState::RedRollDirection);
	auto green = state.check_flag(PaletteState::GreenRollDirection);
	auto blue = state.check_flag(PaletteState::BlueRollDirection);

	if( !red )
		rotate_forward(RED, current);
	else
		rotate_backward(RED, current);

	if( !green )
		rotate_forward(GREEN, current);
	else
		rotate_backward(GREEN, current);

	if( !blue )
		rotate_forward(BLUE, current);
	else
		rotate_backward(BLUE, current);
}

void Palette::maybe_invert_sub_palette_roll_direction()
{
	switch( RANDOM(DIRECTN_CHANGE_PERIOD_IN_TICKS) )
	{
		case RED:
			state.toggle_flag(PaletteState::RedRollDirection);
			break;
		case GREEN:
			state.toggle_flag(PaletteState::GreenRollDirection);
			break;
		case BLUE:
			state.toggle_flag(PaletteState::BlueRollDirection);
			break;
	}
}

void Palette::create(int t)
{
	if( t != -1 )
	{
		state.current = t;
		init_palette(target, (Type) t);
		return;
	}

	state.current = RANDOM(NUM_PALETTE_TYPES);
	init_palette(target, (Type) state.current);
}

void Palette::fade_in_and_rotate()
{
	if( !state.check_flag(PaletteState::FadeComplete) )
	{
		roll_fade_primary_palette(Palette::Target, current, target);
	}
	else
	{
		roll_primary_palette(current);
	}
}

void Palette::begin_fade_out(int to_black)
{
	state.fade_direction = to_black || RANDOM(2) == 0;
	state.clear_flag(PaletteState::FadeComplete);
}

int Palette::fade_out()
{
	roll_fade_primary_palette((FadeMode) state.fade_direction, current);
	auto complete = state.check_flag(PaletteState::FadeComplete);
	return complete;
}

void Palette::init(int logo)
{
	if( logo )
	{
		init_palette(current, RGBW_LIGHTNING);
		std::memcpy(target, current, sizeof(target));
	}
	else
	{
		std::memset(current, '\0', sizeof(current));
	}

	apply();
}

void Palette::apply()
{
	gDisplay.set_palette(current);
}

void Palette::init_palette(uint8_t* palette, enum Type type)
{
	init_test_palette(palette);

	switch( type % 4 )
	{
		case RGBW:
			init_rgbw_palette(palette);
			break;
		case W:
			init_w_palette(palette);
			break;
		case W_HALF:
			init_w_half_palette(palette);
			break;
		case PASTEL:
			init_pastel_palette(palette);
			break;
		default:
			init_rgbw_palette(palette);
			break;
	}

	if( type > 4 )
		add_sparkles_to_palette(palette, 9); // hardcoded amount
}
