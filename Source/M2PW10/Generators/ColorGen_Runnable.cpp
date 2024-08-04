#include "ColorGen_Runnable.h"

#include "M2PW10/GeneratedCube.h"
#include "M2PW10/Tools/MyRandom.h"

FColorGen_Runnable::FColorGen_Runnable(AGeneratedCube *rCube)
{
    rGeneratedCube = rCube;
}

FColorGen_Runnable::~FColorGen_Runnable()
{
}

uint32 FColorGen_Runnable::Run()
{
    while (!rGeneratedCube->rColorGen_Event) {}

    if (rGeneratedCube->rColorGen_Event)
    {
        rGeneratedCube->rColorGen_Event->Wait();
        rGeneratedCube->NewColor = GetRandomColor();
        rGeneratedCube->rColorGen_Event->Trigger();
    }

    return 0;
}

void FColorGen_Runnable::Exit()
{
    rGeneratedCube = nullptr;
}
