/*! \file common.c
 *  \brief Common stuff that can be used by all gamestates.
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

#include "common.h"
#include <allegro5/allegro_primitives.h>
#include <libsuperderpy.h>

struct CommonResources* CreateGameData(struct Game *game) {
	struct CommonResources *data = calloc(1, sizeof(struct CommonResources));
	data->font = al_load_font(GetDataFilePath(game, "fonts/MonkeyIsland.ttf"), 8, 0);

	if (al_get_display_height(game->display)*0.025 >= 16) {
		data->font_bsod = al_load_font(GetDataFilePath(game, "fonts/PerfectDOSVGA437.ttf"),16 * ((al_get_display_height(game->display) > 1080) ? 2 : 1) ,0 );
	} else {
		data->font_bsod = al_load_font(GetDataFilePath(game, "fonts/DejaVuSansMono.ttf"), al_get_display_height(game->display)*0.025,0 );
	}

	data->mediator.lives = 3;
	data->mediator.score = 0;
	data->mediator.modificator = 1;
	data->mediator.strike = 0;
	data->mediator.next = "lollipop";
	data->mediator.pause = false;

	data->mediator.heart = CreateCharacter(game, "heart");
	RegisterSpritesheet(game, data->mediator.heart, "heart");
	RegisterSpritesheet(game, data->mediator.heart, "blank");
	LoadSpritesheets(game, data->mediator.heart);
	SelectSpritesheet(game, data->mediator.heart, "heart");

	return data;
}

void DestroyGameData(struct Game *game, struct CommonResources *resources) {
	free(resources);
}

bool GetAbstractIsItBonusLevelTimeNowFactoryProvider(struct Game *game) {
	return game->data->mediator.strike && (game->data->mediator.strike % 5 == 0);
}

void AdvanceLevel(struct Game *game, bool won) {
	  if (won) {
			  game->data->mediator.score++;
				game->data->mediator.strike++;
		} else {
			  game->data->mediator.lives--;
				game->data->mediator.strike = 0;
		}
		game->data->mediator.modificator *= 1.025;
		SelectSpritesheet(game, game->data->mediator.heart, "heart");
}

void ShowLevelStatistics(struct Game *game) {
	  // show as many bitmaps as there are lives
	  // show additional one as a animated character

	  al_draw_filled_rectangle(0, 0, 320, 240, al_map_rgba(0, 0, 0, 192));

		int x = 75;

		int pos = game->data->mediator.heart->pos;
		struct Spritesheet *a = game->data->mediator.heart->spritesheet;

		for (int i = 0; i < game->data->mediator.lives; i++) {
			  SetCharacterPosition(game, game->data->mediator.heart, x, 50, 0);
				SelectSpritesheet(game, game->data->mediator.heart, "heart");
				DrawCharacter(game, game->data->mediator.heart, al_map_rgb(255, 255, 255), 0);
				x += 48;
		}
		game->data->mediator.heart->pos = pos;
		game->data->mediator.heart->spritesheet = a;

		if (game->data->mediator.lives >= 0) {
			  SetCharacterPosition(game, game->data->mediator.heart, x, 50, 0);
				DrawCharacter(game, game->data->mediator.heart, al_map_rgb(255, 255, 255), 0);
		}

		//DrawTextWithShadow(game->_priv.font, al_map_rgb(255,255,255), 50, 50, 0, text);
}
