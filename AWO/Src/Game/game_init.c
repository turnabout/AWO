#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#include "Visuals/visuals_data.h"
#include "Visuals/visuals_processing.h"
#include "Utilities/utilities.h"
#include "conf.h"
#include "game.h"

int get_game_visuals_data(Game* game);
int init_SDL_components(Game* game, int w, int h);

Game* init_game(Game_Arg_Weathers weathers, int w, int h)
{
    Game* game = malloc(sizeof(Game));

    // Add essential components
    if (init_SDL_components(game, w, h) == ERR) {
        return NULL;
    }

    // Add the visual data to the game instance
    if (get_game_visuals_data(game) == ERR) {
        printf("Error getting game's visual data\n");
        return NULL;
    }

    // Add tile textures, processing weathers
    game->current_weather = -1;

    for (Weather w = WEATHER_FIRST; w < WEATHER_COUNT; w++) {
        if ((1 << w) & weathers) {

            // Assign starting weather to first active one
            if (game->current_weather == -1) {
                game->current_weather = w;
            }
            
            // Add tile texture for this weather
            game->tile_textures[w] = create_tiles_texture(game, w);
        }
    }

    // TODO: Add unit textures used by the game

    // Add other game components
    game->board = GB_create();
    game->sel = tile_selector_create(game->td);

    // Fill game board with initial tiles
    GB_fill(game->board, game->clock, game->td, Sea, Middle);

    // Initialize the input state module
    input_state_init();
    game->mouse_state = get_mouse_state();

    return game;
}

int init_SDL_components(Game* game, int w, int h)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_FLAGS) != 0) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return ERR;
    }

    // Initialize window
    game->win = SDL_CreateWindow(
        GAME_TITLE,              // Window title
        SDL_WINDOWPOS_CENTERED,  // Initial x pos
        SDL_WINDOWPOS_CENTERED,  // Initial y pos
        w,                       // Screen width
        h,                       // Screen height
        SDL_WINDOW_FLAGS         // Flags
    );

    if (game->win == NULL) {
        printf("SDL_CreateWindow: %s\n", SDL_GetError());
        return ERR;
    }

    // Initialize renderer
    game->rend = SDL_CreateRenderer(
        game->win, // Window where rendering is displayed
        -1,        // Index of rendering driver to initialize
        REND_INIT_FLAGS
    );

    if (game->rend == NULL) {
        printf("SDL_CreateRenderer: %s\n", SDL_GetError());
        return ERR;
    }

    // Clear screen using start background color as soon as possible
    SDL_SetRenderDrawColor(game->rend, START_BG_R, START_BG_G, START_BG_B, START_BG_A);
    SDL_RenderClear(game->rend);

    SDL_SetRenderDrawBlendMode(game->rend, SDL_BLENDMODE_BLEND);

    if (SDL_RenderSetLogicalSize(game->rend, w, h) != 0) {
        printf("SDL_RenderSetLogicalSize: %s\n", SDL_GetError());
        return ERR;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

    // Initialize sprite sheet surface
    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("IMG_Init: %s\n", IMG_GetError());
        return ERR;
    }

    SDL_Surface* surface = IMG_Load(SPRITE_SHEET_PATH);

    if (surface == NULL) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return ERR;
    }

    SDL_Surface* formatted_surface = SDL_ConvertSurfaceFormat(
        surface, 
        SDL_PIXELFORMAT_RGBA8888, 
        0
    );

    game->ss = SDL_CreateTextureFromSurface(game->rend, formatted_surface);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(formatted_surface);

    if (game->ss == NULL) {
        printf("Error creating sprite sheet texture: %s\n", IMG_GetError());
        return ERR;
    }

    // Add Font
    game->font = FC_CreateFont();

    FC_LoadFont(
        game->font, 
        game->rend, 
        FONT_PATH, 
        FONT_SIZE, 
        FC_MakeColor(0, 0, 0, 255), 
        TTF_STYLE_NORMAL
    );

    return OK;
}

int get_visuals_JSON(const cJSON **visuals_JSON)
{
    // Get file contents & size
    char *f_contents;
    int size;

    if ((size = read_file(JSON_PATH, &f_contents)) == ERR) {
        return ERR;
    }

    // Populate visuals JSON struct
    *visuals_JSON = cJSON_Parse(f_contents);

    // Clear previously accumulated file contents
    memset(f_contents, 0, size);
    free(f_contents);

    return OK;
}

int get_game_visuals_data(Game* game)
{
    // Load visuals JSON contents into cJSON struct
    const cJSON* visuals_JSON;

    if (get_visuals_JSON(&visuals_JSON) == ERR) {
        return ERR;
    }

    // Get all visual data and attach to game
    game->ud = UD_create_from_JSON(
        cJSON_GetObjectItemCaseSensitive(visuals_JSON, "units")
    );

    game->td = TD_create_from_JSON(
        cJSON_GetObjectItemCaseSensitive(visuals_JSON, "tiles")
    );

    // Get game clock and animation clocks
    game->clock = GC_create(
        cJSON_GetObjectItemCaseSensitive(visuals_JSON, "animationClocks")
    );

    return OK;
}
