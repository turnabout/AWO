#include "Game/_game.h"

#include "Game/Data/Palette/palette.h"

void add_to_sprite_batch__test_palette(Sprite_Batch* sprite_batch);

void render_game(Game* game)
{
    render(game->tiles_renderer, 0, 0);



    // Draw main sprites
    // begin_sprite_batch(game->sprite_batches[SPRITES_SPRITE_BATCH]);

    // add_to_sprite_batch__test_palette(game->sprite_batches[SPRITES_SPRITE_BATCH]);
    // draw_game_board(game->board, game->sprite_batches[SPRITES_SPRITE_BATCH]);

    // end_sprite_batch(game->sprite_batches[SPRITES_SPRITE_BATCH]);
}
