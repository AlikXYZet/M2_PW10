#pragma once

#include "CoreMinimal.h"

class AGeneratedCube;

class M2PW10_API FColorGen_Runnable : public FRunnable
{
public:

    FColorGen_Runnable(AGeneratedCube *rCube);

    virtual ~FColorGen_Runnable() override;

    /* ---   in FRunnable   --- */

    //virtual bool Init() override;
    virtual uint32 Run() override;
    //virtual void Stop() override;
    virtual void Exit() override;
    //--------------------------------------------

private:

    // Указатель на куб, чьи данные генерируем
    AGeneratedCube *rGeneratedCube;

};
