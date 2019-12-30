#pragma once

/*! @brief Game instance.
 */
typedef struct Game Game;

/*! @brief Every possible state for the game.
 */
typedef enum Game_State {

    // The game instance is created, but not prepared for gameplay.
    Game_Created,

    // The game is prepared for gameplay.
    Game_Prepared,

    // The game is currently running.
    Game_Running,
} Game_State;

/*! @brief Creates the game instance.
 *
 *  @param[in] window_width The window's initial tiles_width, in pixels.
 *  @param[in] window_height The window's initial tiles_height, in pixels.
 *  @return The created game instance with state of `Game_Initialized`, NULL if an error occurred.
 */
Game* create_game(int window_width, int window_height);

/*! @brief Updates AWO's game window dimensions.
 *
 *  @param[in] game The game object.
 *  @param[in] width The new window width.
 *  @param[in] height The new window height.
 */
void update_game_window_dimensions(Game* game, int width, int height);

/*! @brief Prepares game for "design room" (stage editor) mode.
 *
 *  @param[in] game The game.
 *  @return 1 if the game was successfully prepared, 0 if an error occurred.
 */
int prepare_design_room_game(Game* game);

/*! @brief Undoes the steps done in game preparation.
 *
 * @note Frees memory used by components initialized during game preparation. Should be used after 
 * the game loop is done running.
 *
 *  @param[in] game The game object.
 */
void unprepare_game(Game* game);

/*! @brief Runs the main game loop until `glfwWindowShouldClose` evaluates false.
 *
 *  @param[in] game The game.
 *  @param[in] callback A function called every frame before the game loop.
 */
void run_game(Game* game, void (*callback)(Game*));

/*! @brief Gets the game's current state.
 *
 *  @param[in] game The game.
 *  @return The game's currently active state.
 */
Game_State get_game_state(Game* game);

/*! @brief Frees all resources taken up by the game and kills it.
 *
 *  @note Expects game state to be `Game_Initialized`.
 *
 *  @param[in] game The game instance to free.
 */
void free_game(Game* game);
