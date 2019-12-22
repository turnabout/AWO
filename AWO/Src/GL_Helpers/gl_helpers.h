#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Creates a shader_program program, using the given paths to the vertex and fragment shaders.
// Returns the shader_program program ID or 0 if unsuccessful.
GLuint create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path);

/*! @brief Creates a texture object from an image loaded at the given path.
 *
 *  @param[in] path The full path to the texture.
 *  @param[out] width The width of the loaded texture.
 *  @param[out] height The height of the loaded texture.
 *  @return The texture object ID or 0 if an error occurred.
 */
GLuint create_texture_object(const char* path, int* width, int* height);

/*! @brief Creates an empty texture object.
 *
 *  @param[out] width The width given to the empty texture.
 *  @param[out] height The height given to the empty texture.
 *  @return The texture object ID or 0 if an error occurred.
 */
GLuint create_empty_texture_object(int width, int height);

/*! @brief Reads the data from the given texture.
 *
 *  @param[in] texture Texture to read the data from.
 *  @param[out] buffer Buffer the data is read into.
 *  @param[in] width Width of the texture.
 *  @param[in] height Height of the texture.
 */
void read_texture_data(GLuint texture, void* buffer, int width, int height);