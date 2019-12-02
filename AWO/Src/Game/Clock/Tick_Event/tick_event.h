#pragma once

#include "types.h"
#include "Game/Clock/enums.h"

// Event generated whenever an animation sub-clock ticks.
typedef struct Tick_Event {

    // Index of the animation clock that ticked.
    Animation_Clock_Index clock_index;

    // Index of the animation sub-clock that ticked.
    Animation_Sub_Clock_Index sub_clock_index;

    // Value held by this tick event.
    Uint8 value;

} Tick_Event;

// List of tick events.
typedef struct Tick_Events_List {

    // List of ticks.
    Tick_Event ticks[ANIMATION_SUB_CLOCK_COUNT];

    // Amount of ticks.
    Uint8 ticks_count;

} Tick_Events_List;

#ifdef _DEBUG
/*! @brief Prints the contents of the given list of tick events.
 *
 *  @param[in] tick_events_list The list of tick events to print.
 */
void print_tick_events_list(Tick_Events_List* tick_events_list);
#endif