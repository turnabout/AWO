#include <stdlib.h>

#include "Utilities/macros.h"
#include "GL_Helpers/gl_helpers.h"
#include "Game/Renderer/Extras_Renderer/extras_renderer.h"
#include "Game/Renderer/Sprite_Batch/sprite_batch.h"

// Identity matrix reference
mat4 identity = GLM_MAT4_IDENTITY_INIT;

struct Extras_Renderer {

    // Shader program used to render extras.
    GLuint shader;

    // Sprite batch used to render extras.
    Sprite_Batch* sprite_batch;

    // Whether an extra has been queued for rendering.
    Bool extra_queued;

};

Extras_Renderer* create_extras_renderer(GLuint sprite_sheet)
{
    Extras_Renderer* renderer = malloc(sizeof(Extras_Renderer));

    renderer->extra_queued = FALSE;
    
    // Create the shader program
    renderer->shader = create_shader_program(
        VERTEX_SHADER_PATH(BASIC_SHADER),
        FRAGMENT_SHADER_PATH(BASIC_SHADER)
    );

    renderer->sprite_batch = create_sprite_batch(
        renderer->shader,
        sprite_sheet,
        0,
        10 // TODO: un-hardcode
    );

    return renderer;
}

void update_extras_renderer_matrices(Extras_Renderer* renderer, mat4 matrix)
{
}

void queue_extra_item_render(Extras_Renderer* renderer, vec2 dst, Frame* frame)
{
    if (!renderer->extra_queued) {
        renderer->extra_queued = TRUE;
        begin_sprite_batch(renderer->sprite_batch);
    }

    add_to_sprite_batch(renderer->sprite_batch, dst, frame, 0);
}

void render_queued_extra_items(Extras_Renderer* renderer)
{
    if (renderer->extra_queued) {
        end_sprite_batch(renderer->sprite_batch);
        renderer->extra_queued = FALSE;
    }
}

void free_extras_renderer(Extras_Renderer* renderer)
{
    if (renderer != NULL) {
        free(renderer);
    }
}