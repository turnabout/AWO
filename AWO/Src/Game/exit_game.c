#include <stdio.h>
#include <stdlib.h>

#include "Game/Inputs/inputs.h"
#include "Game/Data/Palette/palette.h"
#include "Game/_game.h"

void exit_game(Game* game)
{
    glfwTerminate();

    if (game != NULL) {
        free_tiles_data(game->tiles_data);
        free_units_data(game->units_data);
        free_game_clock(game->clock);
        free_keys_state_module();
        free_mouse_state_module();
        free_palette_NDC_indexes();
        free(game);
    }
}
