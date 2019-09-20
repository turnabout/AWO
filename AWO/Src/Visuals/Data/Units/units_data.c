#include "_units_data_internal.h"

#pragma warning( disable : 6011 )

Units_Data* UD_create_from_JSON(const cJSON* ud_JSON)
{
    Units_Data* ud = malloc(sizeof(Units_Data));

    // Store metadata & JSON
    ud->ss_metadata = SS_Meta_create_from_JSON(ud_JSON);
    ud->visuals_JSON = ud_JSON;

    // Store source and destination data
    get_units_src_data(ud, cJSON_GetObjectItemCaseSensitive(ud_JSON, "src"));
    get_units_dst_data(ud, cJSON_GetObjectItemCaseSensitive(ud_JSON, "dst"));

    return ud;
}

void get_units_src_data(Units_Data* ud, const cJSON* src_json)
{
    const cJSON* unit_type_json;
    enum unit_type current_type = UNIT_TYPE_FIRST;

    cJSON_ArrayForEach(unit_type_json, src_json)
    {
        ud->src[current_type++] = get_unit_src_data(unit_type_json);
    }
}

void get_units_dst_data(Units_Data* ud, const cJSON* dst_json)
{
    const cJSON* unit_type_json;
    enum unit_type current_type = UNIT_TYPE_FIRST;

    cJSON_ArrayForEach(unit_type_json, dst_json)
    {
        const cJSON* unit_anim_json;
        enum unit_type current_anim = UNIT_ANIM_FIRST;

        cJSON_ArrayForEach(unit_anim_json, unit_type_json)
        {
            ud->dst[current_type][current_anim++] = AD_create_from_JSON(unit_anim_json);
        }

        current_type++;
    }
}

Src_Unit_Type* get_unit_src_data(const cJSON* unit_type_json)
{
    Src_Unit_Type* type;        // The unit type data struct returned
    Animation_Data*** vars;     // Data for this unit type's variations
    const cJSON* unit_var_json; // cJSON data for current variation

    // Initialize unit type and its variation count
    type = malloc(sizeof(struct Src_Unit_Type));
    type->vars_count = cJSON_GetArraySize(unit_type_json);

    // Initialize Variations' memory
    vars = malloc(sizeof(Animation_Data**) * type->vars_count);

    // Loop variations
    cJSON_ArrayForEach(unit_var_json, unit_type_json)
    {
        const cJSON* unit_anim_json; // cJSON data for current Animation
        Animation_Data** anims;      // All Animations data for this Variation

        // Initialize Animations' memory
        anims = malloc(sizeof(Animation_Data*) * UNIT_ANIM_AMOUNT); 

        // Loop animations
        cJSON_ArrayForEach(unit_anim_json, unit_var_json)
        {
            *(anims++) = AD_create_from_JSON(unit_anim_json);
        };

        // Reset anims pointer to the first variation, then save accumulated data
        anims -= UNIT_ANIM_AMOUNT;
        *(vars++) = anims;
    }

    vars -= type->vars_count;
    type->vars = vars;

    return type;
}

// TODO: Instead, return a Palette_Tree* and store the 'flip' value in an additional pointer arg
struct Palette_Tree* get_unit_palette(unit_var u_var, int* flip)
{
    // TODO: Can't rely on static JSON anymore, use given Units_Data* instead
    /*
    const cJSON* base = cJSON_GetObjectItemCaseSensitive(units_visuals_JSON, "basePalette");
    const cJSON* unit_palettes = cJSON_GetObjectItemCaseSensitive(units_visuals_JSON, "palettes");

    // Get cJSON item for the given palette
    const cJSON* unit_palette = cJSON_GetArrayItem(unit_palettes, u_var);

    if (cJSON_GetArraySize(unit_palette) < 1) {
        return NULL;
    }

    // Build up the result
    struct Unit_Palette* result = malloc(sizeof(struct Unit_Palette));

    result->flip = cJSON_GetObjectItemCaseSensitive(unit_palette, "flip")->valueint;
    result->palette = PT_create_from_JSON(
        base, 
        cJSON_GetObjectItemCaseSensitive(unit_palette, "palette")
    );

    return result;
    */
    return NULL;
}

Animation_Data** access_unit_src_anims(unit_type type, unit_var var)
{
    // If variation doesn't exist on unit type, return default instead
    /*
    unit_var returned_var; = (var < units_data->src[type]->vars_count)
        ? var
        : UNIT_VAR_FIRST;

    return units_data->src[type]->vars[returned_var];
*/
    return NULL;
}

Animation_Data** access_unit_dst_anims(unit_type type)
{
    // return units_data->dst[type];
    return NULL;
}

SS_Metadata* access_units_ss_meta_data()
{
    // return units_data->ss_meta_data;
    return NULL;
}
