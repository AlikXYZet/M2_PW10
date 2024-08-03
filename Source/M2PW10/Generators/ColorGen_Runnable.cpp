#include "ColorGen_Runnable.h"

#include "M2PW10/GeneratedCube.h"
#include "M2PW10/Tools/MyRandom.h"

FColorGen_Runnable::FColorGen_Runnable(AGeneratedCube *rCube, FEvent **orrEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("FColorGen_Runnable::FColorGen_Runnable"));
    rLocalEvent = FPlatformProcess::GetSynchEventFromPool();
    *orrEvent = rLocalEvent;
    rGeneratedCube = rCube;
}

FColorGen_Runnable::~FColorGen_Runnable()
{
}

uint32 FColorGen_Runnable::Run()
{
    UE_LOG(LogTemp, Warning, TEXT("FColorGen_Runnable::Run()"));
    // PS: При прямом вызове функции:
    // срабатывает, но возникает вылет при завершении сессии ("UE4Editor-Core.pdb не загружен")
    // Решение(?): Записать значение от GetRandomColor() в локальную переменную куба
    // и вызвать SetColor(.) через FEvent (???)
    //rGeneratedCube->SetColor(GetRandomColor());
    
    // Изменение данных Куба
    rGeneratedCube->NewColor = GetRandomColor();

    // Вызов изменение цвета
    rLocalEvent->Trigger();
    // PS: Требуется проверить следующее: (результат: +/- , не проверено: *)
    // 
    // + Срабатывание триггера до начала ожидания (указатель rLocalEvent уничтожается)
    // Результат: Основной поток отрабатывает
    // 
    // - Работа основного потока при задержке впомогательного (данного) потока
    // Результат: Основной поток останавливается и ожидает триггера
    // PS: Необходимо найти вариант по типу делегата, но локально между Кубом и его FColorGen_Runnable
    // (пересмотреть вариант с IMessage ???)

    //while (!bIsStopThread) {}
    UE_LOG(LogTemp, Warning, TEXT("FColorGen_Runnable::Run() - completed"));
    return 0;
}

void FColorGen_Runnable::Stop()
{
    FPlatformProcess::ReturnSynchEventToPool(rLocalEvent);
    bIsStopThread = true;

    UE_LOG(LogTemp, Warning, TEXT("FColorGen_Runnable::Stop()"));
}

void FColorGen_Runnable::Exit()
{
    rGeneratedCube = nullptr;
    rLocalEvent = nullptr;
    UE_LOG(LogTemp, Warning, TEXT("FColorGen_Runnable::Exit()"));
}
