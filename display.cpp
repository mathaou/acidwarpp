#include <string>
#include <chrono>

using namespace std::chrono_literals;

#include "display.hpp"
#include "handy.hpp"
#include "palette.hpp"

bool AcidwarpDisplay::_fullscreen;
time_t AcidwarpDisplay::l_time, AcidwarpDisplay::m_time;

static ImVec4 clear_color = ImVec4(255.0f, 0.00f, 0.00f, 1.00f);

#define palette_check(s) _palette_state->check_flag(PaletteState::s)
#define palette_clear(s) _palette_state->clear_flag(PaletteState::s)
#define palette_set(s) _palette_state->set_flag(PaletteState::s)

static ImVec4 enabled_color = { 1.0f, 1.0f, 1.0f, 1.0f }, disabled_color = { 1.0f, 1.0f, 1.0f, .5f };

#define text_color(f) f ? enabled_color : disabled_color

void AcidwarpDisplay::gui()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	auto dims = gDisplay._screen_dimensions;
	dims.x /= 3;

	ImGui::SetNextWindowSize(dims, ImGuiCond_Always);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always, { 0,0 });
	
	ImGui::Begin("Acidwarp Control Panel", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	ImGui::SeparatorText("Palette");

	auto _palette_state = &gPalette.state;

	auto red_roll = palette_check(RedRollDirection);
	auto green_roll = palette_check(GreenRollDirection);
	auto blue_roll = palette_check(BlueRollDirection);
	auto fade_complete = palette_check(FadeComplete);

	ImGui::TextColored(text_color(red_roll), "Red Roll");
	ImGui::SameLine();
	ImGui::TextColored(text_color(green_roll), "Green Roll");
	ImGui::SameLine();
	ImGui::TextColored(text_color(blue_roll), "Blue Roll");
	ImGui::SameLine();
	ImGui::TextColored(text_color(fade_complete), "Fade Complete");

	ImGui::Text("Current Palette:");
	ImGui::SameLine();
	ImGui::Text(gPalette.get_current_palette_name());

	ImGui::Text("Fade Mode:");
	ImGui::SameLine();
	ImGui::Text(gPalette.getCurrentFadeMode());

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Palette Change Speed: ");
	ImGui::SameLine();

	ImGui::PushButtonRepeat(true);
	if( ImGui::ArrowButton("##left", ImGuiDir_Left) )
		_handle_input(CMD_PAL_SLOWER);
	ImGui::SameLine();
	if( ImGui::ArrowButton("##right", ImGuiDir_Right) )
		_handle_input(CMD_PAL_FASTER);
	ImGui::PopButtonRepeat();
	ImGui::SameLine();
	ImGui::Text("%d", _state.rotation_delay);

	static bool freeze = false;
	ImGui::Checkbox("Freeze Palette", &freeze);
	gPalette.state.freeze = freeze;

	if( ImGui::BeginListBox("##palette_list") )
	{
		for( int i = 0; i < Palette::NUM_PALETTE_TYPES; i++ )
		{
			const bool is_selected = (gPalette.state.current == i);
			if( ImGui::Selectable(gPalette.get_current_palette_name(i), is_selected) )
			{
				gPalette.create(i);
				gDisplay._state.clear_flag(DisplayState::NextPalette);
			}

			if( is_selected )
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	ImGui::SeparatorText("Display && Draw");

	// note: when it comes to time to change the speed time, look at mtime

	ImGui::Text("Current Pattern:");
	ImGui::SameLine();
	ImGui::Text(gDraw.get_pattern_name());

	static bool lock = false;
	ImGui::Checkbox("Lock Pattern", &lock);
	if (lock)
		gDisplay._state.set_flag(DisplayState::Lock);
	else
		gDisplay._state.clear_flag(DisplayState::Lock);

	ImGui::SameLine();

	ImGui::Text("Current ID: %d", gDraw._image_function_list_index);

	if( ImGui::BeginListBox("##pattern_list") )
	{
		for( auto i = 0; i < gDraw.pattern_names.size(); i++ )
		{
			const bool is_selected = (gDraw._image_function_list_index == i);
			if( ImGui::Selectable(gDraw.get_pattern_name(i), is_selected) )
			{
				auto idx = i;
				if( idx < 0 )
					idx = gDraw.pattern_names.size() - 1;

				gDraw._image_function_list_index = idx;
				gDraw.target = true;

				gDisplay._state.state = STATE_NEXT;
			}

			if( is_selected )
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();
	}

	static int image_time = gDisplay._state.image_time;
	ImGui::InputInt("Image Time", &image_time);

	if( image_time != gDisplay._state.image_time )
	{
		gDisplay._state.image_time = image_time;
		AcidwarpDisplay::l_time = time(NULL);
		AcidwarpDisplay::m_time = AcidwarpDisplay::l_time + gDisplay._state.image_time;
	}

	ImGui::End();

	ImGui::Render();
}

void AcidwarpDisplay::create_texture_from_renderer(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Surface* surface, bool _gui)
{
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if( _gui )
	{
		gui();

		SDL_RenderSetScale(
			gDisplay._renderer,
			ImGui::GetIO().DisplayFramebufferScale.x,
			ImGui::GetIO().DisplayFramebufferScale.y
		);

		SDL_SetRenderDrawColor(
			gDisplay._renderer,
			(uint8_t) (clear_color.x * 255),
			(uint8_t) (clear_color.y * 255),
			(uint8_t) (clear_color.z * 255),
			(uint8_t) (clear_color.w * 255)
		);
	}
	else
	{
		SDL_ShowCursor(false);
	}

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_DestroyTexture(texture);
	if( _gui )
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void AcidwarpDisplay::init()
{
	_log("Creating display instance");
	static bool inited;

	uint32_t video_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

	if( _fullscreen )
	{
		_fullscreen = true;
		video_flags |= SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
	}

	if( !inited )
	{
		if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0 )
		{
			throw std::exception(SDL_GetError());
		}

		// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);

		_screen_dimensions = { (float) dm.w, (float) dm.h };

		if( !_fullscreen )
		{
			_screen_dimensions.x /= 2;
			_screen_dimensions.y /= 2;
		}
	}

	SDL_ShowCursor(!_fullscreen);

	auto width = (int) _screen_dimensions.x;
	auto height = (int) _screen_dimensions.y;

	_window = SDL_CreateWindow(
		"Acidwarp | 'topher remix",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width * _scaling_factor, height * _scaling_factor,
		video_flags);

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	if( !inited )
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
		ImGui_ImplSDLRenderer2_Init(_renderer);
		inited = true;
	}

	SDL_RenderSetLogicalSize(_renderer, width, height);

	_allocate_offscreen();

	_handle_input(CMD_RESIZE);

	if( _renderer == nullptr )
	{
		throw std::exception("Error creating SDL_Renderer!");
	}
}

AcidwarpDisplay::AcidwarpDisplay(bool fullscreen)
{
	_fullscreen = fullscreen;
	init();
}

AcidwarpDisplay::~AcidwarpDisplay()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyTexture(_texture);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	SDL_Quit();
}

void AcidwarpDisplay::_handle_input(enum acidwarp_command cmd)
{
	switch( cmd )
	{
		case CMD_PAUSE:
			_state.toggle_flag(DisplayState::Flag::Go);
			break;
		case CMD_SKIP:
			_state.set_flag(DisplayState::Flag::Skip);
			break;
		case CMD_QUIT:
			exit(0); // bail out
			break;
		case CMD_NEWPAL:
			_state.set_flag(DisplayState::Flag::NextPalette);
			break;
		case CMD_LOCK:
			_state.toggle_flag(DisplayState::Flag::Lock);
			break;
		case CMD_PAL_FASTER:
			_state.rotation_delay -= 5000;
			if( _state.rotation_delay <= 1 )
				_state.rotation_delay = 1;
			break;
		case CMD_PAL_SLOWER:
			if( _state.rotation_delay == 1 )
				_state.rotation_delay--;

			_state.rotation_delay += 5000;
			if( _state.rotation_delay > 100000 )
				_state.rotation_delay = 100000;
			break;
		case CMD_RESIZE:
			_state.set_flag(DisplayState::Flag::Resize);
			break;
		case CMD_GUI:
			_state.show_gui = !_state.show_gui;
			break;
	}
}

void AcidwarpDisplay::_process_key(int key, SDL_Keymod mod)
{
	switch( key )
	{
		case SDLK_UP:
			_handle_input(CMD_PAL_FASTER);
			break;
		case SDLK_DOWN:
			_handle_input(CMD_PAL_SLOWER);
			break;
		case SDLK_p:
			_handle_input(CMD_PAUSE);
			break;
		case SDLK_n:
			_handle_input(CMD_SKIP);
			break;
		case SDLK_c:
		case SDLK_PAUSE:
			if( (mod & KMOD_CTRL) == 0 )
				break;
		case SDLK_q:
			_handle_input(CMD_QUIT);
			break;
		case SDLK_k:
			_handle_input(CMD_NEWPAL);
			break;
		case SDLK_l:
			_handle_input(CMD_LOCK);
			break;
		case SDLK_g:
			_handle_input(CMD_GUI);
			break;
		default:
			break;
	}
}

void AcidwarpDisplay::process_input()
{
	SDL_Event event;

	auto width = (int) _screen_dimensions.x;
	auto height = (int) _screen_dimensions.y;

	while( SDL_PollEvent(&event) > 0 )
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch( event.type )
		{
			case SDL_KEYDOWN:
				_process_key(event.key.keysym.sym, SDL_GetModState());
				break;
			case SDL_WINDOWEVENT:
				if( event.window.event == SDL_WINDOWEVENT_RESIZED )
				{
					auto ew = (float) (event.window.data1 / _scaling_factor);
					auto eh = (float) (event.window.data2 / _scaling_factor);
					if( width != ew || height != eh )
					{
						_screen_dimensions = { ew, eh };
						init();
					}
				}
				break;
			case SDL_QUIT:
				_handle_input(CMD_QUIT);
				break;

			default:
				break;
		}
	}
}

