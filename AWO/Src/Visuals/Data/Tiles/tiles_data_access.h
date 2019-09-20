#pragma once

#include "C_Hashmap/hashmap.h"
#include "../general_data_access.h"
#include "../Palettes/palette_tree.h"

#include "../AnimationClocks/animation_clocks_enums.h"
#include "tiles_enums.h"

/**
 * \brief A tile's visual data.
 */
typedef struct
{
    map_t vars;            // Map holding every variation (Tile_Var_Data)
    int vars_amount;       // Amount of variations this tile has
    tile_var* vars_list;   // List of every variation this tile has

    Animation_Clock_Index clock; // Clock used by this tile to update its animation
} Tile_Data;

/**
 * \brief A tile's variation's visual data.
 */
typedef struct
{
    Animation_Sub_Clock_Index sub_clock; // Sub clock used by this tile var to update its animation
    Animation* anim;                     // This tile variation's animation
} Tile_Var_Data;

/**
 * \brief All visual data used by tiles.
 */
typedef struct
{
    Tile_Data* src[TILE_TYPE_AMOUNT];
    SS_Meta_Data* ss_meta_data;
} Tiles_Data;

/**
 *  \brief Gets a tile's data.
 *
 *  \param type The tile type.
 *
 *  \return Returns the tile data associated with the given tile type.
 */
Tile_Data* access_tile(tile_type type);

/**
 *  \brief Gets a tile's variation's data.
 *
 *  \param type The tile type.
 *
 *  \param var The tile variation.
 *
 *  \return Returns the tile variation data associated with the given tile type and tile variation.
 *          Returns NULL if given variation was not found on the tile type.
 */
Tile_Var_Data* access_tile_var(tile_type type, tile_var var);

// Debug function printing the entire data of a tile type
void debug_print_tile_data(tile_type type);
