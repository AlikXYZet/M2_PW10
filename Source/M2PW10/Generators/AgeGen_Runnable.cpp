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
    rGeneratedCube->SetAge(CurrentLifetime);
    return true;
}

uint32 FAgeGen_Runnable::Run()
{
    while (!bIsStopThread)
    {
        --CurrentLifetime;

        if (CurrentLifetime < 0)
            rGeneratedCube->Destroy();
        else
        {
            rGeneratedCube->UpdateLifetime(CurrentLifetime);
            FPlatformProcess::Sleep(1.f);
        }
    }

    return 1;
}

void FAgeGen_Runnable::Stop()
{
    // Выход из цикла Run()
    // PS: Из-за вызова задержки ::Sleep(1.f) приходится ожидать его завершения...
    // Требуется организовать быстрый выход из цикла:
    // *? через пустой цикл while(bTime) с выходом по таймеру и вызове Stop() ???
    bIsStopThread = true;
}

void FAgeGen_Runnable::Exit()
{
    // Очистка указаателей
    // PS: Требуется анализ необходимости очистки (ускорение кода на несколько тактов),
    // ибо после вызова Exit() данный класс вроде-как уничтожается, уничтожая за собой указатели
    rGeneratedCube = nullptr;
}