void AcidwarpDisplay::_allocate_offscreen()
{
	//gDraw.abort();

	auto width = (int) _screen_dimensions.x;
	auto height = (int) _screen_dimensions.y;

	if( _output_surface )
		SDL_FreeSurface(_output_surface);

	if( _draw_surface )
		SDL_FreeSurface(_draw_surface);

	_output_surface = SDL_CreateRGBSurface(
		SDL_SWSURFACE, width, height, 8, 0, 0, 0, 0
	);

	if( !_drawing_on_surface )
	{
		if( _output_buffer )
			free(_output_buffer);

		if( _draw_buffer )
			free(_draw_buffer);

		_drawing_on_surface = true;
		_draw_surface = SDL_CreateRGBSurface(
			SDL_SWSURFACE, width, height, 8, 0, 0, 0, 0
		);
	}

	_colors = SDL_AllocPalette(256);
}

void AcidwarpDisplay::_to_surface()
{
	int row;
	unsigned char* outp, * inp = _output_buffer;

	if( SDL_MUSTLOCK(_output_surface) )
	{
		if( SDL_LockSurface(_output_surface) != 0 )
		{
			_errlog("locking surface when ending update");
			exit(-1);
		}
	}

	outp = (unsigned char*) _output_surface->pixels;

	auto width = (int) _screen_dimensions.x;
	auto height = (int) _screen_dimensions.y;

	if( _scaling_factor == 1 )
	{
		for( row = 0; row < height; row++ )
		{
			memcpy(outp, inp, width);
			outp += _output_surface->pitch;
			inp += width;
		}
	}
	else if( _scaling_factor == 2 )
	{
		unsigned char* outp2 = outp + _output_surface->pitch;
		int skip = (_output_surface->pitch - width) << 1;
		int col;
		unsigned char c;
		for( row = 0; row < height; row++ )
		{
			for( col = 0; col < width; col++ )
			{
				c = *(inp++);
				*(outp++) = c;
				*(outp++) = c;
				*(outp2++) = c;
				*(outp2++) = c;
			}
			outp += skip;
			outp2 += skip;
		}
	}

	if( SDL_MUSTLOCK(_output_surface) )
	{
		SDL_UnlockSurface(_output_surface);
	}
}

