/*! \file dosowisko.c
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

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "../utils.h"
#include "../timeline.h"
#include "riots.h"

int Gamestate_ProgressCount = 5;

void Gamestate_Logic(struct Game *game, struct rocketsResources* data) {

}

void Gamestate_Draw(struct Game *game, struct rocketsResources* data) {

}

void Gamestate_Start(struct Game *game, struct rocketsResources* data) {
}

void Gamestate_ProcessEvent(struct Game *game, struct rocketsResources* data, ALLEGRO_EVENT *ev) {
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
        SwitchGamestate(game, "rockets", "menu");
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
    struct rocketsResources *data = malloc(sizeof(struct rocketsResources));

	return data;
}

void Gamestate_Stop(struct Game *game, struct rocketsResources* data) {

}

void Gamestate_Unload(struct Game *game, struct rocketsResources* data) {
	free(data);
}

void Gamestate_Reload(struct Game *game, struct rocketsResources* data) {}

void Gamestate_Resume(struct Game *game, struct rocketsResources* data) {}
void Gamestate_Pause(struct Game *game, struct rocketsResources* data) {}
