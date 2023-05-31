#pragma once

#include <stdint.h>

#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer.h"

#include "state.hpp"

struct AcidwarpDrawingContext;

#include "draw.hpp"

enum acidwarp_command
{
	CMD_PAUSE = 1,
	CMD_SKIP,
	CMD_QUIT,
	CMD_NEWPAL,
	CMD_LOCK,
	CMD_PAL_FASTER,
	CMD_PAL_SLOWER,
	CMD_RESIZE,
	CMD_GUI
};

struct AcidwarpDisplay
{
public:
	static AcidwarpDisplay& instance()
	{
		static AcidwarpDisplay _instance(true);
		return _instance;
	}
private:
	AcidwarpDisplay(bool fullscreen);
	~AcidwarpDisplay();
public:
	void set_palette(unsigned char* palette);
	void begin_update(uint8_t** p, unsigned int* pitch, unsigned int* w, unsigned int* h);
	void finish_update();
	void swap_buffers();
	void process_input();
	void init();
	
	DisplayState _state;
	static bool _fullscreen;
	SDL_Renderer* _renderer = nullptr;
	SDL_Window* _window = nullptr;
	static time_t l_time, m_time;

private:
	void _process_key(int key, SDL_Keymod mod);
	void _allocate_offscreen();
	void _to_surface();

	void gui();
	void create_texture_from_renderer(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Surface* surface, bool _gui);
	void _handle_input(enum acidwarp_command cmd);

	SDL_Texture* _texture = nullptr;
	SDL_Surface* _draw_surface = nullptr, * _output_surface = nullptr;
	SDL_Palette* _colors = nullptr;

	uint8_t* _draw_buffer = nullptr, * _output_buffer = nullptr;

	bool _drawing_on_surface = false;
	int _scaling_factor = 1;
	ImVec2 _screen_dimensions;

	AcidwarpDrawingContext* _drawing_context;
};