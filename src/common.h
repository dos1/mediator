#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>

struct CommonResources {
		// Fill in with common data accessible from all gamestates.
		struct {
				struct {
						ALLEGRO_SAMPLE *bg, *fg, *drums;
				} sample;
				struct {
						ALLEGRO_SAMPLE_INSTANCE *bg, *fg, *drums;
				} instance;
		} muzyczka;

		struct {
				int score;
				int lives;
				float modificator;
				struct Character *heart;
				char* next;
				int strike;
				bool pause;
		} mediator;

		ALLEGRO_FONT *font;
		ALLEGRO_FONT *font_bsod;
};

struct CommonResources* CreateGameData(struct Game *game);
void DestroyGameData(struct Game *game, struct CommonResources *resources);

bool GetAbstractIsItBonusLevelTimeNowFactoryProvider(struct Game *game);
void AdvanceLevel(struct Game *game, bool won);
void ShowLevelStatistics(struct Game *game);
