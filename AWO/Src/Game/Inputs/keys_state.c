#include <stdlib.h>

#include "Game/Inputs/inputs.h"

// Every GLFW key definition corresponding to the game keys
static const int GAME_KEYS[] = { 
    GLFW_KEY_SPACE,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_W,
    GLFW_KEY_A,
    GLFW_KEY_S,
    GLFW_KEY_D,
    GLFW_KEY_F,
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
};

static const int GAME_KEYS_COUNT = sizeof(GAME_KEYS) / sizeof(int);

// Reference to the game's window
static GLFWwindow* window_instance;

// Holds the current state of every key
static Button_State* keys_state;

void update_keys_state()
{
    for (int i = 0; i < GAME_KEYS_COUNT; i++) {
        Button_State previous_state = keys_state[i];
        int current_down_state = glfwGetKey(window_instance, GAME_KEYS[i]);

        keys_state[i] = glfwGetKey(window_instance, GAME_KEYS[i]) | 
                        ((previous_state & 1) ^ current_down_state) << 1;
    }
}

void init_keys_state_module(GLFWwindow* window)
{
    window_instance = window;

    keys_state = (Button_State*)malloc(sizeof(Button_State) * GAME_KEYS_COUNT);

    for (int i = 0; i < GAME_KEYS_COUNT; i++) {
        keys_state[i] = 0;
    }
}

Button_State get_key_state(Key key)
{
    return keys_state[key];
}

void free_keys_state_module()
{
    free(keys_state);
}