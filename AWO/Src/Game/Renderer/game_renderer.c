#include <stdlib.h>

#include "conf.h"
#include "GL_Helpers/gl_helpers.h"
#include "Game/Renderer/game_renderer.h"
#include "Game/Renderer/Render_Grid/render_grid.h"
#include "Game/Renderer/Sprite_Batch/sprite_batch.h"
#include "Game/Data/Palette/palette.h"

static Game_Renderer* renderer;

struct Game_Renderer {

    // Basic shader program for rendering any sprite.
    GLuint sprites_shader_program;

    // Shader program for rendering tile layers.
    GLuint tiles_shader_program;

    // Texture containing the game's raw sprite sheet.
    GLuint sprite_sheet_texture;

    // Texture containing palettes.
    GLuint palettes_texture;

    // Dimensions of the raw sprite sheet
    int sprite_sheet_width, sprite_sheet_height;

    // Layers of tile grids used to render tiles.
    Render_Grid* tile_grid_layers[TILE_LAYER_TYPE_COUNT];

    // VAO used by tiles layers
    GLuint tiles_layers_VAO;
};

int init_game_renderer_shader_programs()
{
    renderer->sprites_shader_program = create_shader_program(
        SHADERS_PATH "sprite.vert",
        SHADERS_PATH "sprite.frag"
    );

    renderer->tiles_shader_program = create_shader_program(
        SHADERS_PATH "tiles.vert",
        SHADERS_PATH "tiles.frag"
    );

    if (!renderer->sprites_shader_program || !renderer->tiles_shader_program) {
        return 0;
    }

    return 1;
}

void init_game_renderer_uniforms(
    int tiles_layer_width, 
    int tiles_layer_height
)
{
    // Sprites shader
    glUseProgram(renderer->sprites_shader_program);
    glUniform1i(glGetUniformLocation(renderer->sprites_shader_program, "sprite_sheet"), 0);
    glUniform1i(glGetUniformLocation(renderer->sprites_shader_program, "palettes_texture"), 1);

    // Tiles shader
    glUseProgram(renderer->tiles_shader_program);
    glUniform1i(glGetUniformLocation(renderer->tiles_shader_program, "sprite_sheet"), 0);
    glUniform1i(glGetUniformLocation(renderer->tiles_shader_program, "palettes_texture"), 1);
    glUniform1i(glGetUniformLocation(renderer->tiles_shader_program, "tiles_texture"), 2);

    glUniform1f(
        glGetUniformLocation(renderer->tiles_shader_program, "sprite_sheet_width"), 
        (GLfloat)renderer->sprite_sheet_width
    );

    glUniform1f(
        glGetUniformLocation(renderer->tiles_shader_program, "sprite_sheet_height"), 
        (GLfloat)renderer->sprite_sheet_height
    );

    glUniform1f(
        glGetUniformLocation(renderer->tiles_shader_program, "quad_width"), 
        (GLfloat)(tiles_layer_width * DEFAULT_TILE_DIMENSION)
    );

    glUniform1f(
        glGetUniformLocation(renderer->tiles_shader_program, "quad_height"), 
        (GLfloat)(tiles_layer_height * DEFAULT_TILE_DIMENSION)
    );

}

void init_game_renderer(
    int tile_layers_width, 
    int tile_layers_height, 
    GLuint palettes_texture,
    Tiles_Data* tiles_data
)
{
    renderer = (Game_Renderer*)malloc(sizeof(Game_Renderer));

    if (!init_game_renderer_shader_programs(renderer)) {
        free_game_renderer(renderer);
        return;
    }

    // Set textures
    renderer->sprite_sheet_texture = create_texture_object(
        SPRITE_SHEET_PATH, 
        &renderer->sprite_sheet_width, 
        &renderer->sprite_sheet_height
    );
    renderer->palettes_texture = palettes_texture;
    init_game_renderer_uniforms(tile_layers_width, tile_layers_height);

    // Set sprite batches
    /*
    game->sprite_batches[SPRITES_SPRITE_BATCH] = create_sprite_batch(
        sprites_shader_program,
        sprite_sheet_texture,
        game->palette_texture,
        MAX_SPRITE_BATCH_ELEMENTS
    );
    */

    // Set tiles layers
    renderer->tiles_layers_VAO = get_render_grid_VAO(tile_layers_width, tile_layers_height);

    // Get empty tile frames used to initially fill every layer
    Animation* empty_tile_frames;
    gather_tile_data(tiles_data, Empty, Default, NULL, NULL, &empty_tile_frames);

    for (int i = 0; i < TILE_LAYER_TYPE_COUNT; i++) {
        renderer->tile_grid_layers[i] = create_render_grid(tile_layers_width, tile_layers_height);
        
        fill_render_grid_pixels(
            renderer->tile_grid_layers[i], 
            (vec4) { 
                empty_tile_frames->frames[0].raw_top_left[0], 
                empty_tile_frames->frames[0].raw_top_left[1], 
                get_active_tile_palette_index(0), 
                0.0 
            }
        );
    }
}

void update_game_renderer_matrix(mat4 matrix, const char* matrix_str)
{
    // Update sprites shader's given matrix
    glUseProgram(renderer->sprites_shader_program);

    glUniformMatrix4fv(
        glGetUniformLocation(renderer->sprites_shader_program, matrix_str), 
        1, 
        GL_FALSE, 
        matrix[0]
    );

    // Update tiles shader's given matrix
    glUseProgram(renderer->tiles_shader_program);

    glUniformMatrix4fv(
        glGetUniformLocation(renderer->tiles_shader_program, matrix_str), 
        1, 
        GL_FALSE, 
        matrix[0]
    );
}

void update_tile_layer_pixels(
    Tile_Layer_Index layer,
    Point* points,
    Uint16 count,
    vec4 value
)
{
    update_render_grid_pixels(renderer->tile_grid_layers[layer], points, count, value);
}

void update_tile_layer_pixels_low(
    Tile_Layer_Index layer,
    Point* points,
    Uint16 count,
    vec2 value
)
{
    update_render_grid_pixels_low(renderer->tile_grid_layers[layer], points, count, value);
}

void render_tiles_layers()
{
    // Render tiles layers
    glUseProgram(renderer->tiles_shader_program);
    glBindVertexArray(renderer->tiles_layers_VAO);

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, renderer->sprite_sheet_texture);

    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, renderer->palettes_texture);

    for (int i = 0; i < TILE_LAYER_TYPE_COUNT; i++) {
        render_r_grid(renderer->tile_grid_layers[i]);
    }
}

void free_game_renderer()
{
    if (renderer != NULL) {

        // Free tiles layers
        for (int i = 0; i < TILE_LAYER_TYPE_COUNT; i++) {
            free_render_grid(renderer->tile_grid_layers[i]);
        }

        free(renderer);
    }
}