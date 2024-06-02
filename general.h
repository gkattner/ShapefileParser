#pragma once

#include <iostream>

//////////////// macros //////////////////////////
#define RETURN_ON_ERROR(func) \
Return_t retval = func; \
if (func != eReturnGlobalOk) \
    return retval;\

//////////////// typedefs ////////////////////////
typedef uint16_t Return_t;
typedef uint32_t Parts_t;

//////////////// defines /////////////////////////
uint32_t little2native32(uint32_t in)
{
    uint32_t out = 0x00000000;
    out += (in & 0x000000FF) << 24;
    out += (in & 0x0000FF00) << 8;
    out += (in & 0x00FF0000) >> 8;
    out += (in & 0xFF000000) >> 24;

    return out;
}

