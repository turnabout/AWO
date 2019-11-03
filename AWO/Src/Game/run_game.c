#include <cglm/cglm.h>

#include "Game/Inputs/inputs.h"
#include "Game/Sprite_Batch/sprite_batch.h"
#include "Game/Data/Palette/palette.h"
#include "Game/_game.h"

void add_to_sprite_batch__test_palette(Sprite_Batch* sprite_batch, GLuint texture);

static Animation* test_tile_animation;
static Animation** test_unit_animations;
static GLuint test_texture;

void test_update_units_black_pixel(Game* game)
{
    static Unit_Variation unit_variation = OS;

    // Update unit variation to update
    if (get_key_state(KEY_1) == BUTTON_DOWN_START) { unit_variation = OS; }
    if (get_key_state(KEY_2) == BUTTON_DOWN_START) { unit_variation = BM; }
    if (get_key_state(KEY_3) == BUTTON_DOWN_START) { unit_variation = GE; }
    if (get_key_state(KEY_4) == BUTTON_DOWN_START) { unit_variation = YC; }
    if (get_key_state(KEY_5) == BUTTON_DOWN_START) { unit_variation = BH; }

    // Update black pixels of selected unit variation
    if (get_key_state(KEY_A) == BUTTON_DOWN_START) {
        glBindTexture(GL_TEXTURE_2D, game->palette_texture);
        update_palette_black_pixel(
            get_unit_palette_index_i(unit_variation, 0), 
            0
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (get_key_state(KEY_S) == BUTTON_DOWN_START) {
        glBindTexture(GL_TEXTURE_2D, game->palette_texture);
        update_palette_black_pixel(
            get_unit_palette_index_i(unit_variation, 0), 
            58
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (get_key_state(KEY_D) == BUTTON_DOWN_START) {
        glBindTexture(GL_TEXTURE_2D, game->palette_texture);
        update_palette_black_pixel(
            get_unit_palette_index_i(unit_variation, 0), 
            115
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (get_key_state(KEY_F) == BUTTON_DOWN_START) {
        glBindTexture(GL_TEXTURE_2D, game->palette_texture);
        update_palette_black_pixel(
            get_unit_palette_index_i(unit_variation, 0), 
            173
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void update_game(Game* game, float delta_time)
{
    update_keys_state();
    update_mouse_buttons_state();
    update_game_clock(game->clock, delta_time);

    test_update_units_black_pixel(game);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void test_palettes(Game* game)
{

    GLfloat unit_x = 500;
    for (Unit_Variation unit_var = UNIT_VAR_FIRST; unit_var < UNIT_VAR_COUNT; unit_var++) {
        add_to_sprite_batch(
            game->sprite_batches[SPRITES_SPRITE_BATCH], 
            (vec2) { unit_x, 500 }, 
            &(test_unit_animations[Right]->frames[0]),
            get_unit_palette_index(unit_var, 0)
        );

        unit_x += 25;
    }

    GLfloat tile_x = 500;
    for (Weather weather = WEATHER_FIRST; weather < WEATHER_COUNT; weather++) {
        add_to_sprite_batch(
            game->sprite_batches[SPRITES_SPRITE_BATCH], 
            (vec2) { tile_x, 550 }, 
            &test_tile_animation->frames[0],
            get_tile_palette_index(weather, 0)
        );

        add_to_sprite_batch(
            game->sprite_batches[SPRITES_SPRITE_BATCH], 
            (vec2) { tile_x, 571 }, 
            &test_tile_animation->frames[0],
            get_tile_palette_index(weather, 1)
        );

        tile_x += 25;
    }
}

void render_game(Game* game)
{
    // Draw main sprites
    begin_sprite_batch(game->sprite_batches[SPRITES_SPRITE_BATCH]);

    test_palettes(game);

    end_sprite_batch(game->sprite_batches[SPRITES_SPRITE_BATCH]);
}

void run_game(Game* game)
{
    // Test drawing 
    test_tile_animation = gather_tile_data(
        game->tiles_data, 
        River, Horizontal, 
        NULL, 
        NULL
    );

    test_unit_animations = get_unit_animations(
        game->units_data,
        Bomber,
        OS
    );

    // --------------------------------------------------------------
    static float delta_time = 0.0f;      // Time between current frame and last frame.
    static float last_frame_time = 0.0f; // Time of last frame.

    // Use vSync
    glfwSwapInterval(1);

    // Reset the GLFW timer before starting the game loop
    glfwSetTime(0);

    while (!glfwWindowShouldClose(game->window)) {

        // Update delta time
        float current_frame_time = (float)glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        update_game(game, delta_time);
        render_game(game);

        glfwSwapBuffers(game->window);
    }
}
