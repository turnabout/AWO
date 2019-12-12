#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "Game/Data/Tile/tiles_data.h"

typedef struct Game_Renderer Game_Renderer;

typedef enum Tile_Layer_Index {
    TILE_LAYER_0,
    TILE_LAYER_1,
} Tile_Layer_Index;

#define TILE_LAYER_TYPE_COUNT TILE_LAYER_1 + 1

void init_game_renderer(
    int tile_layers_width, 
    int tile_layers_height, 
    GLuint game_palette_texture,
    Tiles_Data* tiles_data
);

/*! @brief Update one of the matrices used by the game renderer to render objects.
 *
 *  @param[in] matrix The new matrix.
 *  @param[in] matrix_str The string representing the matrix to update ("view"|"projection").
 */
void update_game_renderer_matrix(mat4 matrix, const char* matrix_str);

/*! @brief Render the game renderer's tiles layers.
 */
void render_game_renderer();

/*! @brief Clear a pixel in the game renderer's tile layers.
 *
 *  @param[in] x X coordinate of the pixel to clear.
 *  @param[in] y Y coordinate of the pixel to clear.
 */
void clear_tile_layers_pixel(Uint8 x, Uint8 y);

/*! @brief Update a pixel's value in a render grid layer's tile texture.
 *
 *  @param[in] layer Which layer to update.
 *  @param[in] x X offset of the pixel to update.
 *  @param[in] y Y offset of the pixel to update.
 *  @param[in] value The value used to update with.
 */
void update_tile_layer_pixel(
    Tile_Layer_Index layer,
    Uint8 x,
    Uint8 y,
    vec4 value
);

void update_tile_layer_pixel_high(
    Tile_Layer_Index layer,
    Uint8 x,
    Uint8 y,
    vec2 value
);

void update_tile_layer_pixel_low(
    Tile_Layer_Index layer,
    Uint8 x,
    Uint8 y,
    vec2 value
);

void free_game_renderer();
