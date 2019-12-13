#include <stdlib.h>

#include "Game/Data/Tile/Property_Tile/_property_tiles_data.h"
#include "Game/Data/Tile/_tiles_data.h"

void update_tiles_data_active_property_weather_var(
    Tiles_Data* tiles_data,
    Weather weather_variation
)
{
    Weather used_weather =
        (weather_variation < PROPERTY_WEATHER_COUNT)
            ? weather_variation
            : WEATHER_FIRST;

    tiles_data->property_tiles->active_src = tiles_data->property_tiles->src[used_weather];

}

Frame* get_property_type_frame(
    Tiles_Data* tiles_data, 
    Property_Type property_type, 
    Army_Type army_variation
)
{
    // Get the data for the given property type
    Property_Type_Data* prop_type_data = tiles_data->property_tiles->active_src[property_type];

    // If given army variation doesn't exist on unit type, return data for default variation
    Army_Type used_army_variation = 
        (army_variation < prop_type_data->army_variation_count)
            ? army_variation
            : ARMY_TYPE_FIRST;

    return prop_type_data->frames[used_army_variation];
}

void free_properties_data(Tiles_Data* tiles_data)
{
    for (Weather weather = WEATHER_FIRST; weather < PROPERTY_WEATHER_COUNT; weather++) {
        for (Property_Type type = PROPERTY_TYPE_FIRST; type < PROPERTY_TYPE_COUNT; type++) {
            free(tiles_data->property_tiles->src[weather][type]->frames);
            free(tiles_data->property_tiles->src[weather][type]);
        }
    }

    free(tiles_data->property_tiles);
}
