#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TextRenderComponent.h"

#include "Generators/AgeGen_Runnable.h"
#include "M2PW10/Tools/MyRandom.h"

#include "GeneratedCube.generated.h"

UCLASS()
class M2PW10_API AGeneratedCube : public AActor
{
    GENERATED_BODY()

public:
    /* ---   Base   --- */

    AGeneratedCube();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    UStaticMeshComponent *CubeMesh = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    UTextRenderComponent *TextAge;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
    UTextRenderComponent *TextLifetime;
    //--------------------------------------------

protected:

    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;

    virtual void Destroyed() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //--------------------------------------------



    /* ---   Age   --- */
public:

    //UFUNCTION(BlueprintCallable)  // for tests
    void SetAge(const int32 iAge);

    void UpdateLifetime(const int32 iLifetime);
    UFUNCTION(BlueprintNativeEvent)
    void UpdateLifetime_BP(const int32 Lifetime);

private:

    // Возвраст кубика: время жизни в секундах
    int32 Age = 0;

    FAgeGen_Runnable *AgeGen_Class = nullptr;
    FRunnableThread *AgeGen_Thread = nullptr;

    void StopAgeThread();
    void CreateAgeThread();
    //--------------------------------------------



    /* ---   Color   --- */
public:

    void ColorGeneration(UStaticMeshComponent *irMesh);

private:

    //--------------------------------------------
};
