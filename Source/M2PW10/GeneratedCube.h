#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TextRenderComponent.h"

#include "Generators/AgeGen_Runnable.h"
#include "Generators/ColorGen_Runnable.h"

#include "GeneratedCube.generated.h"



/* ---   Struct for IMessage   --- */

// Общая структура фильтрации
USTRUCT(BlueprintType)
struct FCubeData
{
    GENERATED_BODY()

    // Указатель на Куб (необходим для фильтрации)
    AGeneratedCube *rCube = nullptr;

    FCubeData(AGeneratedCube *iCube = nullptr) : rCube(iCube) {}
};

// Структура данных для FAgeGen_Runnable
USTRUCT(BlueprintType)
struct FCubeLifetime : public FCubeData
{
    GENERATED_BODY()

    // Данные (атомарность/потокобезопасность не обязательна)
    //std::atomic_int32_t CurrentLifetime = 0;
    int32 CurrentLifetime = 0;

    FCubeLifetime(AGeneratedCube *iCube = nullptr, int32 iLifetime = 0)
        : FCubeData(iCube), CurrentLifetime(iLifetime) {};
};

// Структура данных для FColorGen_Runnable
USTRUCT(BlueprintType)
struct FCubeColor : public FCubeData
{
    GENERATED_BODY()

    // Данные
    FLinearColor ColorData = FLinearColor::Black;

    FCubeColor(AGeneratedCube *iCube = nullptr, FLinearColor iColor = FLinearColor::Black)
        : FCubeData(iCube), ColorData(iColor) {};
};

// PS: Дабы избежать вызова "Message" всех дочерних структур от FCubeData,
// требуется прописывать "USTRUCT(BlueprintType)" и "GENERATED_BODY()"
// во всех дочерних структурах
//----------------------------------------------------------------------------------------



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

    // Функция для реализации функций остановки всех потоков в Destroyed() и EndPlay();
    void StopAllThread();
    //--------------------------------------------



    /* ---   Age   --- */
public:

    //UFUNCTION(BlueprintCallable)  // for tests
    void SetAge(const int32 iAge);

    void UpdateLifetime(const int32 iLifetime);

private:

    // "Получатель" данных
    TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> EP_AgeReceiver;

    // "Обрабодчик" данных
    void BM_AgeHandler(
        const struct FCubeLifetime &Message,
        const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context);

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
