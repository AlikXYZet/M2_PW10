#pragma once

#include "CoreMinimal.h"

class AGeneratedCube;

class M2PW10_API FColorGen_Runnable : public FRunnable
{
public:

    FColorGen_Runnable(AGeneratedCube *rCube, FEvent **orrEvent);

    virtual ~FColorGen_Runnable() override;

    /* ---   in FRunnable   --- */

    //virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;
    //--------------------------------------------

private:

    // Указатель на куб, чьи данные генерируем
    AGeneratedCube *rGeneratedCube;

    // Контроль работы потока
    // PS: Из-за частого чтения, желательно сделать потокобезопасным
    FThreadSafeBool bIsStopThread = false;

    // Здешний Event
    FEvent *rLocalEvent = nullptr;
};
