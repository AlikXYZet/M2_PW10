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
    // PS: Прямой вызов функции, работающей на игровом потоке (GameThread)
    // PPS: Расчёт данных происходит в этом потоке, а результат копируется в игровой поток
    SetColor_GameThread(rGeneratedCube, GetRandomColor());

    return 1;
}

void FColorGen_Runnable::Exit()
{
    rGeneratedCube = nullptr;
}

void FColorGen_Runnable::SetColor_GameThread(AGeneratedCube *irCube, const FLinearColor &iColor)
{
    // В лямбду-функцию необходимо передавать не ссылку, а копию переменных 
    // для избежания ошибки доступа к памяти другим потоком
    AsyncTask(ENamedThreads::GameThread, [irCube, iColor]()
        {
            irCube->SetColor(iColor);
        });
}
