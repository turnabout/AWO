#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>

#include "conf.h"
#include "Game/Data/Palette/game_palette.h"
#include "Game/Inputs/inputs.h"
#include "Game/Player/player.h"
#include "Game/_game.h"

Bool load_level(Game* game, Stage* stage, CO_Type player_COs[MAX_PLAYER_COUNT])
{
    if (stage == NULL) {
        return FALSE;
    }

    // Load components used for rendering the game
    game->palette_texture = create_game_palette_texture(
        game->raw_palette_texture, 
        WEATHER_DEFAULT, 
        stage->player_armies
    );

    if ((game->renderer = create_game_renderer(
        stage->width,
        stage->height,
        game->palette_texture,
        game->tiles_data,
        game->window_width,
        game->window_height
    )) == NULL) {
        return FALSE;
    }

    activate_game_clock_subscribers(
        game->clock,
        game->renderer,
        &game->palette_texture
    );

    game->camera = create_game_camera(
        game->renderer, 
        &game->window_width, 
        &game->window_height, 
        stage
    );

    // Set up game board
    game->board = create_game_board(
        game->clock, 
        game->renderer,
        stage, 
        player_COs,
        game->tiles_data, 
        game->raw_palette_texture,
        game->palette_texture
    );

    return TRUE;
}

Game* init_game(int window_width, int window_height)
{
    Game* game = malloc(sizeof(Game));

    // Initialize base game components: GLFW, GLAD and game data.
    if (!init_GL(game, window_width, window_height) || !init_game_data(game)) {
        return NULL;
    }

    // Initialize input handling modules
    init_keyboard_module(game->window);
    game->mouse_state = init_mouse_module(game->window);

    // Initialize the pointer going over hovered tiles
    game->pointer = create_game_cursor(game->UI_data);

    // Load the level
    Stage* stage = game->stages[1];
    stage = generate_filled_stage(Sea, Middle, 255, 255);
    stage = generate_custom_stage();

    CO_Type player_COs[MAX_PLAYER_COUNT] = {Andy, Eagle};

    if (!load_level(game, stage, player_COs)) {
        return NULL;
    }

    // Initialize the editor for the design room mode
    game->editor = create_game_editor(
        game->tiles_data, 
        game->units_data, 
        &game->window_width, 
        &game->window_height
    );

    return game;
}