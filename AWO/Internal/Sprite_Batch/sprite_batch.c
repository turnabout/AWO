#include <stdlib.h>

#include "Sprite_Batch/sprite_batch.h"

/* How many elements are included in a single vertex:
 * - Destination x & y
 * - Texture x & y
 * - Palette index
 */
#define VERTEX_FLOAT_COUNT 5

// How many vertices make up a full quad (4)
#define QUAD_VERTICES_AMOUNT 4

/*! @brief Sprite batch object. Describes a sprite batch.
 */
struct Sprite_Batch {

    // The VAO generated and used by this sprite batch.
    GLuint VAO, VBO, EBO;

    // Shader program used by this sprite batch.
    GLuint shader_program;

    // The sprite sheet and palette textures.
    GLuint sprite_sheet_texture, palette_texture;

    // Amount of elements currently queued and max amount
    int elements_queued, elements_max;

    // Offset taken up by the sprite batch's elements
    size_t current_offset;

};

void init_sprite_batch_data(Sprite_Batch* sprite_batch)
{
    // VAO
    glGenVertexArrays(1, &sprite_batch->VAO);
    glBindVertexArray(sprite_batch->VAO);

    // VBO
    glGenBuffers(1, &sprite_batch->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, sprite_batch->VBO);

    // Calculate stride - how many bytes we need to use to jump from one vertex to another
    size_t stride = VERTEX_FLOAT_COUNT * sizeof(GLfloat);

    // Reserve VBO memory (one 2D quad's data requires 4 vertices)
    size_t quad_size = stride * QUAD_VERTICES_AMOUNT;
    glBufferData(GL_ARRAY_BUFFER, quad_size * sprite_batch->elements_max, NULL, GL_DYNAMIC_DRAW);

    // EBO
    GLuint indices_base[] = {
        0, 1, 2, // First triangle
        2, 1, 3, // Second triangle
    };

    size_t indices_size = (sizeof(GLuint) * 6) * sprite_batch->elements_max;
    GLuint* indices = malloc(indices_size);

    for (int i = 0; i < sprite_batch->elements_max; i++) {
        int start_index = i * 6;

        indices[start_index + 0] = indices_base[0] + (4 * i);
        indices[start_index + 1] = indices_base[1] + (4 * i);
        indices[start_index + 2] = indices_base[2] + (4 * i);
        indices[start_index + 3] = indices_base[3] + (4 * i);
        indices[start_index + 4] = indices_base[4] + (4 * i);
        indices[start_index + 5] = indices_base[5] + (4 * i);
    }

    glGenBuffers(1, &sprite_batch->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_batch->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    free(indices);

    // Vertex attribute for destination position
    glVertexAttribPointer(
        0, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)(0 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(0);

    // Vertex attribute for texture position
    glVertexAttribPointer(
        1, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)(2 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    // Vertex attribute for palette index
    glVertexAttribPointer(
        2, 
        1, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)(4 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Sprite_Batch* create_sprite_batch(
    GLuint shader_program, 
    GLuint sprite_sheet_texture, 
    GLuint palette_texture, 
    int max_elements
)
{
    Sprite_Batch* sprite_batch = malloc(sizeof(Sprite_Batch));

    if (sprite_batch == NULL) {
        return NULL;
    }

    sprite_batch->shader_program = shader_program;
    sprite_batch->sprite_sheet_texture = sprite_sheet_texture;
    sprite_batch->palette_texture = palette_texture;
    sprite_batch->elements_max = max_elements;
    sprite_batch->elements_queued = 0;

    init_sprite_batch_data(sprite_batch);

    return sprite_batch;
}

void begin_sprite_batch(Sprite_Batch* sprite_batch)
{
    // Set active sprite sheet texture/shader_program program/VAO
    glUseProgram(sprite_batch->shader_program);

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, sprite_batch->sprite_sheet_texture);

    if (sprite_batch->palette_texture) {
        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, sprite_batch->palette_texture);
    }

    glBindVertexArray(sprite_batch->VAO);

    // Reset counters
    sprite_batch->current_offset = 0;
    sprite_batch->elements_queued = 0;
}

void add_to_sprite_batch(
    Sprite_Batch* sprite_batch, 
    vec2 dst, 
    Frame* frame, 
    GLfloat palette_index
)
{
    // Set the quad's vertices data
    GLfloat vertices[QUAD_VERTICES_AMOUNT][VERTEX_FLOAT_COUNT] = {

        // Top left
        { 
            dst[0], dst[1],                         // <vec2> Destination
            frame->top_left[0], frame->top_left[1], // <vec2> Texture
            palette_index                           // <float> Palette index
        },     

        // Top right
        {
            dst[0] + (float)frame->width, dst[1],
            frame->top_right[0], frame->top_right[1],
            palette_index
        },

        // Bottom left
        {
            dst[0], dst[1] + (float)frame->height, 
            frame->bottom_left[0],frame->bottom_left[1],
            palette_index
        },

        // Bottom right
        {
            dst[0] + (float)frame->width, dst[1] + (float)frame->height, 
            frame->bottom_right[0], frame->bottom_right[1],
            palette_index
        }

    };

    // Store vertices data in previously allocated buffer
    glBufferSubData(GL_ARRAY_BUFFER, sprite_batch->current_offset, sizeof(vertices), vertices);

    // Update sprite batch state
    sprite_batch->elements_queued++;
    sprite_batch->current_offset += sizeof(vertices);
}

void end_sprite_batch(Sprite_Batch* sprite_batch)
{
    glDrawElements(GL_TRIANGLES, 6 * sprite_batch->elements_queued, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
    glBindVertexArray(0);
}

void free_sprite_batch(Sprite_Batch* sprite_batch)
{
    glUseProgram(sprite_batch->shader_program);
    glBindVertexArray(sprite_batch->VAO);

    // Free buffers & VAO
    glDeleteBuffers(1, &sprite_batch->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &sprite_batch->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteVertexArrays(1, &sprite_batch->VAO);

    free(sprite_batch);
}

void add_to_sprite_batch__test_palette(Sprite_Batch* sprite_batch)
{
    #define LEFT   0
    #define RIGHT  300
    #define BOTTOM 100
    #define TOP    0

    GLfloat quad_vertices[QUAD_VERTICES_AMOUNT][VERTEX_FLOAT_COUNT] = {
        {LEFT, BOTTOM, 0.0, 0.0, 0.0}, // Bot left
        {RIGHT, BOTTOM, 1.0, 0.0, 0.0}, // Bot right
        {LEFT, TOP, 0.0, 1.0, 0.0}, // Top left
        {RIGHT, TOP, 1.0, 1.0, 0.0}, // Top right
    };

    // Store vertices data in previously allocated buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_vertices), quad_vertices);

    // Update sprite batch state
    sprite_batch->elements_queued++;
    sprite_batch->current_offset += sizeof(quad_vertices);
}