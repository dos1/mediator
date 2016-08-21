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

#include "../common.h"
#include <libsuperderpy.h>
#include "pause.h"

int Gamestate_ProgressCount = 1;

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	al_draw_filled_rectangle(0, 0, 320, 180, al_map_rgba(0,0,0,192));
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5 - 25, ALLEGRO_ALIGN_CENTRE, "Game paused!");
	DrawTextWithShadow(data->font, al_map_rgb(255,255,255), game->viewport.width*0.5, game->viewport.height*0.5 + 5, ALLEGRO_ALIGN_CENTRE, "SPACE to resume");
}

void Gamestate_Start(struct Game *game, struct GamestateResources* data) {

}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && ((ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE) || (ev->keyboard.keycode == ALLEGRO_KEY_SPACE))) {
		    ResumeAllGamestates(game);
				StopCurrentGamestate(game);
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));

	data->font = al_load_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),8,0 );
	(*progress)(game);
	return data;
}

void Gamestate_Stop(struct Game *game, struct GamestateResources* data) {}

void Gamestate_Unload(struct Game *game, struct GamestateResources* data) {
	al_destroy_font(data->font);
	free(data);
}

void Gamestate_Reload(struct Game *game, struct GamestateResources* data) {}

void Gamestate_Resume(struct Game *game, struct GamestateResources* data) {}
void Gamestate_Pause(struct Game *game, struct GamestateResources* data) {}
