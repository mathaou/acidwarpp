#include "draw.hpp"
#include "handy.hpp"
#include "display.hpp"
#include "bitmap.hpp"
#include "gen_img.hpp"

AcidwarpDrawingContext::AcidwarpDrawingContext()
{
	_image_function_list = make_shuffled_list(pattern_names.size());

	if( !(_draw_state.mutex = SDL_CreateMutex()) ||
	   !(_draw_state.done_condition = SDL_CreateCond()) ||
	   !(_draw_state.next_condition = SDL_CreateCond()) )
	{
		_errlog("could not create drawing synchronziation primatives");
		exit(-1);
	}

	if( (_drawing_thread = SDL_CreateThread(
		_drawing_main,
		"DrawingThread",
		this)) == nullptr )
	{
		_errlog("creating drawing thread");
		exit(-1);
	}
}

int AcidwarpDrawingContext::_drawing_main(void* param)
{
	auto context = static_cast<AcidwarpDrawingContext*>(param);
	int displayed_image;
	int draw_image = context->_image_function_list[context->_image_function_list_index];
	displayed_image = draw_image;
	
	auto state = &context->_draw_state;

	for( ;;)
	{
		context->draw(draw_image);

		SDL_LockMutex(state->mutex);
		state->done = (SDL_bool) true;
		SDL_CondSignal(state->done_condition);

		while( !state->next )
		{
			SDL_CondWait(state->next_condition, state->mutex);
		}

		state->next = (SDL_bool) false;
		SDL_UnlockMutex(state->mutex);

		if( state->check_flag(DrawState::Quit) )
			break;

		if( state->check_flag(DrawState::Redraw) )
		{
			draw_image = displayed_image;
			state->clear_flag(DrawState::Redraw);
		}
		else
		{
			if( !context->target )
				context->draw_advance();
			else
				context->target = false;

			displayed_image = draw_image;
			draw_image = context->_image_function_list[context->_image_function_list_index];
		}
	}

	return 0;
}

void AcidwarpDrawingContext::abort()
{
	if( _drawing_thread )
	{
		SDL_LockMutex(_draw_state.mutex);

		while( !_draw_state.done )
		{
			_draw_state.set_flag(DrawState::Abort);
			SDL_CondWait(_draw_state.done_condition, _draw_state.mutex);
			_draw_state.clear_flag(DrawState::Abort);
		}

		SDL_UnlockMutex(_draw_state.mutex);
	}
}

void AcidwarpDrawingContext::draw(int img)
{
	uint8_t* buffer;
	unsigned int buffer_stride, width, height;

	gDisplay.begin_update(&buffer, &buffer_stride, &width, &height);
	
	if( img < 0 )
	{
		write_bitmap_image_to_array(buffer, NOAHS_FACE, width, height, buffer_stride);
	}
	else
	{
		generate_image_float(img, buffer, width / 2, height / 2, width, height, NUM_COLORS, buffer_stride, _draw_state._state_flags & DRAW_SCALED);
	}

	gDisplay.finish_update();
}

void AcidwarpDrawingContext::draw_next()
{
	SDL_LockMutex(_draw_state.mutex);

	while( !_draw_state.done )
	{
		SDL_CondWait(_draw_state.done_condition, _draw_state.mutex);
	}

	gDisplay.swap_buffers();

	_draw_state.next = (SDL_bool) true;
	_draw_state.done = (SDL_bool) false;

	SDL_CondSignal(_draw_state.next_condition);
	SDL_UnlockMutex(_draw_state.mutex);
}

void AcidwarpDrawingContext::draw_continue()
{
	SDL_LockMutex(_draw_state.mutex);
	_draw_state.next = (SDL_bool) true;
	_draw_state.done = (SDL_bool) false;
	SDL_CondSignal(_draw_state.next_condition);
	SDL_UnlockMutex(_draw_state.mutex);
}

void AcidwarpDrawingContext::draw_same()
{
	_draw_state.set_flag(DrawState::Redraw);
	draw_continue();
	draw_next();
}

void AcidwarpDrawingContext::draw_advance(int idx)
{
	_draw_state._state_flags &= ~DRAW_LOGO;

	if( idx != -1 )
	{
		_image_function_list_index = idx;
		return;
	}

	if( ++_image_function_list_index >= _image_function_list.size() )
	{
		_image_function_list_index = 0;
		_image_function_list = make_shuffled_list(pattern_names.size());
	}
}

AcidwarpDrawingContext::~AcidwarpDrawingContext()
{
	if( _drawing_thread )
	{
		int status;
		_draw_state.set_flag(DrawState::Quit);
		abort();
		draw_continue();
		SDL_WaitThread(_drawing_thread, &status);
		_drawing_thread = NULL;
	}
	if( _draw_state.done_condition != NULL )
	{
		SDL_DestroyCond(_draw_state.done_condition);
		_draw_state.done_condition = NULL;
	}
	if( _draw_state.next_condition != NULL )
	{
		SDL_DestroyCond(_draw_state.next_condition);
		_draw_state.next_condition = NULL;
	}
	if( _draw_state.mutex != NULL )
	{
		SDL_DestroyMutex(_draw_state.mutex);
		_draw_state.mutex = NULL;
	}
}