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
#include <stdio.h>

#include "../utils.h"
#include "../timeline.h"
#include "../config.h"
#include "info.h"

int Gamestate_ProgressCount = 1;

void Gamestate_Logic(struct Game *game, struct dosowiskoResources* data) {
    data->tick++;
    if (data->tick > 110) {
        SwitchGamestate(game, "info", "lollipop");
    }
}

void Gamestate_Draw(struct Game *game, struct dosowiskoResources* data) {
    al_draw_bitmap(data->bitmap, 0, 0, 0);
    if ((data->tick / 20) % 2 == 0) {
        DrawTextWithShadow(game->_priv.font, al_map_rgb(255,255,255), 320/2, 180/2, ALLEGRO_ALIGN_CENTER, "Use mouse");
    }
}

void Gamestate_Start(struct Game *game, struct dosowiskoResources* data) {
    data->tick = 0;
    al_grab_mouse(game->display);
    al_hide_mouse_cursor(game->display);
}

void Gamestate_ProcessEvent(struct Game *game, struct dosowiskoResources* data, ALLEGRO_EVENT *ev) {
    //TM_HandleEvent(data->timeline, ev);
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
        SwitchGamestate(game, "info", "lollipop");
	}
    if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ENTER)) {
        SwitchGamestate(game, "info", "lollipop");
    }
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	struct dosowiskoResources *data = malloc(sizeof(struct dosowiskoResources));
    data->bitmap = al_load_bitmap( GetDataFilePath(game, "bg.png"));

    data->font = al_load_ttf_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"),100,0 );
	(*progress)(game);

	return data;
}

void Gamestate_Stop(struct Game *game, struct dosowiskoResources* data) {

}

void Gamestate_Unload(struct Game *game, struct dosowiskoResources* data) {
    free(data);
}

void Gamestate_Reload(struct Game *game, struct dosowiskoResources* data) {}

void Gamestate_Resume(struct Game *game, struct dosowiskoResources* data) {}
void Gamestate_Pause(struct Game *game, struct dosowiskoResources* data) {}
