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
 *  @param[in] stage The stage to load.
 *  @param[in] player_COs Array containing every CO used by every player, in order.
 *  @param[in] raw_palette_texture The texture holding all palette data.
 *  @param[in] game_palette_texture The palette texture used for this current ongoing game.
 *  @return The created game board object.
 */
Game_Board* create_game_board(
    Game_Clock* game_clock,
    Tiles_Data* tiles_data,
    Stage* stage,
    CO_Type player_COs[MAX_PLAYER_COUNT],
    GLuint raw_palette_texture,
    GLuint game_palette_texture
);

/*! @brief Edits type/variation of the tile at the given game board coordinates.
 *
 *  @param[in] game_board The game board.
 *  @param[in] game_clock The game clock.
 *  @param[in] tiles_data The tiles data object.
 *  @param[in] new_type New type of the tile.
 *  @param[in] new_variation New variation of the tile.
 *  @param[in] x Game board x coordinate of the new tile.
 *  @param[in] y Game board y coordinate of the new tile.
 */
void edit_game_board_tile(
    Game_Board* game_board,
    Game_Clock* game_clock,
    Tiles_Data* tiles_data,
    Tile_Type new_type,
    int new_variation,
    Uint8 x,
    Uint8 y
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
