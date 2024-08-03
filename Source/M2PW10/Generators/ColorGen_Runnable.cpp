#include "ColorGen_Runnable.h"
#include "MessageEndpointBuilder.h"

#include "M2PW10/GeneratedCube.h"
#include "M2PW10/Tools/MyRandom.h"

FColorGen_Runnable::FColorGen_Runnable(AGeneratedCube *rCube)
{
    rGeneratedCube = rCube;

    EP_ColorSender = FMessageEndpoint::Builder("Sender_ColorGen_Runnable").Build();
}

FColorGen_Runnable::~FColorGen_Runnable()
{
}

uint32 FColorGen_Runnable::Run()
{
    // PS: При прямом вызове функции:
    // срабатывает, но возникает вылет при завершении сессии ("UE4Editor-Core.pdb не загружен")
    // Решение(?): Использовать IMessageBus для передачи данных с вызовом функции через делегат
    //rGeneratedCube->SetColor(GetRandomColor());

    // Отправка данных
    // PS: Требуется отслеживание Куба при работе с несколькими Кубами
    if (EP_ColorSender.IsValid())
        EP_ColorSender->Publish<FCubeColor>(new FCubeColor(rGeneratedCube, GetRandomColor()));

    return 1;
}

void FColorGen_Runnable::Exit()
{
    if (EP_ColorSender.IsValid())
        EP_ColorSender.Reset();

    rGeneratedCube = nullptr;
}
