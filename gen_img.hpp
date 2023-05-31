#include <stdint.h>
#include <math.h>

/* ACID WARP (c)Copyright 1992, 1993 by Noah Spurrier
 * All Rights reserved. Private Proprietary Source Code by Noah Spurrier
 * Ported to Linux by Steven Wills
 * Ported to SDL by Boris Gjenero
 */

void generate_image_float(int imageFuncNum, uint8_t* buf_graf,
						  int _xcenter, int _ycenter,
						  int _width, int _height,
						  int colors, int pitch, int normalize)
{
	long x1, x2, x3, x4, y1, y2, y3, y4;
	long _x, _y;
	long _color;
	double x, y;
	double dx, dy;
	double dist, angle;
	double color;
	double xcenter, ycenter, width, height;

	if( normalize )
	{
		xcenter = (double) (_xcenter * 320) / _width;
		ycenter = (double) (_ycenter * 200) / _height;
		width = (double) 320;
		height = (double) 200;
	}
	else
	{
		xcenter = _xcenter;
		ycenter = _ycenter;
		width = _width;
		height = _height;
	}

	/* Some general purpose random angles and offsets.
	 * Not all functions use them.
	 */

	x1 = RANDOM(40) - 20;
	x2 = RANDOM(40) - 20;
	x3 = RANDOM(40) - 20;
	x4 = RANDOM(40) - 20;
	y1 = RANDOM(40) - 20;
	y2 = RANDOM(40) - 20;
	y3 = RANDOM(40) - 20;
	y4 = RANDOM(40) - 20;

	for( _y = 0;
		_y < _height
		&& !gDraw._draw_state.check_flag(DrawState::Abort)
		;
		++_y )
	{
		if( normalize )
		{
			y = (double) (_y * 200) / _height;
		}
		else
		{
			y = _y;
		}

		for( _x = 0; _x < _width; ++_x )
		{
			if( normalize )
			{
				x = (double) (_x * 320) / _width;
			}
			else
			{
				x = _x;
			}
			dx = x - xcenter;
			/* dy may be altered below, so calculate here */
			dy = y - ycenter;

			dist = lut_dist(dx, dy);
			angle = lut_angle(dx, dy);

			switch( imageFuncNum )
			{
				case 0: /* Rays plus 2D Waves */
					color = angle + lut_sin(dist * 10) / 64 +
						lut_cos(x * ANGLE_UNIT / width * 2) / 32 +
						lut_cos(y * ANGLE_UNIT / height * 2) / 32;
					break;

				case 1: /* Rays plus 2D Waves */
					color = angle + lut_sin(dist * 10) / 16 +
						lut_cos(x * ANGLE_UNIT / width * 2) / 8 +
						lut_cos(y * ANGLE_UNIT / height * 2) / 8;
					break;

				case 2:
					color = lut_sin(lut_dist(dx + x1, dy + y1) * 4) / 32 +
						lut_sin(lut_dist(dx + x2, dy + y2) * 8) / 32 +
						lut_sin(lut_dist(dx + x3, dy + y3) * 16) / 32 +
						lut_sin(lut_dist(dx + x4, dy + y4) * 32) / 32;
					break;

				case 3: /* Peacock */
					color = angle + lut_sin(lut_dist(dx + 20, dy) * 10) / 32 +
						angle + lut_sin(lut_dist(dx - 20, dy) * 10) / 32;
					break;

				case 4:
					color = lut_sin(dist) / 16;
					break;

				case 5: /* 2D Wave + Spiral */
					color = lut_cos(x * ANGLE_UNIT / width) / 8 +
						lut_cos(y * ANGLE_UNIT / height) / 8 +
						angle + lut_sin(dist) / 32;
					break;

				case 6: /* Peacock, three centers */
					color = lut_sin(lut_dist(dx, dy - 20) * 4) / 32 +
						lut_sin(lut_dist(dx + 20, dy + 20) * 4) / 32 +
						lut_sin(lut_dist(dx - 20, dy + 20) * 4) / 32;
					break;

				case 7: /* Peacock, three centers */
					color = angle +
						lut_sin(lut_dist(dx, dy - 20) * 8) / 32 +
						lut_sin(lut_dist(dx + 20, dy + 20) * 8) / 32 +
						lut_sin(lut_dist(dx - 20, dy + 20) * 8) / 32;
					break;

				case 8: /* Peacock, three centers */
					color = lut_sin(lut_dist(dx, dy - 20) * 12) / 32 +
						lut_sin(lut_dist(dx + 20, dy + 20) * 12) / 32 +
						lut_sin(lut_dist(dx - 20, dy + 20) * 12) / 32;
					break;

				case 9: /* Five Arm Star */
					color = dist + lut_sin(5 * angle) / 64;
					break;

				case 10: /* 2D Wave */
					color = lut_cos(x * ANGLE_UNIT / width * 2) / 4 +
						lut_cos(y * ANGLE_UNIT / height * 2) / 4;
					break;

				case 11: /* 2D Wave */
					color = lut_cos(x * ANGLE_UNIT / width) / 8 +
						lut_cos(y * ANGLE_UNIT / height) / 8;
					break;

				case 12: /* Simple Concentric Rings */
					color = dist;
					break;
				case 13: /* Simple Rays */
					color = angle;
					break;
				case 14: /* Toothed Spiral Sharp */
					color = angle + lut_sin(dist * 8) / 32;
					break;

				case 15: /* Rings with sine */
					color = lut_sin(dist * 4) / 32;
					break;

				case 16: /* Rings with sine with sliding inner Rings */
					color = dist + lut_sin(dist * 4) / 32;
					break;

				case 17:
					color = lut_sin(lut_cos(2 * x * ANGLE_UNIT / width)) / (20 + dist) + lut_sin(lut_cos(2 * y * ANGLE_UNIT / height)) / (20 + dist);
					break;

				case 18: /* 2D Wave */
					color = lut_cos(7 * x * ANGLE_UNIT / width) / (20 + dist) +
						lut_cos(7 * y * ANGLE_UNIT / height) / (20 + dist);
					break;

				case 19: /* 2D Wave */
					color = lut_cos(17 * x * ANGLE_UNIT / width) / (20 + dist) +
						lut_cos(17 * y * ANGLE_UNIT / height) / (20 + dist);
					break;

				case 20: /* 2D Wave Interference */
					color = lut_cos(17 * x * ANGLE_UNIT / width) / 32 +
						lut_cos(17 * y * ANGLE_UNIT / height) / 32 + dist + angle;
					break;

				case 21: /* 2D Wave Interference */
					color = lut_cos(7 * x * ANGLE_UNIT / width) / 32 +
						lut_cos(7 * y * ANGLE_UNIT / height) / 32 + dist;
					break;

				case 22: /* 2D Wave Interference */
					color = lut_cos(7 * x * ANGLE_UNIT / width) / 32 +
						lut_cos(7 * y * ANGLE_UNIT / height) / 32 +
						lut_cos(11 * x * ANGLE_UNIT / width) / 32 +
						lut_cos(11 * y * ANGLE_UNIT / height) / 32;
					break;

				case 23:
					color = lut_sin(angle * 7) / 32;
					break;

				case 24:
					color = lut_sin(lut_dist(dx + x1, dy + y1) * 2) / 12 +
						lut_sin(lut_dist(dx + x2, dy + y2) * 4) / 12 +
						lut_sin(lut_dist(dx + x3, dy + y3) * 6) / 12 +
						lut_sin(lut_dist(dx + x4, dy + y4) * 8) / 12;
					break;

				case 25:
					color = angle + lut_sin(lut_dist(dx + x1, dy + y1) * 2) / 16 +
						angle + lut_sin(lut_dist(dx + x2, dy + y2) * 4) / 16 +
						lut_sin(lut_dist(dx + x3, dy + y3) * 6) / 8 +
						lut_sin(lut_dist(dx + x4, dy + y4) * 8) / 8;
					break;

				case 26:
					color = angle + lut_sin(lut_dist(dx + x1, dy + y1) * 2) / 12 +
						angle + lut_sin(lut_dist(dx + x2, dy + y2) * 4) / 12 +
						angle + lut_sin(lut_dist(dx + x3, dy + y3) * 6) / 12 +
						angle + lut_sin(lut_dist(dx + x4, dy + y4) * 8) / 12;
					break;

				case 27:
					color = lut_sin(lut_dist(dx + x1, dy + y1) * 2) / 32 +
						lut_sin(lut_dist(dx + x2, dy + y2) * 4) / 32 +
						lut_sin(lut_dist(dx + x3, dy + y3) * 6) / 32 +
						lut_sin(lut_dist(dx + x4, dy + y4) * 8) / 32;
					break;

				case 28: /* Random Curtain of Rain (in strong wind) */
					if( y == 0 || x == 0 )
						color = RANDOM(16);
					else
						color = (*(buf_graf + (pitch * _y) + (_x - 1)) + *(buf_graf + (pitch * (_y - 1)) + _x)) / 2 + RANDOM(16) - 8;
					break;

				case 29:
					if( y == 0 || x == 0 )
						color = RANDOM(1024);
					else
						color = dist / 6 + (*(buf_graf + (pitch * _y) + (_x - 1)) + *(buf_graf + (pitch * (_y - 1)) + _x)) / 2 + RANDOM(16) - 8;
					break;

				case 30:
					color = xor (xor (lut_sin(lut_dist(dx, dy - 20) * 4) / 32,
								 lut_sin(lut_dist(dx + 20, dy + 20) * 4) / 32),
								 lut_sin(lut_dist(dx - 20, dy + 20) * 4) / 32);
					break;

				case 31:
					color = xor (mod(angle, (ANGLE_UNIT / 4)), dist);
					break;

				case 32:
					color = xor (dy, dx);
					break;

				case 33: /* Variation on Rain */
					if( y == 0 || x == 0 )
						color = RANDOM(16);
					else
						color = (*(buf_graf + (pitch * _y) + (_x - 1)) + *(buf_graf + (pitch * (_y - 1)) + _x)) / 2;

					color += RANDOM(2) - 1;

					if( color < 64 )
						color += RANDOM(16) - 8;
					break;

				case 34: /* Variation on Rain */
					if( y == 0 || x == 0 )
						color = RANDOM(16);
					else
						color = (*(buf_graf + (pitch * _y) + (_x - 1)) + *(buf_graf + (pitch * (_y - 1)) + _x)) / 2;

					if( color < 100 )
						color += RANDOM(16) - 8;
					break;

				case 35:
					color = angle + lut_sin(dist * 8) / 32;
					dx = x - xcenter;
					dy = (y - ycenter) * 2;
					dist = lut_dist(dx, dy);
					angle = lut_angle(dx, dy);
					color = (color + angle + lut_sin(dist * 8) / 32) / 2;
					break;

				case 36:
					color = angle + lut_sin(dist * 10) / 16 +
						lut_cos(x * ANGLE_UNIT / width * 2) / 8 +
						lut_cos(y * ANGLE_UNIT / height * 2) / 8;
					dx = x - xcenter;
					dy = (y - ycenter) * 2;
					dist = lut_dist(dx, dy);
					angle = lut_angle(dx, dy);
					color = (color + angle + lut_sin(dist * 8) / 32) / 2;
					break;

				case 37:
					color = angle + lut_sin(dist * 10) / 16 +
						lut_cos(x * ANGLE_UNIT / width * 2) / 8 +
						lut_cos(y * ANGLE_UNIT / height * 2) / 8;
					dx = x - xcenter;
					dy = (y - ycenter) * 2;
					dist = lut_dist(dx, dy);
					angle = lut_angle(dx, dy);
					color = (color + angle + lut_sin(dist * 10) / 16 +
							 lut_cos(x * ANGLE_UNIT / width * 2) / 8 +
							 lut_cos(y * ANGLE_UNIT / height * 2) / 8) /
						2;
					break;

				case 38:
					/* Intent is to interlace two different screens */
					if( _y % 2 )
					{
						dy *= 2;
						dist = lut_dist(dx, dy);
						angle = lut_angle(dx, dy);
					}
					color = angle + lut_sin(dist * 8) / 32;
					break;

				case 39:
					color = xor (mod(angle, (ANGLE_UNIT / 4)), dist);
					dx = x - xcenter;
					dy = (y - ycenter) * 2;
					dist = lut_dist(dx, dy);
					angle = lut_angle(dx, dy);
					color = (color + (xor (mod(angle, (ANGLE_UNIT / 4)), dist))) / 2;
					break;

				case 40:
					color = xor (dy, dx);
					dx = x - xcenter;
					dy = (y - ycenter) * 2;
					color = (color + xor (dy, dx)) / 2;
					break;
				default:
					color = RANDOM(255); // wig the f out
					break;
			}

			_color = (long) color % (colors - 1);
			if( color < 0 )
				_color += (colors - 2);

			++_color;

			*(buf_graf + (pitch * _y) + _x) = (uint8_t) _color;
		}
		/* end for (y = 0; y < height; ++y)        */
	}
	/* end for (x = 0; x < width; ++x)        */
}
