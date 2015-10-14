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
#include "rockets.h"

int Gamestate_ProgressCount = 5;

struct Rocket* CreateRocket(struct Game *game, struct RocketsResources* data, struct Rocket* rockets, bool right) {
    struct Rocket *n = malloc(sizeof(struct Rocket));
    n->next = NULL;
    n->prev = NULL;
    n->dx = (right ? -1.5 : 1.5) + (rand() / (float)RAND_MAX) * 0.6 - 0.3;
    n->dy = -2 + (rand() / (float)RAND_MAX) * 0.5 - 0.25;
    n->modifier = 0.025;
    n->blown = false;
    n->character = CreateCharacter(game, "rocket");
    n->character->spritesheets = data->rocket_template->spritesheets;
    n->character->shared = true;
    SelectSpritesheet(game, n->character, right ? "usa" : "ru");
    SetCharacterPosition(game, n->character, right ? 270 : 48, right ? 136 : 122, right ? -0.33 : 0.33);

    if (rockets) {
        struct Rocket *tmp = rockets;
        while (tmp->next) {
            tmp=tmp->next;
        }
        tmp->next = n;
        n->prev = tmp;
        return rockets;
    } else {
        return n;
    }
}

void DrawRockets(struct Game *game, struct RocketsResources* data, struct Rocket* rockets) {
    struct Rocket *tmp = rockets;
    while (tmp) {
        DrawCharacter(game, tmp->character, al_map_rgb(255,255,255), 0);
        tmp=tmp->next;
    }
}

void UpdateRockets(struct Game *game, struct RocketsResources *data, struct Rocket* rockets) {
    struct Rocket *tmp = rockets;
    while (tmp) {
        tmp->dy+= tmp->modifier;
        tmp->dx+= (tmp->dx > 0) ? (-tmp->modifier / 5) : (tmp->modifier / 5);
        MoveCharacter(game, tmp->character, tmp->dx, tmp->dy, tmp->blown ? 0 : ((tmp->dx > 0) ? 0.015 : -0.015));
        AnimateCharacter(game, tmp->character, 1);

        if (!tmp->blown) {
            if ((tmp->character->y > 140) && (rand() % 4 == 0)) {
                tmp->blown = true;
                tmp->modifier = 0;
                tmp->character->angle = 0;
                tmp->dx = 0;
                tmp->dy = 0;
                SelectSpritesheet(game, tmp->character, "boom");
                MoveCharacter(game, tmp->character, -25, -18, 0);
            }
        }
        tmp=tmp->next;
    }
}

void Gamestate_Logic(struct Game *game, struct RocketsResources* data) {

    if (data->counter % 40 == 0) {
        if (data->counter % 80 == 0) {
            data->rockets_left = CreateRocket(game, data, data->rockets_left, false);
        } else {
            data->rockets_right = CreateRocket(game, data, data->rockets_right, true);
        }
    }

    UpdateRockets(game, data, data->rockets_left);
    UpdateRockets(game, data, data->rockets_right);

    AnimateCharacter(game, data->usa_flag, 1);
    AnimateCharacter(game, data->ru_flag, 1);

    data->counter++;
    data->cloud_rotation += 0.002;
}

void Gamestate_Draw(struct Game *game, struct RocketsResources* data) {

    al_set_target_bitmap(data->combined);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_bitmap(data->earth, 0, 0, 0);
    //al_draw_bitmap(data->clouds, -140, -210, 0);
    //al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA, ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ZERO);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ZERO, ALLEGRO_INVERSE_ALPHA);
    al_draw_rotated_bitmap(data->clouds, 250, 250, -90 + 250, -160 + 250 + 20, data->cloud_rotation, 0);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

    al_set_target_bitmap(data->pixelator);

    al_draw_bitmap(data->bg, 0, 0, 0);
    al_draw_bitmap(data->earth2, 0, 0, 0);
    al_draw_bitmap(data->combined, 0, 0, 0);
    al_draw_bitmap(data->cursor, data->cursorx, data->cursory, 0);

    DrawRockets(game, data, data->rockets_left);
    DrawRockets(game, data, data->rockets_right);

    DrawCharacter(game, data->usa_flag, al_map_rgb(255,255,255), 0);
    DrawCharacter(game, data->ru_flag, al_map_rgb(255,255,255), 0);

    al_set_target_backbuffer(game->display);
    al_draw_bitmap(data->pixelator, 0, 0, 0);
}

