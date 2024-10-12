#include "InputHandler.h"
#include <age\data\args.h>
#include <age\input\input.h>

void InputHandler::Install()
{
    if (datArgParser::Get("nojoy"))
    {
        ioInput::bUseJoystick.set(false);
    }
    if (datArgParser::Get("nokb"))
    {
        ioInput::bUseKeyboard.set(false);
    }
}
