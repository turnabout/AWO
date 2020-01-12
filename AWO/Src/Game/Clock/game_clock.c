#include <stdlib.h>

#include "Game/Clock/_game_clock.h"

Game_Clock* create_game_clock(Clock_Data* clock_data)
{
    Game_Clock* game_clock = malloc(sizeof(Game_Clock));

    game_clock->frame_count = 0;
    game_clock->accum_time = 0;

    // Create the pub-sub module responsible for linking the game clock publisher with subscribers
    game_clock->pub_sub = create_clock_pub_sub();

    // Create the timers array (publishers)
    for (Clock_Index i = 0; i < ANIMATION_CLOCK_COUNT; i++) {
        game_clock->timers[i] = create_clock_timer(clock_data, i, game_clock->pub_sub);
    }

    /*
    game_clock->tile_subscriber = NULL;
    game_clock->property_lights_subscriber = NULL;

    // Create the subscriber modules - Tiles clock subscriber module
    game_clock->tile_subscriber = create_game_clock_tile_subscriber(game_renderer);

    register_clocks_pub_sub_subscriber(
        game_clock->pub_sub,
        (void*)game_clock->tile_subscriber,
        process_tile_subscriber_event,
        (Clock_Index[ANIMATION_CLOCK_COUNT]) {
            Sea_Clock_0,
            Sea_Clock_1,
            Sea_Clock_2,
            River_Clock,
            Smoke_Clock
        },
        5
    );

    // Property lights clock subscriber module
    game_clock->property_lights_subscriber = create_game_clock_property_lights_subscriber(
        game_palette
    );

    register_clocks_pub_sub_subscriber(
        game_clock->pub_sub,
        (void*)game_clock->property_lights_subscriber,
        process_property_light_tick_event,
        (Clock_Index[ANIMATION_CLOCK_COUNT]) {
            Property_Lights_Clock
        },
        1
    );
    */

    return game_clock;
}

void update_game_clock(Game_Clock* game_clock, float delta_time)
{

    // Reached time for a frame
    if ((game_clock->accum_time += delta_time) >= FRAME_TIME) {

        // Get frame count
        float frame_count = game_clock->accum_time / FRAME_TIME;

        for (int i = 0; i < ANIMATION_CLOCK_COUNT; i++) {
            update_timer(game_clock->timers[i], (int)frame_count);
        }

        game_clock->accum_time -= (int)frame_count * FRAME_TIME;
    }
}

void register_game_clock_tile(
    Game_Clock* game_clock, 
    Tile* tile,
    Clock_Index clock_index
)
{
    register_clock_subscriber_tile(game_clock->tile_subscriber, tile, clock_index);
}

void unregister_game_clock_tile(
    Game_Clock* game_clock, 
    Tile* tile,
    Clock_Index clock_index
)
{
    unregister_clock_subscriber_tile(game_clock->tile_subscriber, tile, clock_index);
}

void free_game_clock(Game_Clock* game_clock)
{
    if (game_clock == NULL) {
        return;
    }

    for (Clock_Index i = 0; i < ANIMATION_CLOCK_COUNT; i++) {
        free_clock_timer(game_clock->timers[i]);
    }

    free_clock_pub_sub(game_clock->pub_sub);
    // free_game_clock_tile_subscriber(game_clock->tile_subscriber);
    // free_game_clock_property_lights_subscriber(game_clock->property_lights_subscriber);
}