#include <stdlib.h>
#include <string.h>

#include "Game/Data/Palette/_palette.h"

GLuint create_game_palette_texture(
    GLuint raw_palette_texture, 
    Weather active_weather, 
    Army_Type player_armies[MAX_PLAYER_COUNT]
)
{
    // Get the raw palette texture data
    Palette_Texture_Row* raw_palette_texture_data = (Palette_Texture_Row*)malloc(
        sizeof(Palette_Texture_Row) * PALETTE_TEX_HEIGHT
    );

    glBindTexture(GL_TEXTURE_2D, raw_palette_texture);

    glGetTexImage(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        raw_palette_texture_data
    );

    // Create and populate destination the game palette texture data
    Palette_Texture_Row* game_palette_texture_data = (Palette_Texture_Row*)malloc(
        sizeof(Palette_Texture_Row) * PALETTE_TEX_HEIGHT
    );

    memset(game_palette_texture_data, 0, sizeof(Palette_Texture_Row) * PALETTE_TEX_HEIGHT);

    // Get the size of a single row in a palette texture
    // * 4 due to an RGBA array taking up 4 bytes
    size_t row_size = 4 * PALETTE_TEX_WIDTH;

    // Populate active tile palette
    memcpy(
        &game_palette_texture_data[get_active_tile_palette_index_i(FALSE)],
        &raw_palette_texture_data[get_raw_tile_palette_index_i(active_weather, FALSE)],
        row_size
    );

    memcpy(
        &game_palette_texture_data[get_active_tile_palette_index_i(1)],
        &raw_palette_texture_data[get_raw_tile_palette_index_i(active_weather, TRUE)],
        row_size
    );

    // Populate player units & player properties palettes
    for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
        Army_Type player_army = player_armies[i];

        if (player_army == ARMY_TYPE_NONE) {
            continue;
        }

        // Populate player units palettes
        memcpy(
            &game_palette_texture_data[get_player_unit_palette_index_i(i, FALSE)],
            &raw_palette_texture_data[get_raw_unit_palette_index_i(player_army, FALSE)],
            row_size
        );

        memcpy(
            &game_palette_texture_data[get_player_unit_palette_index_i(i, TRUE)],
            &raw_palette_texture_data[get_raw_unit_palette_index_i(player_army, TRUE)],
            row_size
        );

        // Populate properties palettes
        memcpy(
            &game_palette_texture_data[get_player_property_palette_index_i(i)],
            &raw_palette_texture_data[get_raw_property_palette_index_i(player_army)],
            row_size
        );
    }

    // Populate neutral & fog property palettes
    memcpy(
        &game_palette_texture_data[get_player_property_palette_index_i(Player_Index_Neutral)],
        &raw_palette_texture_data[get_raw_property_palette_index_i(ARMY_TYPE_NEUTRAL)],
        row_size
    );

    memcpy(
        &game_palette_texture_data[ get_fog_property_palette_index_i()],
        &raw_palette_texture_data[get_raw_fog_property_palette_index_i()],
        row_size
    );

    // Create the texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA, 
        PALETTE_TEX_WIDTH, 
        PALETTE_TEX_HEIGHT, 
        0, 
        GL_RGBA, 
        GL_UNSIGNED_BYTE, 
        game_palette_texture_data
    );

    free(game_palette_texture_data);
    free(raw_palette_texture_data);

    // Before exiting, apply the initial weather colors to the properties palettes
    update_properties_weather_colors(texture);

    return texture;
}
