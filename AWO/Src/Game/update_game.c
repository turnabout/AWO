#include "Game/_game.h"

#define CONTROL_MODE_NORMAL 0
#define CONTROL_MODE_PAN    1

static int control_mode = CONTROL_MODE_NORMAL;

void update_game(Game* game, float delta_time)
{
    update_keyboard_state();
    update_mouse_buttons_state();

    update_game_clock(game->clock, delta_time);
    update_game_board(game->board);

    // printf("%d\n", get_mouse_scroll_value());

    // Update panning
    if (control_mode == CONTROL_MODE_PAN) {
        update_camera_mouse_pan_mode(game->camera);

        // End "pan" control mode
        if (
            get_key_state(KEY_SPACE) == BUTTON_UP || 
            get_mouse_button_state(MOUSE_BUTTON_LEFT) == BUTTON_UP
        ) {
            end_camera_mouse_pan_mode(game->camera);
            control_mode = CONTROL_MODE_NORMAL;
        }

    } else if (control_mode == CONTROL_MODE_NORMAL) {

        // Start "pan" control mode
        if (get_key_state(KEY_SPACE) == BUTTON_DOWN &&
            get_mouse_button_state(MOUSE_BUTTON_LEFT) == BUTTON_DOWN) {

            start_camera_mouse_pan_mode(game->camera);
            control_mode = CONTROL_MODE_PAN;
        }

        // Zoom at mouse coordinates if mouse wheel was scrolled
        float scroll_value;

        if (scroll_value = get_mouse_scroll_value()) {
            int mouse_x, mouse_y;
            get_mouse_position(&mouse_x, &mouse_y);
            zoom_game_camera(game->camera, scroll_value, mouse_x, mouse_y);
        }
    }

    /*
    if (get_key_state(KEY_UP) == BUTTON_DOWN_START) {
        // Get mouse position
        int mouse_x, mouse_y;
        get_mouse_position(&mouse_x, &mouse_y);

        // zoom_game_camera(game->camera, 1.0f, mouse_x, mouse_y);
        zoom_game_camera(game->camera, 1.0f, mouse_x, mouse_y);
    }

    if (get_key_state(KEY_DOWN) == BUTTON_DOWN_START) {
        // zoom_camera_out(game->camera, 1.0f, game->window_width / 2, game->window_height / 2);
    }
    */

    /*
    if (get_key_state(KEY_W) == BUTTON_DOWN) {
        update_game_camera_view_position(game->camera, 0.0f, -3.0f);
    }

    if (get_key_state(KEY_S) == BUTTON_DOWN) {
        update_game_camera_view_position(game->camera, 0.0f, 3.0f);
    }

    if (get_key_state(KEY_A) == BUTTON_DOWN_START) {
        update_game_camera_view_position(game->camera, 1.0f, 0.0f);
    }

    if (get_key_state(KEY_D) == BUTTON_DOWN_START) {
        update_game_camera_view_position(game->camera, -1.0f, 0.0f);
    }
    */
}