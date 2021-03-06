#include <stdlib.h>
#include <cglm/cglm.h>

#include "Config/config.h"
#include "Game_Data/Frame/_frame.h"

Frame* create_frame(cJSON* frame_JSON, int ss_width, int ss_height)
{
    Frame* frame = malloc(sizeof(Frame));
    populate_frame(frame, frame_JSON, (float)ss_width, (float)ss_height);

    return frame;
}

void populate_frame(Frame* frame, cJSON* frame_JSON, float ss_width, float ss_height)
{
    // Gather dimensions
    float x = (float)cJSON_GetObjectItem(frame_JSON, "x")->valueint;
    float y = (float)cJSON_GetObjectItem(frame_JSON, "y")->valueint;

    Uint8 w = (cJSON_HasObjectItem(frame_JSON, "w"))
                ? cJSON_GetObjectItem(frame_JSON, "w")->valueint
                : DEFAULT_ENTITY_SIZE;

    Uint8 h = (cJSON_HasObjectItem(frame_JSON, "h"))
                ? cJSON_GetObjectItem(frame_JSON, "h")->valueint
                : DEFAULT_ENTITY_SIZE;

    frame->width = w;
    frame->height = h;

    vec4 top_left     = { (x    ) / ss_width, (y)     / ss_height, 0.0f, 1.0f };
    vec4 top_right    = { (x + w) / ss_width, (y)     / ss_height, 0.0f, 1.0f };
    vec4 bottom_left  = { (x    ) / ss_width, (y + h) / ss_height, 0.0f, 1.0f };
    vec4 bottom_right = { (x + w) / ss_width, (y + h) / ss_height, 0.0f, 1.0f };

    vec2 raw_top_left = { x, y };

    glm_vec4_copy(top_left, frame->top_left);
    glm_vec4_copy(top_right, frame->top_right);
    glm_vec4_copy(bottom_left, frame->bottom_left);
    glm_vec4_copy(bottom_right, frame->bottom_right);

    frame->raw_top_left[0] = raw_top_left[0];
    frame->raw_top_left[1] = raw_top_left[1];
}

void free_frame(Frame* frame)
{
    free(frame);
}
