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
    StopAgeThread();
    StopColorThread(); // Перестраховка

    Super::Destroyed();
}

void AGeneratedCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Останов потоков при выходе из активной сессии
    StopAgeThread();
    StopColorThread(); // Перестраховка

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
    rAgeGen_Event = FPlatformProcess::GetSynchEventFromPool();

    if (!AgeGen_Thread)
    {
        if (!AgeGen_Class)
            AgeGen_Class = new FAgeGen_Runnable(this);

        AgeGen_Thread = FRunnableThread::Create(
            AgeGen_Class,
            TEXT("AgeGenThread"),
            0,
            EThreadPriority::TPri_Normal);
    }

    while (!rAgeGen_Event) {}
    rAgeGen_Event->Trigger();
    rAgeGen_Event->Wait();
    UpdateAge();
}


void AGeneratedCube::StopAgeThread()
{
    if (AgeGen_Thread)
    {
        // Снятие потока с паузы, если был кем-либо приостановлен
        AgeGen_Thread->Suspend(false);

        rAgeGen_Event->Trigger();
        AgeGen_Thread->Kill(true);

        // Очистка указателей
        // PS: Если AgeGen_Thread валиден, то и AgeGen_Class валиден - смотри CreateAgeThread()
        AgeGen_Thread = nullptr;
        AgeGen_Class = nullptr;
    }

    if (rAgeGen_Event)
    {
        rAgeGen_Event->Trigger();
        FPlatformProcess::ReturnSynchEventToPool(rAgeGen_Event);
        rAgeGen_Event = nullptr;
    }
}
//----------------------------------------------------------------------------------------



/* ---   Color   --- */

void AGeneratedCube::UpdateColor()
{
    CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), NewColor);
}

void AGeneratedCube::CreateColorThread()
{
    rColorGen_Event = FPlatformProcess::GetSynchEventFromPool();

    if (!ColorGen_Thread)
    {
        if (!ColorGen_Class)
            ColorGen_Class = new FColorGen_Runnable(this);

        ColorGen_Thread = FRunnableThread::Create(
            ColorGen_Class,
            TEXT("ColorGenThread"),
            0,
            EThreadPriority::TPri_Normal);
    }

    while (!rColorGen_Event) {}
    rColorGen_Event->Trigger();
    rColorGen_Event->Wait();
    UpdateColor();
}

void AGeneratedCube::StopColorThread()
{
    if (ColorGen_Thread && rColorGen_Event)
    {
        ColorGen_Thread->Suspend(false);
        rColorGen_Event->Trigger();
        ColorGen_Thread->Kill(true);

        ColorGen_Thread = nullptr;
        ColorGen_Class = nullptr;
    }

    if (rColorGen_Event)
    {
        rColorGen_Event->Trigger();
        FPlatformProcess::ReturnSynchEventToPool(rColorGen_Event);
        rColorGen_Event = nullptr;
    }
}
//----------------------------------------------------------------------------------------