void Gamestate_Start(struct Game *game, struct RocketsResources* data) {
    data->rockets_left = NULL;
    data->rockets_right = NULL;

    SetCharacterPosition(game, data->usa_flag, 266, 105, 0);
    SetCharacterPosition(game, data->ru_flag, 13, 103, 0);

    SelectSpritesheet(game, data->usa_flag, "usa");
    SelectSpritesheet(game, data->ru_flag, "ru");

    data->counter = 0;
    data->cloud_rotation = 0;

    al_get_mouse_cursor_position(&data->cursorx, &data->cursory);
}

void Gamestate_ProcessEvent(struct Game *game, struct RocketsResources* data, ALLEGRO_EVENT *ev) {
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
        SwitchGamestate(game, "rockets", "menu");
    } else if (ev->type == ALLEGRO_EVENT_MOUSE_AXES) {
        data->cursorx = ev->mouse.x / 10;
        data->cursory = ev->mouse.y / 10;
        PrintConsole(game, "mouse %d %d", data->cursorx, data->cursory);
    }
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
    struct RocketsResources *data = malloc(sizeof(struct RocketsResources));

    data->bg = al_load_bitmap( GetDataFilePath(game, "rockets/bg.png"));

    data->earth = al_load_bitmap( GetDataFilePath(game, "rockets/earth.png"));
    data->earth2 = al_load_bitmap( GetDataFilePath(game, "rockets/earth2.png"));

    data->clouds = al_load_bitmap( GetDataFilePath(game, "rockets/clouds.png"));

    data->cursor = al_create_bitmap(16, 16);
    al_set_target_bitmap(data->cursor);
    al_clear_to_color(al_map_rgb(0, 0, 255));

    data->pixelator = al_create_bitmap(320, 180);
    al_set_target_bitmap(data->pixelator);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    data->combined = al_create_bitmap(320, 180);
    al_set_target_bitmap(data->combined);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    al_set_target_backbuffer(game->display);

    data->rocket_template = CreateCharacter(game, "rocket");
    RegisterSpritesheet(game, data->rocket_template, "usa");
    RegisterSpritesheet(game, data->rocket_template, "ru");
    RegisterSpritesheet(game, data->rocket_template, "boom");
    RegisterSpritesheet(game, data->rocket_template, "blank");
    LoadSpritesheets(game, data->rocket_template);

    data->usa_flag = CreateCharacter(game, "flag");
    RegisterSpritesheet(game, data->usa_flag, "usa");
    LoadSpritesheets(game, data->usa_flag);

    data->ru_flag = CreateCharacter(game, "flag");
    RegisterSpritesheet(game, data->ru_flag, "ru");
    LoadSpritesheets(game, data->ru_flag);

    return data;
}

void Gamestate_Stop(struct Game *game, struct RocketsResources* data) {

}

void Gamestate_Unload(struct Game *game, struct RocketsResources* data) {
    al_destroy_bitmap(data->bg);
    al_destroy_bitmap(data->earth);
    al_destroy_bitmap(data->earth2);
    al_destroy_bitmap(data->clouds);
    al_destroy_bitmap(data->combined);
    al_destroy_bitmap(data->cursor);
    al_destroy_bitmap(data->pixelator);
	free(data);
}

void Gamestate_Reload(struct Game *game, struct RocketsResources* data) {}

void Gamestate_Resume(struct Game *game, struct RocketsResources* data) {}
void Gamestate_Pause(struct Game *game, struct RocketsResources* data) {}
