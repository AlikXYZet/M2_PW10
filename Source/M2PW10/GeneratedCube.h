#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TextRenderComponent.h"

#include "Generators/AgeGen_Runnable.h"
#include "Generators/ColorGen_Runnable.h"

#include "GeneratedCube.generated.h"

USTRUCT(BlueprintType)
struct FCubeColor
{
    GENERATED_BODY()

    FLinearColor ColorData = FLinearColor::Black;

    FCubeColor(FLinearColor iColor = FLinearColor::Black)
        : ColorData(iColor) {};
};



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

    //UFUNCTION(BlueprintCallable)
    void SetColor(const FLinearColor iColor);

private:

    // "Получатель" данных
    TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> EP_ColorReceiver;

    // "Обрабодчик" данных
    void BM_ColorHandler(
        const struct FCubeColor &Message,
        const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context);

    FColorGen_Runnable *ColorGen_Class = nullptr;
    FRunnableThread *ColorGen_Thread = nullptr;

    void StopColorThread();
    void CreateColorThread();
    //--------------------------------------------
};
