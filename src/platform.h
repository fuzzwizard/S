#pragma once

#include "alltypes.h"

#if CS_OSX
#include <stdlib.h>
#elif CS_WINDOWS
#include <malloc.h>
#include <windows.h>
#endif