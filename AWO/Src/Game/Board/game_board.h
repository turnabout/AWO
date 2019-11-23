#pragma once

#include "Game/Clock/game_clock.h"
#include "Game/Stage/stage.h"
#include "Game/Clock/game_clock.h"
#include "Game/Data/Tile/tiles_data.h"
#include "Game/Player/player.h"

/*! @brief Represents the game board used by players during gameplay. Contains all game entities.
 */
typedef struct Game_Board Game_Board;

/*! @brief Creates game board loaded with the given data.
 *
 *  @param[in] game_clock The game clock.
 *  @param[in] tiles_data The tiles data object.
 *  @param[in] stage_descriptor Descriptor detailing the stage to load.
 *  @param[in] player_COs Array containing every CO used by every player, in order.
 *  @return The created game board object.
 */
Game_Board* create_game_board(
    Game_Clock* game_clock,
    Tiles_Data* tiles_data,
    Stage* stage_descriptor,
    CO_Type player_COs[MAX_PLAYER_COUNT]
);

/*! @brief Updates the game board and its entities.
 *
 *  @param[in] game_board The game board.
 */
void update_game_board(Game_Board* game_board);

/*! @brief Frees all memory occupied by the game board and its contents.
 *
 *  @param[in] game_board The game board.
 */
void free_game_board(Game_Board* game_board);
