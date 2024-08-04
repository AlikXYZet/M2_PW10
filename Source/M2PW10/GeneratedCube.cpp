#include "GeneratedCube.h"

AGeneratedCube::AGeneratedCube()
{
    // Вызывать Tick() каждый такт?
    PrimaryActorTick.bCanEverTick = false;  // предварительно
    //PrimaryActorTick.TickInterval = 1;    // 1 = 1 sec

    /* ---   CubeMesh   --- */
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
    CubeMesh->SetCanEverAffectNavigation(false);

    RootComponent = CubeMesh;
    //--------------------------------------------



    /* ---   Text   --- */
    TextAge = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Age"));
    TextAge->SetupAttachment(RootComponent);
    TextAge->SetTextRenderColor(FColor::Red);
    TextAge->SetRelativeLocation(FVector(60.f, 30.f, 10.f));

    TextLifetime = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Lifetime"));
    TextLifetime->SetupAttachment(RootComponent);
    TextLifetime->SetTextRenderColor(FColor::Red);
    TextLifetime->SetRelativeLocation(FVector(60.f, 30.f, -40.f));
    //--------------------------------------------
}

void AGeneratedCube::BeginPlay()
{
    Super::BeginPlay();

    // Создание потоков генерации данных
    CreateAgeThread();
    CreateColorThread();
}

void AGeneratedCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGeneratedCube::Destroyed()
{
    // Останов потоков при уничтожении актора
    StopAllCubeThread();

    Super::Destroyed();
}

void AGeneratedCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Останов потоков при выходе из активной сессии
    StopAllCubeThread();

    Super::EndPlay(EndPlayReason);
}
//----------------------------------------------------------------------------------------



/* ---   Age   --- */

void AGeneratedCube::SetAge(const int32 &iAge)
{
    TextAge->SetText(FString::Printf(TEXT("%d"), iAge));
}

void AGeneratedCube::UpdateLifetime(const int32 &iLifetime)
{
    if (iLifetime >= 0)
    {
        TextLifetime->SetText(FString::Printf(TEXT("%d"), iLifetime));
    }
    else
        Destroy();
}

void AGeneratedCube::CreateAgeThread()
{
    CreateCubeThread<FAgeGen_Runnable>(
        AgeGen_Thread,
        AgeGen_Class,
        TEXT("AgeGenThread"));
}

void AGeneratedCube::StopAgeThread()
{
    StopCubeThread<FAgeGen_Runnable>(
        AgeGen_Thread,
        AgeGen_Class);
}
//----------------------------------------------------------------------------------------



/* ---   Color   --- */

void AGeneratedCube::SetColor(const FLinearColor &iColor)
{
    CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), iColor);

    StopColorThread();
}

void AGeneratedCube::CreateColorThread()
{
    CreateCubeThread<FColorGen_Runnable>(
        ColorGen_Thread,
        ColorGen_Class,
        TEXT("ColorGenThread"));
}

void AGeneratedCube::StopColorThread()
{
    StopCubeThread<FColorGen_Runnable>(
        ColorGen_Thread,
        ColorGen_Class);
}
//----------------------------------------------------------------------------------------



/* ---   CubeThread   --- */

template<typename TR, class>
void AGeneratedCube::CreateCubeThread(
    FRunnableThread *&irRunnableThread,
    TR *&irRunnable,
    const TCHAR *ThreadName)
{
    if (!irRunnableThread)
    {
        if (!irRunnable)
            irRunnable = new TR(this);

        irRunnableThread = FRunnableThread::Create(
            irRunnable,
            ThreadName,
            0,
            EThreadPriority::TPri_Normal);
    }
}

template<typename TR, class>
inline void AGeneratedCube::StopCubeThread(
    FRunnableThread *&irRunnableThread,
    TR *&irRunnable)
{
    if (irRunnableThread)
    {
        irRunnableThread->Suspend(false);
        irRunnableThread->Kill(false);

        irRunnableThread = nullptr;
        irRunnable = nullptr;
    }
}

void AGeneratedCube::StopAllCubeThread()
{
    StopAgeThread();
    StopColorThread(); // Перестраховка
}
//----------------------------------------------------------------------------------------