#pragma once

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef struct { int32 low; int32 high; } int64;
typedef struct { uint32 low; uint32 high; } uint64;

static_assert(sizeof(int64) == 8 && sizeof(uint64) == 8);
