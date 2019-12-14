#include <stdlib.h>
#include <string.h>

#include "Game/Stage/_stage.h"
#include "Utilities/utilities.h"

// TODO: remove
void allocate_stage_grid(Stage* stage)
{
    stage->tiles_grid = malloc(sizeof(Stage_Tile_Row) * stage->height);

    for (Uint8 y = 0; y < stage->height; y++) {
        stage->tiles_grid[y] = malloc(sizeof(Stage_Tile) * stage->width);
    }
}

Stage* create_stage(
    char* stage_name,
    Uint8 width,
    Uint8 height,
    Uint8 player_count,
    Army_Type player_armies[MAX_PLAYER_COUNT]
)
{
    Stage* stage = malloc(sizeof(Stage));

    stage->width = width;
    stage->height = height;
    stage->player_count = player_count;
    memcpy(stage->name, stage_name, STAGE_NAME_MAX_LENGTH);

    for (int i = 0; i < player_count; i++) {
        stage->player_armies[i] = player_armies[i];
    }

    // Allocate space for tile grid
    stage->tiles_grid = malloc(sizeof(Stage_Tile_Row) * stage->height);

    for (Uint8 y = 0; y < stage->height; y++) {
        stage->tiles_grid[y] = malloc(sizeof(Stage_Tile) * stage->width);
    }

    return stage;
}

void inline edit_stage_tile(Stage* stage, int x, int y, int type, int variation)
{
    stage->tiles_grid[y][x].type = type;
    stage->tiles_grid[y][x].variation = variation;
}

void fill_stage_tiles(Stage* stage, int type, int variation)
{
    for (Uint8 y = 0; y < stage->height; y++) {
        for (Uint8 x = 0; x < stage->width; x++) {
            edit_stage_tile(stage, x, y, type, variation);
        }
    }
}

void free_stage(Stage* stage)
{
    if (stage == NULL) {
        return;
    }

    if (stage->tiles_grid != NULL) {

        for (int y = 0; y < stage->height; y++) {
            if (stage->tiles_grid[y] != NULL) {
                free(stage->tiles_grid[y]);
            }
        }

        free(stage->tiles_grid);
    }

    free(stage);
}