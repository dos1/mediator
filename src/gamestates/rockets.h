/*! \file dosowisko.h
 *  \brief Init animation with dosowisko.net logo.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

struct RocketsResources {
		ALLEGRO_FONT *font;
        ALLEGRO_BITMAP *bg, *earth, *earth2, *cursor, *pixelator, *combined, *clouds;
        struct Character *rocket_template, *usa_flag, *ru_flag;

        struct Rocket {
            struct Character *character;
            float dx, dy, modifier;
            bool blown;
            struct Rocket *next, *prev;
        } *rockets_left, *rockets_right;

        int counter;
        float cloud_rotation;

        int cursorx, cursory;

		struct Timeline *timeline;
};
