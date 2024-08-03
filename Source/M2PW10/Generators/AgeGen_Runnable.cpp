#include "AgeGen_Runnable.h"

#include "M2PW10/GeneratedCube.h"
#include "M2PW10/Tools/MyRandom.h"

FAgeGen_Runnable::FAgeGen_Runnable(AGeneratedCube *rCube)
{
    //UE_LOG(LogTemp, Warning, TEXT("FAgeGen_Runnable::FAgeGen_Runnable"));
    rGeneratedCube = rCube;
    CurrentLifetime = GetRandomAge();
}

FAgeGen_Runnable::~FAgeGen_Runnable()
{
}

bool FAgeGen_Runnable::Init()
{
    //UE_LOG(LogTemp, Warning, TEXT("FAgeGen_Runnable::Init()"));
    rGeneratedCube->SetAge(CurrentLifetime);
    return true;
}

uint32 FAgeGen_Runnable::Run()
{
    //UE_LOG(LogTemp, Warning, TEXT("FAgeGen_Runnable::Run()"));

    while (!bIsStopThread)
    {
        --CurrentLifetime;

        //if (CurrentLifetime < 0)
        //{
        //    if (rGeneratedCube)
        //        rGeneratedCube->Destroy();
        //}
        //else
        //{
        rGeneratedCube->UpdateLifetime(CurrentLifetime);
        FPlatformProcess::Sleep(1.f);
        //}

        // PS: Ошибка при завершении сессии сразу после уничтожения Куба
    }

    //UE_LOG(LogTemp, Warning, TEXT("FAgeGen_Runnable::Run() - completed"));

    return 1;
}

void FAgeGen_Runnable::Stop()
{
    // Выход из цикла Run()
    // PS: Из-за вызова задержки ::Sleep(1.f) приходится ожидать его завершения...
    // Требуется организовать быстрый выход из цикла:
    // *? через пустой цикл while(bTime) с выходом по таймеру и вызове Stop() ???
    bIsStopThread = true;

    //UE_LOG(LogTemp, Warning, TEXT("FAgeGen_Runnable::Stop()"));
}

void FAgeGen_Runnable::Exit()
{
    // Очистка указаателей
    // PS: Требуется анализ необходимости очистки (ускорение кода на несколько тактов),
    // ибо после вызова Exit() данный класс вроде-как уничтожается, уничтожая за собой указатели
    rGeneratedCube = nullptr;

    //UE_LOG(LogTemp, Warning, TEXT("FAgeGen_Runnable::Exit()"));
}
