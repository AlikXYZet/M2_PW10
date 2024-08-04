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

bool FAgeGen_Runnable::Init()
{
    // PS: Прямой вызов функции, работающей на игровом потоке (GameThread)
    SetAge_GameThread(rGeneratedCube, CurrentLifetime);
    return true;
}

uint32 FAgeGen_Runnable::Run()
{
    while (!bIsStopThread)
    {
        --CurrentLifetime;

        // PS: Прямой вызов функции, работающей на игровом потоке (GameThread)
        UpdateLifetime_GameThread(rGeneratedCube, CurrentLifetime);
        FPlatformProcess::Sleep(1.f);
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

void FAgeGen_Runnable::SetAge_GameThread(AGeneratedCube *irCube, int32 iAge)
{
    AsyncTask(ENamedThreads::GameThread, [irCube, iAge]()
        {
            irCube->SetAge(iAge);
        });
}

void FAgeGen_Runnable::UpdateLifetime_GameThread(AGeneratedCube *irCube, int32 iLifetime)
{
    AsyncTask(ENamedThreads::GameThread, [irCube, iLifetime]()
        {
            irCube->UpdateLifetime(iLifetime);
        });
}
