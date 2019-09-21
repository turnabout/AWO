#pragma once

// Every possible animation clock's index.
typedef enum {
    Sea_Clock,
    River_Clock,
    // Units_Clock,

    // None
    No_Clock = -1,
} Animation_Clock_Index;

#define ANIMATION_CLOCK_FIRST Sea_Clock
#define ANIMATION_CLOCK_LAST River_Clock
#define ANIMATION_CLOCK_COUNT ANIMATION_CLOCK_LAST + 1

// Every possible animation sub clock's index.
typedef enum {
    // Sea Clock
    Pyramid1 = 0,
    Pyramid2 = 1,
    Ten      = 2,

    // River Clock
    Eight    = 0,

    // None
    No_Sub_Clock = -1,
} Animation_Sub_Clock_Index;
