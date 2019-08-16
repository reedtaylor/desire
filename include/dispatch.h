#ifndef _DISPATCH_H
#define _DISPATCH_H

#include <string>

#include "desire_util.h"

int DispatchFromDE(std::string message, DesireState *desire_state);

int DispatchToDE(std::string message, DesireState *desire_state);

#endif
