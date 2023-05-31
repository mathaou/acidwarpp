#include <string.h>
#include <stdio.h>

#include "acidwarp_gui.hpp"
#include "display.hpp"
#include "palette.hpp"

#include "handy.hpp"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#define display_check(s) _display_state->check_flag(DisplayState::s)
#define display_clear(s) _display_state->clear_flag(DisplayState::s)
#define display_set(s) _display_state->set_flag(DisplayState::s)

static void main_loop()
{
	auto _display_state = &gDisplay._state;

	gDisplay.process_input();

	if( display_check(Resize) )
	{
		display_clear(Resize);
		if( gDisplay._state.state != display_state::STATE_INITIAL )
		{
			gDraw.draw_same();
			gPalette.apply();
		}
	}

	if( display_check(Skip) )
	{
		if( gDisplay._state.state != display_state::STATE_INITIAL )
		{
			gDisplay._state.state = display_state::STATE_NEXT;
		}

		gDisplay._state.show_logo = false;
	}

	if( display_check(NextPalette) )
	{
		if( !gDisplay._state.show_logo || !gPalette.state.freeze )
		{
			gPalette.create();
		}

		display_clear(NextPalette);
	}

	auto show_logo = gDisplay._state.show_logo;

	switch( gDisplay._state.state )
	{
		case STATE_INITIAL:
			gDraw._draw_state._state_flags |= gDisplay._state.show_logo ? DRAW_LOGO : 0;
			gPalette.init(gDisplay._state.show_logo);

			__fallthrough;
		case STATE_NEXT:
			gDraw.draw_next();

			if( !show_logo && !display_check(Skip) && !gPalette.state.freeze )
			{
				gPalette.create();
			}

			gPalette.state.clear_flag(PaletteState::FadeComplete);

			display_clear(Skip);

			AcidwarpDisplay::l_time = time(NULL);
			AcidwarpDisplay::m_time = AcidwarpDisplay::l_time + gDisplay._state.image_time;
			gDisplay._state.state = STATE_DISPLAY;

			__fallthrough;
		case STATE_DISPLAY:
			gPalette.fade_in_and_rotate();
			AcidwarpDisplay::l_time = time(NULL);

			if( AcidwarpDisplay::l_time > AcidwarpDisplay::m_time && !display_check(Lock) )
			{
				gPalette.begin_fade_out(show_logo);
				gDisplay._state.state = STATE_FADEOUT;
			}

			break;
		case STATE_FADEOUT:
			if( gPalette.fade_out() )
			{
				gDisplay._state.show_logo = false;
				gDisplay._state.state = STATE_NEXT;
			}
			break;
	}
}

int main(int argc, char** argv)
{
	RANDOMIZE();
	gDisplay._fullscreen = false;
	(void) gTimer; // init singleton

	_log("entering main loop");
	for( ;;)
	{
		main_loop();
		gTimer.wait();
	}

	return 0;
}