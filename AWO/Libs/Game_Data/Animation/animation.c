#include <stdio.h>
#include <stdlib.h>

#include "Game_Data/Animation/animation.h"
#include "Game_Data/Frame/_frame.h"

Animation* create_animation(const cJSON* animation_JSON, int ss_width, int ss_height)
{
    Animation* animation = malloc(sizeof(Animation));

    animation->count = cJSON_GetArraySize(animation_JSON);
    Frame* frames = (Frame*)malloc( sizeof(Frame) * animation->count );

    // Gather data from every frame and populate the allocated frames
    cJSON* frame_JSON = NULL;
    cJSON_ArrayForEach(frame_JSON, animation_JSON)
    {
        populate_frame(frames, frame_JSON, (float)ss_width, (float)ss_height);
        frames++;
    }

    animation->frames = frames - animation->count;
    return animation;

}

void free_animation(Animation* animation)
{
    if (animation != NULL) {
        if (animation->frames != NULL) {
            free(animation->frames);
        }

        free(animation);
    }
}

#ifdef _DEBUG
void print_animation(Animation* animation)
{
    printf("\nFrames [\n");

    for (int i = 0; i < animation->count; i++) {
        printf("\t{\n");
        printf("\tTop left\n\t");
        glm_vec3_print(animation->frames[i].top_left, stdout);

        printf("\tTop right\n\t");
        glm_vec3_print(animation->frames[i].top_right, stdout);

        printf("\tBottom left\n\t");
        glm_vec3_print(animation->frames[i].bottom_left, stdout);

        printf("\tBottom right\n\t");
        glm_vec3_print(animation->frames[i].bottom_right, stdout);

        printf(
            "\t(Raw top left): (%d, %d)\n", 
            (int)animation->frames[i].raw_top_left[0],
            (int)animation->frames[i].raw_top_left[1]
        );
        
        printf(
            "\t(Dimensions): %d | %d\n", 
            animation->frames->width, 
            animation->frames->height
        );

        printf("\t},\n");
    }

    printf("]\n");
}
#endif
