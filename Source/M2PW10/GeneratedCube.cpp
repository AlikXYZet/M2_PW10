#include "GeneratedCube.h"

AGeneratedCube::AGeneratedCube()
{
    // Вызывать Tick() каждый такт?
    PrimaryActorTick.bCanEverTick = true; // предварительно
    PrimaryActorTick.TickInterval = 1.f;    // 1.0f = 1 sec

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

    // Для памятки
    //UE_LOG(LogTemp, Warning, TEXT("AGeneratedCube::AGeneratedCube"));
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

    UpdateLifetime();
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

void AGeneratedCube::UpdateAge()
{
    TextAge->SetText(FString::Printf(TEXT("%d"), Age));
}

void AGeneratedCube::UpdateLifetime()
{
    rAgeGen_Event->Trigger();
    rAgeGen_Event->Wait();

    if (Lifetime >= 0)
    {
        TextLifetime->SetText(FString::Printf(TEXT("%d"), Lifetime));
    }
    else
        this->Destroy();
}

void AGeneratedCube::CreateAgeThread()
{
    CreateCubeThread<FAgeGen_Runnable>(
        AgeGen_Thread,
        AgeGen_Class,
        rAgeGen_Event,
        &AGeneratedCube::UpdateAge);
}


void AGeneratedCube::StopAgeThread()
{
    StopCubeThread<FAgeGen_Runnable>(
        AgeGen_Thread,
        AgeGen_Class,
        rAgeGen_Event);
}
//----------------------------------------------------------------------------------------



/* ---   Color   --- */

void AGeneratedCube::UpdateColor()
{
    CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), NewColor);

    StopColorThread();
}

void AGeneratedCube::CreateColorThread()
{
    CreateCubeThread<FColorGen_Runnable>(
        ColorGen_Thread,
        ColorGen_Class,
        rColorGen_Event,
        &AGeneratedCube::UpdateColor);
}

void AGeneratedCube::StopColorThread()
{
    StopCubeThread<FColorGen_Runnable>(
        ColorGen_Thread,
        ColorGen_Class,
        rColorGen_Event);
}
//----------------------------------------------------------------------------------------



/* ---   CubeThread   --- */

template<typename TR, class>
void AGeneratedCube::CreateCubeThread(
    FRunnableThread *&irRunnableThread,
    TR *&irRunnable,
    FEvent *&irEvent,
    void (AGeneratedCube:: *Function)(void))
{
    irEvent = FPlatformProcess::GetSynchEventFromPool();

    if (!irRunnableThread)
    {
        if (!irRunnable)
            irRunnable = new TR(this);

        irRunnableThread = FRunnableThread::Create(
            irRunnable,
            TEXT("ColorGenThread"),
            0,
            EThreadPriority::TPri_Normal);
    }

    while (!irEvent) {}
    irEvent->Trigger();
    irEvent->Wait();
    (this->*Function)();
}

template<typename TR, class>
void AGeneratedCube::StopCubeThread(
    FRunnableThread *&irRunnableThread,
    TR *&irRunnable,
    FEvent *&irEvent)
{
    if (irRunnableThread)
    {
        irRunnableThread->Suspend(false);
        irEvent->Trigger();
        irRunnableThread->Kill(true);

        irRunnableThread = nullptr;
        irRunnable = nullptr;
    }

    if (irEvent)
    {
        irEvent->Trigger();
        FPlatformProcess::ReturnSynchEventToPool(irEvent);
        irEvent = nullptr;
    }
}

void AGeneratedCube::StopAllCubeThread()
{
    StopAgeThread();
    StopColorThread(); // Перестраховка
}
//----------------------------------------------------------------------------------------
