#pragma once

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <random>

#include "timer.hpp"

#define NUM_COLORS 256

#define M_PI 3.14159265358979323846

#define gDisplay AcidwarpDisplay::instance()
#define gDraw AcidwarpDrawingContext::instance()
#define gTimer TimerState::instance()
#define gPalette Palette::instance()

#define loop for (;;)
#define loopend(x) \
    {              \
        if (x)     \
            break; \
    }

#define until(a) while (!(a))

#define RANDOMIZE() (srand((uint8_t)time((time_t *)NULL)))
#define RANDOM(a) (rand() / (RAND_MAX / (a) + 1))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ABS(a) (((a) < 0) ? -(a) : (a))
#define FABS(a) (((a) < F 0) ? -(a) : (a))
#define DABS(a) (((a) < 0.0) ? -(a) : (a))

#define SIGN(a) (((a) < 0) ? -1 : 1)
#define FSIGN(a) (((a) < F 0) ? F(-1) : F 1)
#define DSIGN(a) (((a) < 0.0) ? -1.0 : 1.0)

#define DONE 1
#define NOT_DONE 0

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define _logf(m, ...) fprintf(stdout, "[ %s:%s:%d ] " m " %s\n", __FILENAME__, __func__, __LINE__, __VA_ARGS__, "");
#define _errlogf(m, ...) fprintf(stdout, "!!! %s:%s:%d !!! " m " %s\n", __FILENAME__, __func__, __LINE__, __VA_ARGS__, "")
#define _errlog(m) _errlogf("%s", m)
#define _log(m) _logf("%s", m)

#include <chrono>

inline std::vector<int> make_shuffled_list(int size, bool shuffle = false)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine e(seed);
	std::vector<int> vec_2(size);
	for( auto i = 0; i < size; i++ )
		vec_2.at(i) = i;

	if (shuffle )
		std::shuffle(vec_2.begin(), vec_2.end(), e);

	return vec_2;
}

#define mod(x, y) fmod((x), (y))
#define xor(x, y) ((int)(x) ^ (int)(y))

/* ANGLE_UNIT corresponds to M_PI * 2, the full circle in radians.
 * To avoid visible seams, resulting colours need to go from 0 to 254,
 * with each getting an equal slice of the circle.
 */
#define ANGLE_UNIT                  (255.0)
#define ANGLE_UNIT_2                (ANGLE_UNIT*2)
#define ANGLE_UNIT_HALF             (ANGLE_UNIT/2)
#define ANGLE_UNIT_EIGHTH           (ANGLE_UNIT/8)
#define ANGLE_UNIT_QUART            (ANGLE_UNIT/4)
#define ANGLE_UNIT_THREE_QUARTERS   (ANGLE_UNIT*3/4)

 /* TRIG_UNIT is full scale of trig functions. It was changed to
  * try to avoid visible seams where the palette wraps.
  */
#define TRIG_UNIT                   (ANGLE_UNIT*2)

inline double lut_sin(double a)
{
	return TRIG_UNIT * sin(a * M_PI * 2 / ANGLE_UNIT);
}

inline double lut_cos(double a)
{
	return TRIG_UNIT * cos(a * M_PI * 2 / ANGLE_UNIT);
}

inline double lut_angle(double dx, double dy)
{
	double angle = atan2(dy, dx) * ANGLE_UNIT / (M_PI * 2);
	/* Always return a positive result */
	if( angle < 0.0 ) angle += (double) ANGLE_UNIT_2;
	return angle;
}

inline double lut_dist(double x, double y)
{
	return sqrt(x * x + y * y);
}