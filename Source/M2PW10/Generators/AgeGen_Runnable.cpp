#include "AgeGen_Runnable.h"
#include "MessageEndpointBuilder.h"

#include "M2PW10/GeneratedCube.h"
#include "M2PW10/Tools/MyRandom.h"

FAgeGen_Runnable::FAgeGen_Runnable(AGeneratedCube *rCube)
{
    rGeneratedCube = rCube;
    CurrentLifetime = GetRandomAge();

    EP_LifetimeSender = FMessageEndpoint::Builder("Sender_AgeGen_Runnable").Build();;
}

FAgeGen_Runnable::~FAgeGen_Runnable()
{
}

bool FAgeGen_Runnable::Init()
{
    rGeneratedCube->SetAge(CurrentLifetime);
    return true;
}

uint32 FAgeGen_Runnable::Run()
{
    while (!bIsStopThread)
    {
        --CurrentLifetime;

        if (EP_LifetimeSender.IsValid())
            EP_LifetimeSender->Publish<FCubeLifetime>(new FCubeLifetime(rGeneratedCube, CurrentLifetime));

        FPlatformProcess::Sleep(1.f);
    }

    return 1;
}

void FAgeGen_Runnable::Stop()
{
    // Выход из цикла Run()
    // PS: Из-за вызова задержки ::Sleep(1.f) приходится ожидать его завершения...
    // Требуется организовать быстрый выход из цикла:
    // *? через пустой цикл while(bTime) с выходом по таймеру и вызове Stop() ???
    // *? использование стороннего таймера ???
    bIsStopThread = true;
}

void FAgeGen_Runnable::Exit()
{
    if (EP_LifetimeSender.IsValid())
        EP_LifetimeSender.Reset();

    rGeneratedCube = nullptr;
}
