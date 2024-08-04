#include "AgeGen_Runnable.h"

#include "M2PW10/GeneratedCube.h"
#include "M2PW10/Tools/MyRandom.h"

FAgeGen_Runnable::FAgeGen_Runnable(AGeneratedCube *rCube)
{
    rGeneratedCube = rCube;
    CurrentLifetime = GetRandomAge();
}

FAgeGen_Runnable::~FAgeGen_Runnable()
{
}

uint32 FAgeGen_Runnable::Run()
{
    while (!rGeneratedCube->rAgeGen_Event) {}

    rGeneratedCube->rAgeGen_Event->Wait();
    rGeneratedCube->Age = CurrentLifetime;
    rGeneratedCube->Lifetime = CurrentLifetime;
    rGeneratedCube->rAgeGen_Event->Trigger();

    while (!bIsStopThread)
    {
        --CurrentLifetime;

        if (rGeneratedCube->rAgeGen_Event)
        {
            rGeneratedCube->rAgeGen_Event->Wait();
            rGeneratedCube->Lifetime = CurrentLifetime;
            rGeneratedCube->rAgeGen_Event->Trigger();
        }
    }

    return 1;
}

void FAgeGen_Runnable::Stop()
{
    bIsStopThread = true;
}

void FAgeGen_Runnable::Exit()
{
    rGeneratedCube = nullptr;
}
