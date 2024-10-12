#include "BorderlessHandler.h"
#include <age\data\args.h>

void BorderlessHandler::Install()
{
    if (datArgParser::Get("borderless")) 
    {
        // write to border bool address
        mem::write(0x674FA8, static_cast<uint8_t>(0));
    }
}