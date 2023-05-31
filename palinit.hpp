#pragma once

#include <stdint.h>

inline void init_test_palette(uint8_t* palArray)
{
	for( int palRegNum = 0; palRegNum < 128; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) palRegNum / 2;
		palArray[palRegNum * 3 + 1] = (uint8_t) palRegNum / 2;
		palArray[palRegNum * 3 + 2] = (uint8_t) palRegNum / 2;
	}

	for( int palRegNum = 128; palRegNum < 256; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) (255 - palRegNum) / 2;
		palArray[palRegNum * 3 + 1] = (uint8_t) (255 - palRegNum) / 2;
		palArray[palRegNum * 3 + 2] = (uint8_t) (255 - palRegNum) / 2;
	}
}

inline void add_sparkles_to_palette(uint8_t* palArray, int sparkle_amount)
{
	int palRegNum;

	for( palRegNum = 1; palRegNum < 256; palRegNum += 4 )
	{
		palArray[palRegNum * 3] = (uint8_t) MIN(63, palArray[palRegNum * 3] + sparkle_amount);
		palArray[palRegNum * 3 + 1] = (uint8_t) MIN(63, palArray[palRegNum * 3 + 1] + sparkle_amount);
		palArray[palRegNum * 3 + 2] = (uint8_t) MIN(63, palArray[palRegNum * 3 + 2] + sparkle_amount);
	}
}

inline void init_rgbw_palette(uint8_t* palArray)
{
	int palRegNum;

	for( palRegNum = 0; palRegNum < 32; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) palRegNum * 2;
		palArray[(palRegNum + 64) * 3] = (uint8_t) 0;
		palArray[(palRegNum + 128) * 3] = (uint8_t) 0;
		palArray[(palRegNum + 192) * 3] = (uint8_t) palRegNum * 2;

		palArray[palRegNum * 3 + 1] = (uint8_t) 0;
		palArray[(palRegNum + 64) * 3 + 1] = (uint8_t) palRegNum * 2;
		palArray[(palRegNum + 128) * 3 + 1] = (uint8_t) 0;
		palArray[(palRegNum + 192) * 3 + 1] = (uint8_t) palRegNum * 2;

		palArray[palRegNum * 3 + 2] = (uint8_t) 0;
		palArray[(palRegNum + 64) * 3 + 2] = (uint8_t) 0;
		palArray[(palRegNum + 128) * 3 + 2] = (uint8_t) palRegNum * 2;
		palArray[(palRegNum + 192) * 3 + 2] = (uint8_t) palRegNum * 2;
	}

	for( palRegNum = 32; palRegNum < 64; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) (63 - palRegNum) * 2;
		palArray[(palRegNum + 64) * 3] = (uint8_t) 0;
		palArray[(palRegNum + 128) * 3] = (uint8_t) 0;
		palArray[(palRegNum + 192) * 3] = (uint8_t) (63 - palRegNum) * 2;

		palArray[palRegNum * 3 + 1] = (uint8_t) 0;
		palArray[(palRegNum + 64) * 3 + 1] = (uint8_t) (63 - palRegNum) * 2;
		palArray[(palRegNum + 128) * 3 + 1] = (uint8_t) 0;
		palArray[(palRegNum + 192) * 3 + 1] = (uint8_t) (63 - palRegNum) * 2;

		palArray[palRegNum * 3 + 2] = (uint8_t) 0;
		palArray[(palRegNum + 64) * 3 + 2] = (uint8_t) 0;
		palArray[(palRegNum + 128) * 3 + 2] = (uint8_t) (63 - palRegNum) * 2;
		palArray[(palRegNum + 192) * 3 + 2] = (uint8_t) (63 - palRegNum) * 2;
	}
}

inline void init_w_palette(uint8_t* palArray)
{
	int palRegNum;

	for( palRegNum = 0; palRegNum < 128; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) palRegNum / 2;
		palArray[palRegNum * 3 + 1] = (uint8_t) palRegNum / 2;
		palArray[palRegNum * 3 + 2] = (uint8_t) palRegNum / 2;
	}

	for( palRegNum = 128; palRegNum < 256; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) (255 - palRegNum) / 2;
		palArray[palRegNum * 3 + 1] = (uint8_t) (255 - palRegNum) / 2;
		palArray[palRegNum * 3 + 2] = (uint8_t) (255 - palRegNum) / 2;
	}
}

inline void init_w_half_palette(uint8_t* palArray)
{
	int palRegNum;

	for( palRegNum = 0; palRegNum < 64; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) palRegNum;
		palArray[palRegNum * 3 + 1] = (uint8_t) palRegNum;
		palArray[palRegNum * 3 + 2] = (uint8_t) palRegNum;

		palArray[(palRegNum + 64) * 3] = (uint8_t) (63 - palRegNum);
		palArray[(palRegNum + 64) * 3 + 1] = (uint8_t) (63 - palRegNum);
		palArray[(palRegNum + 64) * 3 + 2] = (uint8_t) (63 - palRegNum);
	}

	for( palRegNum = 128; palRegNum < 256; ++palRegNum )
	{
		palArray[palRegNum * 3] = 0;
		palArray[palRegNum * 3 + 1] = 0;
		palArray[palRegNum * 3 + 2] = 0;
	}
}

inline void init_pastel_palette(uint8_t* palArray)
{
	int palRegNum;

	for( palRegNum = 0; palRegNum < 128; ++palRegNum )
	{
		palArray[palRegNum * 3] = (uint8_t) 31 + palRegNum / 4;
		palArray[palRegNum * 3 + 1] = (uint8_t) 31 + palRegNum / 4;
		palArray[palRegNum * 3 + 2] = (uint8_t) 31 + palRegNum / 4;

		palArray[(palRegNum + 128) * 3] = (uint8_t) 31 + (127 - palRegNum) / 4;
		palArray[(palRegNum + 128) * 3 + 1] = (uint8_t) 31 + (127 - palRegNum) / 4;
		palArray[(palRegNum + 128) * 3 + 2] = (uint8_t) 31 + (127 - palRegNum) / 4;
	}
}
