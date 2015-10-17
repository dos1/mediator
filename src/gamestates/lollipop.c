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
#include "lollipop.h"

int Gamestate_ProgressCount = 5;


bool switchMinigame(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
        if (state == TM_ACTIONSTATE_START) {
            StopGamestate(game, "lollipop");
            StartGamestate(game, "riots");
        }
        return true;
}

bool theEnd(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
        if (state == TM_ACTIONSTATE_START) {
            StopGamestate(game, "lollipop");
            StartGamestate(game, "theend");
        }
        return true;
}

void Gamestate_Logic(struct Game *game, struct RocketsResources* data) {

    if ((data->spawncounter == data->currentspawn) && ((data->counter < data->timelimit) || (data->lost))) {
        data->dx =(( (rand() / (float)RAND_MAX) - 0.5)/ 500.0) * game->mediator.modificator;
        PrintConsole(game, "DX %f", data->dx);
        data->spawncounter = 0;
    }

    if (data->lost) {
        data->zadyma++;
        if (data->zadyma >= 255) {
            data->zadyma = 255;
        }
    }

    if ((data->lost) && (data->hearts > 80)) {
        AnimateCharacter(game, game->mediator.heart, 1);
        if (game->mediator.heart->pos == 6) {
            al_play_sample_instance(data->jump_sound);
        }
    }

    if (data->lost) {
        data->hearts++;
    }

    if (data->won) {
        AnimateCharacter(game, data->euro, 1);
    }

    if ((data->counter >= data->timelimit) && (!data->lost) && (!data->won)) {
        bool stillthere = false;
        struct Rocket *tmp = data->rockets_left;
        while (tmp) {
            if (!tmp->blown) {
                stillthere = true;
                break;
            }
            tmp = tmp->next;
        }
        tmp = data->rockets_right;
        while (tmp) {
            if (!tmp->blown) {
                stillthere = true;
                break;
            }
            tmp = tmp->next;
        }
        if (!stillthere) {
            SelectSpritesheet(game, data->euro, "euro");
            SetCharacterPosition(game, data->euro, 0, 0, 0);
            al_play_sample_instance(data->rainbow_sound);
            data->won = true;
            AdvanceLevel(game, true);
            SelectSpritesheet(game, data->riot, "win");

            TM_AddDelay(data->timeline, 2500);
            TM_AddAction(data->timeline, switchMinigame, NULL, "switchMinigame");
        }
    }

    if (data->won) {
        AnimateCharacter(game, data->riot, 1);
    } else {

    if ((data->currentpos < -0.15) || (data->currentpos > 0.15)) {
        if (!data->lost) {
            AdvanceLevel(game, false);
            data->lost = true;
            SelectSpritesheet(game, data->riot, "end");
            TM_AddDelay(data->timeline, 3500);
            if (game->mediator.lives > 0) {
                TM_AddAction(data->timeline, switchMinigame, NULL, "switchMinigame");
            } else {

                TM_AddAction(data->timeline, theEnd, NULL, "switchMinigame");
            }
            al_play_sample_instance(data->boom_sound);
        }

    }

    }

    data->counter++;
    data->spawncounter++;
    data->cloud_rotation += 0.002;

    if (!data->lost) {
        data->currentpos += data->dx;
    }

    TM_Process(data->timeline);
}

void Gamestate_Draw(struct Game *game, struct RocketsResources* data) {

    al_set_target_bitmap(data->pixelator);
    al_clear_to_color(al_map_rgba(128,192,255,0));
    al_draw_bitmap(data->bg, 0, 0, 0);


    if (data->won) {
        //DrawCharacter(game, data->euro, al_map_rgb(255,255,255), 0);
    }

    if ((!data->lost) && (!data->won)) {
        al_draw_rotated_bitmap(data->earth2, 158, 140, 158, 140, data->currentpos, 0);
        al_draw_bitmap(data->earth, 0, 0, 0);

        //DrawCharacter(game, data->cursor, al_map_rgb(255,255,255), 0);
    }

PrintConsole(game, "%f", data->currentpos);

    if ((!data->lost) && (!data->won)) {
        al_draw_filled_rectangle(78, 5, 78+164, 5+5, al_map_rgb(155, 142, 142));
        al_draw_filled_rectangle(80, 6, 80+160, 6+3, al_map_rgb(66, 55, 30));
        al_draw_filled_rectangle(80, 6, (data->counter < data->timelimit) ? (80+ 160 * (1 - (data->counter / (float)data->timelimit))) : 80, 6+3, al_map_rgb(225,182, 80));
    }

    al_set_target_backbuffer(game->display);
    al_draw_bitmap(data->pixelator, 0, 0, 0);

    if ((!data->lost) && (!data->won)) {
        al_draw_filled_rectangle(78, 5, 78+164, 5+5, al_map_rgb(155, 142, 142));
        al_draw_filled_rectangle(80, 6, 80+160, 6+3, al_map_rgb(66, 55, 30));
        al_draw_filled_rectangle(80, 6, (data->counter < data->timelimit) ? (80+160 * (1 - (data->counter / (float)data->timelimit))) : 80, 6+3, al_map_rgb(225,182, 80));
    }

    if (data->won) {
        DrawCharacter(game, data->riot, al_map_rgb(255,255,255), 0);
    }

    if (data->lost) {
        al_draw_bitmap(data->currentpos < 0 ? data->clouds : data->combined, 0, 0, 0);
        if (data->hearts > 80) {
            ShowLevelStatistics(game);
        }
    }

    //Gamestate_Logic(game, data);

}

