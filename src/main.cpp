#include "tracy/Tracy.hpp"

#include "motor/unicamotor.h"

void* operator new(std::size_t size)
{
    void* pointer = malloc(size);
    TracyAlloc(pointer, size);
    return pointer;
}

void operator delete(void* ptr) noexcept
{
    TracyFree(ptr);
    free(ptr);
}

int main(int argc, char* argv[])
{
    Unicamotor engine;
    while (!engine.WasExitRequested())
    {
        FrameMarkNamed("EngineLoop");
    }

    return 0;
}
