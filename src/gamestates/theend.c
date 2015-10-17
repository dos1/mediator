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
#include "theend.h"

int Gamestate_ProgressCount = 1;

void Gamestate_Logic(struct Game *game, struct dosowiskoResources* data) {

}

void Gamestate_Draw(struct Game *game, struct dosowiskoResources* data) {
    al_draw_bitmap(data->bitmap, 0, 0, 0);
    char text[255];
    snprintf(text, 255, "%d", game->mediator.score);
    DrawTextWithShadow(game->_priv.font, al_map_rgb(255,255,255), 320/2, 20, ALLEGRO_ALIGN_CENTER, "Score:");
    DrawTextWithShadow(data->font, al_map_rgb(255,255,255), 320/2, 30, ALLEGRO_ALIGN_CENTER, text);
    snprintf(text, 255, "High score: %d", data->score);
    DrawTextWithShadow(game->_priv.font, al_map_rgb(255,255,255), 320/2, 140, ALLEGRO_ALIGN_CENTER, text);
    DrawTextWithShadow(game->_priv.font, al_map_rgb(255,255,255), 320/2, 162, ALLEGRO_ALIGN_CENTER, "Press RETURN");
}

void Gamestate_Start(struct Game *game, struct dosowiskoResources* data) {
    al_set_sample_instance_gain(game->muzyczka.instance.drums, 0.0);
    al_set_sample_instance_gain(game->muzyczka.instance.fg, 0.0);
    al_set_sample_instance_gain(game->muzyczka.instance.bg, 1.5);

    char *end = "";
    data->score = strtol(GetConfigOptionDefault(game, "Mediator", "score", "0"), &end, 10);

    if (game->mediator.score > data->score) {
        char text[255];
        snprintf(text, 255, "%d", game->mediator.score);
        SetConfigOption(game, "Mediator", "score", text);
    }
}

void Gamestate_ProcessEvent(struct Game *game, struct dosowiskoResources* data, ALLEGRO_EVENT *ev) {
    //TM_HandleEvent(data->timeline, ev);
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
        SwitchGamestate(game, "theend", "menu");
	}
    if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ENTER)) {
        SwitchGamestate(game, "theend", "menu");
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