void Gamestate_Start(struct Game *game, struct RocketsResources* data) {
    data->rockets_left = NULL;
    data->rockets_right = NULL;

    data->timelimit = 400 * game->mediator.modificator;
    data->spawnspeed = 80 / game->mediator.modificator;
    data->currentspawn = data->spawnspeed;
    data->spawncounter = data->spawnspeed - 20;

    data->lost = false;
    data->won = false;

    data->hearts = 0;

    data->flash = 0;
    data->zadyma = 16;

    data->currentpos = 0;
    data->dx = 0;

    SetCharacterPosition(game, data->usa_flag, 185, 80, 0);
    SetCharacterPosition(game, data->ru_flag, 25, 80, 0);

    SetCharacterPosition(game, data->cursor, -100, -100, 0);

    SetCharacterPosition(game, data->riot, 0, 0, 0);
    SelectSpritesheet(game, data->riot, "win");

    SelectSpritesheet(game, data->cursor, "hand");

    data->counter = 0;
    data->cloud_rotation = 0;

    data->mousemove.bottom = false;
    data->mousemove.top = false;
    data->mousemove.left = false;
    data->mousemove.right = false;

    al_grab_mouse(game->display);
}

void Gamestate_ProcessEvent(struct Game *game, struct RocketsResources* data, ALLEGRO_EVENT *ev) {
    TM_HandleEvent(data->timeline, ev);
    if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
        SwitchGamestate(game, "lollipop", "menu");
    } else if (ev->type == ALLEGRO_EVENT_MOUSE_AXES) {
        int mousex = ev->mouse.dx / (al_get_display_width(game->display) / 320);

        data->currentpos += mousex / 3000.0;
        al_set_mouse_xy(game->display, 240, 120);
    }
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
    struct RocketsResources *data = malloc(sizeof(struct RocketsResources));

    data->timeline = TM_Init(game, "lollipop");

    data->bg = al_load_bitmap( GetDataFilePath(game, "lollipop/bg.png"));

    data->earth = al_load_bitmap( GetDataFilePath(game, "lollipop/peoples.png"));
    data->earth2 = al_load_bitmap( GetDataFilePath(game, "lollipop/lollipop.png"));

    data->clouds = al_load_bitmap( GetDataFilePath(game, "lollipop/przegrywdziew.png"));
    data->combined = al_load_bitmap( GetDataFilePath(game, "lollipop/przegrywchop.png"));

    data->rocket_sample = al_load_sample( GetDataFilePath(game, "riots/rocket.wav") );
    data->boom_sample = al_load_sample( GetDataFilePath(game, "lollipop/lost.wav") );
    data->jump_sample = al_load_sample( GetDataFilePath(game, "riots/boom.wav") );
    data->rainbow_sample = al_load_sample( GetDataFilePath(game, "rockets/rainbow.wav") );
    data->wuwu_sample = al_load_sample( GetDataFilePath(game, "riots/vuvu.wav") );
    data->riot_sample = al_load_sample( GetDataFilePath(game, "riots/riot.wav") );

    data->rocket_sound = al_create_sample_instance(data->rocket_sample);
    al_attach_sample_instance_to_mixer(data->rocket_sound, game->audio.fx);
    al_set_sample_instance_playmode(data->rocket_sound, ALLEGRO_PLAYMODE_ONCE);

    data->boom_sound = al_create_sample_instance(data->boom_sample);
    al_attach_sample_instance_to_mixer(data->boom_sound, game->audio.fx);
    al_set_sample_instance_playmode(data->boom_sound, ALLEGRO_PLAYMODE_ONCE);

    data->rainbow_sound = al_create_sample_instance(data->rainbow_sample);
    al_attach_sample_instance_to_mixer(data->rainbow_sound, game->audio.fx);
    al_set_sample_instance_playmode(data->rainbow_sound, ALLEGRO_PLAYMODE_ONCE);

    data->jump_sound = al_create_sample_instance(data->jump_sample);
    al_attach_sample_instance_to_mixer(data->jump_sound, game->audio.fx);
    al_set_sample_instance_playmode(data->jump_sound, ALLEGRO_PLAYMODE_ONCE);

    data->riot_sound = al_create_sample_instance(data->riot_sample);
    al_attach_sample_instance_to_mixer(data->riot_sound, game->audio.fx);
    al_set_sample_instance_playmode(data->riot_sound, ALLEGRO_PLAYMODE_ONCE);

    data->wuwu_sound = al_create_sample_instance(data->wuwu_sample);
    al_attach_sample_instance_to_mixer(data->wuwu_sound, game->audio.fx);
    al_set_sample_instance_playmode(data->wuwu_sound, ALLEGRO_PLAYMODE_ONCE);


    data->cursor = CreateCharacter(game, "cursor");
    RegisterSpritesheet(game, data->cursor, "hand");
    LoadSpritesheets(game, data->cursor);

    data->pixelator = al_create_bitmap(320, 180);
    al_set_target_bitmap(data->pixelator);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_set_target_backbuffer(game->display);

    data->rocket_template = CreateCharacter(game, "rocket");
    RegisterSpritesheet(game, data->rocket_template, "rock");
    RegisterSpritesheet(game, data->rocket_template, "bottle");
    RegisterSpritesheet(game, data->rocket_template, "bottle2");
    RegisterSpritesheet(game, data->rocket_template, "atom");
    RegisterSpritesheet(game, data->rocket_template, "boom");
    RegisterSpritesheet(game, data->rocket_template, "blank");
    LoadSpritesheets(game, data->rocket_template);

    data->usa_flag = CreateCharacter(game, "kibols");
    RegisterSpritesheet(game, data->usa_flag, "legia");
    RegisterSpritesheet(game, data->usa_flag, "poland");
    LoadSpritesheets(game, data->usa_flag);

    data->ru_flag = CreateCharacter(game, "kibols");
    RegisterSpritesheet(game, data->ru_flag, "lech");
    RegisterSpritesheet(game, data->ru_flag, "poland");
    LoadSpritesheets(game, data->ru_flag);

    data->rainbow = CreateCharacter(game, "rainbow");
    RegisterSpritesheet(game, data->rainbow, "shine");
    RegisterSpritesheet(game, data->rainbow, "be");
    LoadSpritesheets(game, data->rainbow);

    data->riot = CreateCharacter(game, "loliwin");
    RegisterSpritesheet(game, data->riot, "win");
    RegisterSpritesheet(game, data->riot, "end");
    LoadSpritesheets(game, data->riot);

    data->euro = CreateCharacter(game, "euro");
    RegisterSpritesheet(game, data->euro, "euro");
    LoadSpritesheets(game, data->euro);

    return data;
}

void Gamestate_Stop(struct Game *game, struct RocketsResources* data) {
    TM_CleanQueue(data->timeline);
    al_ungrab_mouse();
}

void Gamestate_Unload(struct Game *game, struct RocketsResources* data) {
    al_destroy_bitmap(data->bg);
    al_destroy_bitmap(data->earth);
    al_destroy_bitmap(data->earth2);
    al_destroy_bitmap(data->clouds);
    al_destroy_bitmap(data->combined);
    al_destroy_bitmap(data->pixelator);
    al_destroy_sample_instance(data->rocket_sound);
    al_destroy_sample_instance(data->boom_sound);
    al_destroy_sample(data->rocket_sample);
    al_destroy_sample(data->boom_sample);
    // TODO: DestroyCharacters
    free(data);
}

void Gamestate_Reload(struct Game *game, struct RocketsResources* data) {}

void Gamestate_Resume(struct Game *game, struct RocketsResources* data) {}
void Gamestate_Pause(struct Game *game, struct RocketsResources* data) {}
