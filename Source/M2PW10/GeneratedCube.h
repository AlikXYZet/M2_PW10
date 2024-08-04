#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TextRenderComponent.h"

#include "Generators/AgeGen_Runnable.h"
#include "Generators/ColorGen_Runnable.h"

#include "GeneratedCube.generated.h"



/*---   Шаблон на проверку типа : true, если "TR" является FAgeGen_Runnable или FColorGen_Runnable    ---*/

template<typename TR>
constexpr bool is_cubeRunnable =
std::is_same<TR, FAgeGen_Runnable>::value
|| std::is_same<TR, FColorGen_Runnable>::value;
//----------------------------------------------------------------------------------------------------



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

private:

    // Возвраст кубика: время жизни в секундах
    int32 Age = 0;

    FAgeGen_Runnable *AgeGen_Class = nullptr;
    FRunnableThread *AgeGen_Thread = nullptr;

    void CreateAgeThread();
    void StopAgeThread();
    //--------------------------------------------



    /* ---   Color   --- */
public:

    //UFUNCTION(BlueprintCallable)
    void SetColor(const FLinearColor iColor);

private:

    FColorGen_Runnable *ColorGen_Class = nullptr;
    FRunnableThread *ColorGen_Thread = nullptr;

    void CreateColorThread();
    void StopColorThread();
    //--------------------------------------------



    /* ---   CubeThread   --- */
    // PS: Используется (int *&) x - ссылка на указатель

    template<typename TR, class = std::enable_if_t<is_cubeRunnable<TR>>>
    void CreateCubeThread(
        FRunnableThread *&irRunnableThread,
        TR *&irRunnable,
        const TCHAR *ThreadName);

    template<typename TR, class = std::enable_if_t<is_cubeRunnable<TR>>>
    void StopCubeThread(
        FRunnableThread *&irRunnableThread,
        TR *&irRunnable);

    // Функция для реализации функций остановки всех потоков в Destroyed() и EndPlay();
    void StopAllCubeThread();
    //--------------------------------------------
};
