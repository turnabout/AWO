#ifdef __EMSCRIPTEN__
#include <stdio.h>
#include <stdarg.h>

#include "Utilities/Message_Service/message_service.h"

void start_message_service()
{
}

void push_msg(char* format, ...)
{
    va_list a_ptr;

    va_start(a_ptr, format);
    printf("%s\n", format, a_ptr);
    va_end(a_ptr);
}

void push_msg_l(Game_Message_Label label, char* format, ...)
{
    va_list a_ptr;

    va_start(a_ptr, format);
    printf("%s\n", format, a_ptr);
    va_end(a_ptr);
}

Game_Message* pop_msg()
{
    return NULL;
}

void end_message_service()
{
}

#endif
