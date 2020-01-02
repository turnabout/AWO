#ifndef __EMSCRIPTEN__
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "Utilities/Linked_List/linked_list.h"
#include "Utilities/Message_Service/message_service.h"

static Linked_List* list = NULL;

void start_message_service()
{
    if (list != NULL) {
        return;
    }

    list = create_linked_list(NULL, 0);
}

void push_msg(Game_Message_Label label, char* format, ...)
{
    if (list == NULL) {
        return;
    }

    Game_Message* message = malloc(sizeof(Game_Message));

    if (message == NULL) {
        return;
    }

    message->label = label;

    // Copy string into message
    int format_len = strlen(format) + 1;
    int copied_len = (format_len < GAME_MSG_MAX_LEN) ? format_len : GAME_MSG_MAX_LEN;

    va_list a_ptr;

    va_start(a_ptr, format);
    vsnprintf(message->str, GAME_MSG_MAX_LEN, format, a_ptr);
    va_end(a_ptr);

    // Store message in message service's list
    append_linked_list_item(list, (void*)message);
}

Game_Message* pop_msg()
{
    if (list == NULL) {
        return NULL;
    }

    Game_Message* message = get_linked_list_nth_element(list, 0);

    if (message != NULL) {
        delete_linked_list_item(list, (void*)message, FALSE);
    }

    return message;
}

void end_message_service()
{
    if (list == NULL) {
        return;
    }

    free_linked_list(list);
}

#endif