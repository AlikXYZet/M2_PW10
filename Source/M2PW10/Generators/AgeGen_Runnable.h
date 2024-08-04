#pragma once

#include "CoreMinimal.h"

class AGeneratedCube;

class M2PW10_API FAgeGen_Runnable : public FRunnable
{
public:

    FAgeGen_Runnable(AGeneratedCube *rCube);

    virtual ~FAgeGen_Runnable() override;

    /* ---   in FRunnable   --- */

    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;
    //--------------------------------------------

private:

    // Указатель на куб, чьи данные генерируем
    // PS: Нет смысла в проверке на валидность, так как поток создаётся в BeginPlay() данного Куба,
    // а уничтожается в перед уничтожением данного Куба. Т.е., всегда валиден в данном потоке
    AGeneratedCube *rGeneratedCube;

    // Контроль работы потока (потокабезопасна - нужна ли в данном случае ???)
    //FThreadSafeBool bIsStopThread = false;
    bool bIsStopThread = false;

    // Переменная отсчёта времени жизни
    int32 CurrentLifetime = 0;

    // Сохранение данных возроста Куба
    // (Функция игрового потока [GameThread])
    void SetAge_GameThread(AGeneratedCube *irCube, int32 iAge);

    // Контроль времени жизни Куба
    // (Функция игрового потока [GameThread])
    void UpdateLifetime_GameThread(AGeneratedCube *irCube, int32 iLifetime);
};
