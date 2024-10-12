#include "CustomExceptionHandler.h"
#include <age\data\stack.h>

void CustomExceptionHandler::Install()
{
    InstallCallback("ExceptMain", "Hook except main for better exception handling.",
        &datStack::ExceptionFilterCombined, {
            cb::call(0x618232),
        }
    );
}
