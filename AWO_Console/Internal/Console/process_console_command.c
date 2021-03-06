#include <stdio.h>

#include "Console/_console.h"

int process_console_command(Console* console)
{
    // Exit early if user command is empty
    if (console->user_command_char_count == 0) {
        return CMD_Ret_Error;
    }

    // Translate the user-entered command string into usable chunks
    char* chunks[1 + CMD_ARG_MAX_COUNT];
    int chunk_count = 0;
    int next_chunk_start = 0;

    // Initialize all possible chunks to NULL
    for (int i = 0; i <= CMD_ARG_MAX_COUNT; i++) {
        chunks[i] = NULL;
    }

    for (int i = 0; i <= COMMAND_MAX_LENGTH; i++) {
        if (console->user_command[i] == '\0') {

            if (i > 0) {
                chunks[chunk_count++] = &(console->user_command[next_chunk_start]);
            }

            break;
        }

        if (console->user_command[i] == ' ') {
            chunks[chunk_count++] = &(console->user_command[next_chunk_start]);
            next_chunk_start = i + 1;
            console->user_command[i] = '\0';
        }
    }
    
    // Error processing chunks
    if (chunks[0] == NULL) {
        add_console_message(console, COLOR_PAIR_ERROR, "Error processing command");
        return CMD_Ret_Error;
    }

    // Attempt to get the command
    Command* command = get_command_by_name(console->command_list, chunks[0]);

    // Error: Command not found
    if (command == NULL) {
        add_console_message(
            console, 
            COLOR_PAIR_ERROR, 
            "Error: command '%s' does not exist", 
            chunks[0]
        );

        reset_console_user_command(console);
        return CMD_Ret_Error;
    }

    // Confirm the command's arguments
    int user_arg_count = chunk_count - 1;
    int command_arg_count = get_command_arg_count(command);

    // Error: Not enough arguments
    if (user_arg_count < command_arg_count) {
        add_console_message(
            console, 
            COLOR_PAIR_ERROR, 
            "Error: expected %d arguments, got %d", 
            command_arg_count,
            user_arg_count
        );

        reset_console_user_command(console);
        return CMD_Ret_Error;
    }

    // Attempt to load the arguments into the command
    char* command_args_raw[CMD_ARG_MAX_COUNT];

    for (int i = 0; i < command_arg_count; i++) {
        command_args_raw[i] = chunks[1 + i];
    }

    if (!load_command_arguments(command, command_args_raw)) {
        add_console_message(
            console, 
            COLOR_PAIR_ERROR, 
            "Error: incorrect argument types given"
        );
    }

    return execute_command_function(command, (void*)console);
}