void AcidwarpDisplay::begin_update(uint8_t** p, unsigned int* pitch, unsigned int* w, unsigned int* h)
{
	if( _drawing_on_surface )
	{
		if( SDL_MUSTLOCK(_draw_surface) )
		{
			SDL_LockSurface(_draw_surface);
		}

		*p = static_cast<uint8_t*>(_draw_surface->pixels);
		*pitch = _draw_surface->pitch;
	}
	else
	{
		*p = _draw_buffer;
		*pitch = (int) _screen_dimensions.x;
	}

	*w = (int) _screen_dimensions.x;
	*h = (int) _screen_dimensions.y;
}

void AcidwarpDisplay::finish_update()
{
	if( !_drawing_on_surface )
		return;

	if( SDL_MUSTLOCK(_draw_surface) )
		SDL_LockSurface(_draw_surface);

	_draw_buffer = nullptr;
}

void AcidwarpDisplay::swap_buffers()
{
	if( !_drawing_on_surface )
	{
		uint8_t* temp = _draw_buffer;
		_draw_buffer = _output_buffer;
		_output_buffer = temp;

		_to_surface();
	}
	else
	{
		SDL_Surface* temp = _draw_surface;
		_draw_surface = _output_surface;
		_output_surface = temp;
	}

	create_texture_from_renderer(_texture, _renderer, _output_surface, _state.show_gui);
}

void AcidwarpDisplay::set_palette(unsigned char* palette)
{
	static SDL_Color colors[NUM_COLORS];
	for( auto i = 0; i < NUM_COLORS; i++ )
	{
		colors[i].r = palette[i * 3 + 0] << 2;
		colors[i].g = palette[i * 3 + 1] << 2;
		colors[i].b = palette[i * 3 + 2] << 2;
	}

	SDL_SetPaletteColors(
		_output_surface->format->palette,
		colors,
		0,
		NUM_COLORS
	);

	create_texture_from_renderer(_texture, _renderer, _output_surface, _state.show_gui);
}