#include <stdlib.h>

#include "Game/Data/Palette/_palette.h"

// TODO: take in a unit variation as an arg instead of a palette index
void update_unit_palette_black_pixel(GLuint palette_index, GLubyte new_color)
{
    GLubyte new_color_array[4] = { new_color, new_color, new_color, 255 };

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        palette_index,
        1,
        1,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        &new_color_array
    );
}

void update_active_tile_palette(Weather weather)
{
    // TODO: redo for game palette (this code was for raw palette)
    /*
    GLuint new_tile_palette_index = get_tile_palette_index_i(weather, FALSE);
    GLuint new_fog_tile_palette_index = get_tile_palette_index_i(weather, TRUE);

    // Get selected weather palette data and transfer into 
    Palette_Texture_Row* texture_data = malloc(sizeof(Palette_Texture_Row) * PALETTE_TEX_HEIGHT);

    glGetTexImage(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        texture_data
    );

    // Update active tile/fog palettes with new palettes
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        get_active_tile_palette_index_i(FALSE),
        PALETTE_TEX_WIDTH,
        1,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        &texture_data[new_tile_palette_index]
    );

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        get_active_tile_palette_index_i(TRUE),
        PALETTE_TEX_WIDTH,
        1,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        &texture_data[new_fog_tile_palette_index]
    );

    // Update property palettes' weather colors
    // TODO

    free(texture_data);
    */
}