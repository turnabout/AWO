#include <cglm/cglm.h>

#include "Game/_game.h"
#include "Game/Data/Palette/game_palette.h"

void render_game(Game* game)
{
    // Render the main grids
    render_game_renderer_grids(game->renderer);

    // Queue extras for render
    render_game_pointer(game->pointer, game->renderer);

    render_extras(game->renderer);
}
