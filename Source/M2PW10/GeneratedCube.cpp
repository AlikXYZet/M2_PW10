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

void AGeneratedCube::SetAge(AGeneratedCube *irCube, const int32 iAge)
{
    AsyncTask(ENamedThreads::GameThread, [irCube, iAge]()
        {
            irCube->TextAge->SetText(FString::Printf(TEXT("%d"), iAge));
        });
}

void AGeneratedCube::UpdateLifetime(AGeneratedCube *irCube, const int32 iLifetime)
{
    AsyncTask(ENamedThreads::GameThread, [irCube, iLifetime]()
        {
            if (iLifetime >= 0)
            {
                irCube->TextLifetime->SetText(FString::Printf(TEXT("%d"), iLifetime));
            }
            else
                irCube->Destroy();
        });
}

void AGeneratedCube::StopAgeThread()
{
    if (AgeGen_Thread)
    {
        AgeGen_Thread->Suspend(false);
        AgeGen_Thread->Kill(false);

        AgeGen_Thread = nullptr;
        AgeGen_Class = nullptr;
    }
}

void AGeneratedCube::CreateAgeThread()
{
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
}
//----------------------------------------------------------------------------------------



/* ---   Color   --- */

void AGeneratedCube::SetColor(AGeneratedCube *irCube, const FLinearColor iColor)
{
    AsyncTask(ENamedThreads::GameThread, [irCube, iColor]()
        {
            irCube->CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), iColor);

            irCube->StopColorThread();
        });
}

void AGeneratedCube::StopColorThread()
{
    if (ColorGen_Thread)
    {
        //ColorGen_Thread->Suspend(false);
        ColorGen_Thread->Kill(false);

        ColorGen_Thread = nullptr;
        ColorGen_Class = nullptr;
    }
}

void AGeneratedCube::CreateColorThread()
{
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
}
//----------------------------------------------------------------------------------------
