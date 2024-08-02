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
    // PS: При прямом вызове функции:
    // срабатывает, но возникает вылет при завершении сессии ("UE4Editor-Core.pdb не загружен")
    // Решение(?): Записать значение от GetRandomColor() в локальную переменную куба
    // и вызвать функцию/делегат (???)
    //rGeneratedCube->SetColor(GetRandomColor());

    rGeneratedCube->NewColor = GetRandomColor();
    //rGeneratedCube->UpdateColor();

    return 0;
}

void FColorGen_Runnable::Exit()
{
    rGeneratedCube = nullptr;
}
