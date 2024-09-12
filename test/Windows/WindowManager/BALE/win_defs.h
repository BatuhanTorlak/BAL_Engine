#ifndef WIN_DEFS_H
#define WIN_DEFS_H

#define BAL_MALLOC(__size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, __size)
#define BAL_MALLOC_0(__size) HeapAlloc(GetProcessHeap(), 0, __size)
#define BAL_FREE(__ptr) HeapFree(GetProcessHeap(), 0, __ptr)
#define BAL_REALLOC(__ptr, __size) HeapReAlloc(GetProcessHeap(), 0, __ptr, __size)
#define BAL_MEMSET(__mem, __value, __size) RtlFillMemory(__mem, __value, __size)
#define BAL_MEMCPY(__source, __dest, __size) CopyDeviceMemory(__dest, __source, __size)
#define BAL_MEMMOV(__source, __dest, __size) RtlMoveMemory(__dest, __source, __size)

#endif