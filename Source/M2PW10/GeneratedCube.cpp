#include "GeneratedCube.h"

#include "MessageEndpointBuilder.h"

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
    StopAllThread();

    Super::Destroyed();
}

void AGeneratedCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Останов потоков при выходе из активной сессии
    StopAllThread();

    Super::EndPlay(EndPlayReason);
}

void AGeneratedCube::StopAllThread()
{
    StopAgeThread();
    StopColorThread(); // Перестраховка
}
//----------------------------------------------------------------------------------------



/* ---   Age   --- */

void AGeneratedCube::SetAge(const int32 iAge)
{
    Age = iAge;
    TextAge->SetText(FString::Printf(TEXT("%d"), iAge));
    TextLifetime->SetText(FString::Printf(TEXT("%d"), iAge));
}

void AGeneratedCube::UpdateLifetime(const int32 iLifetime)
{
    if (iLifetime >= 0)
    {
        TextLifetime->SetText(FString::Printf(TEXT("%d"), iLifetime));
    }
    else
        this->Destroy();
}

void AGeneratedCube::BM_AgeHandler(
    const FCubeLifetime &Message,
    const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context)
{
    if (Message.rCube == this)
        if (Message.CurrentLifetime < 61)
            UpdateLifetime(Message.CurrentLifetime);
        else
            UE_LOG(LogTemp, Error, TEXT("AGeneratedCube::BM_AgeHandler CurrentLifetime > 60"));
}

void AGeneratedCube::StopAgeThread()
{
    if (AgeGen_Thread)
    {
        // Снятие потока с паузы, если был кем-либо приостановлен
        AgeGen_Thread->Suspend(false);
        // Завершение потока без(!) ожидания
        // PS: С ожиданием возникает баг:
        // * Куб слишком долго уничтожается
        // * Основной поток зависает
        AgeGen_Thread->Kill(false);

        // Очистка указателей
        // PS: Если AgeGen_Thread валиден, то и AgeGen_Class валиден - смотри CreateAgeThread()
        AgeGen_Thread = nullptr;
        AgeGen_Class = nullptr;
    }

    if (EP_AgeReceiver.IsValid())
        EP_AgeReceiver.Reset();
}

void AGeneratedCube::CreateAgeThread()
{
    EP_AgeReceiver = FMessageEndpoint::Builder("AgeReceiver_AGeneratedCube")
        .Handling<FCubeLifetime>(this, &AGeneratedCube::BM_AgeHandler);

    if (EP_AgeReceiver.IsValid())
        EP_AgeReceiver->Subscribe<FCubeLifetime>();

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

void AGeneratedCube::SetColor(const FLinearColor iColor)
{
    CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), iColor);

    StopColorThread();
}

void AGeneratedCube::BM_ColorHandler(const FCubeColor &Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context)
{
    if (Message.rCube == this)
        SetColor(FLinearColor(Message.ColorData));
}

void AGeneratedCube::StopColorThread()
{
    // Аналогичен StopAgeThread()

    if (ColorGen_Thread)
    {
        ColorGen_Thread->Suspend(false);
        ColorGen_Thread->Kill(false);

        ColorGen_Thread = nullptr;
        ColorGen_Class = nullptr;
    }

    if (EP_ColorReceiver.IsValid())
        EP_ColorReceiver.Reset();
}

void AGeneratedCube::CreateColorThread()
{
    // Аналогичен CreateAgeThread()

    EP_ColorReceiver = FMessageEndpoint::Builder("ColorReceiver_AGeneratedCube")
        .Handling<FCubeColor>(this, &AGeneratedCube::BM_ColorHandler);

    if (EP_ColorReceiver.IsValid())
        EP_ColorReceiver->Subscribe<FCubeColor>();

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
