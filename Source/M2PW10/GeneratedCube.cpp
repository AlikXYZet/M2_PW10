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
        UpdateLifetime(Message.CurrentLifetime);
}

void AGeneratedCube::CreateAgeThread()
{
    EP_AgeReceiver = FMessageEndpoint::Builder(TEXT("AgeReceiver_AGeneratedCube"))
        .Handling<FCubeLifetime>(this, &AGeneratedCube::BM_AgeHandler);

    if (EP_AgeReceiver.IsValid())
        EP_AgeReceiver->Subscribe<FCubeLifetime>();

    //CreateCubeMessage<FCubeLifetime>(
    //    EP_AgeReceiver,
    //    &AGeneratedCube::BM_AgeHandler,
    //    TEXT("AgeReceiver_AGeneratedCube"));

    CreateCubeThread<FAgeGen_Runnable>(AgeGen_Thread, AgeGen_Class, TEXT("AgeGenThread"));
}

void AGeneratedCube::StopAgeThread()
{
    StopCubeThread(AgeGen_Thread, AgeGen_Class);
    StopCubeMessage(EP_AgeReceiver);
}
//----------------------------------------------------------------------------------------



/* ---   Color   --- */

void AGeneratedCube::SetColor(const FLinearColor iColor)
{
    CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), iColor);

    StopColorThread();
}

void AGeneratedCube::BM_ColorHandler(
    const FCubeColor &Message,
    const TSharedRef<IMessageContext, ESPMode::ThreadSafe> &Context)
{
    if (Message.rCube == this)
        SetColor(FLinearColor(Message.ColorData));
}

void AGeneratedCube::CreateColorThread()
{
    EP_ColorReceiver = FMessageEndpoint::Builder(TEXT("ColorReceiver_AGeneratedCube"))
        .Handling<FCubeColor>(this, &AGeneratedCube::BM_ColorHandler);

    if (EP_ColorReceiver.IsValid())
        EP_ColorReceiver->Subscribe<FCubeColor>();

    //CreateCubeMessage<FCubeColor>(
    //    EP_ColorReceiver,
    //    &AGeneratedCube::BM_ColorHandler,
    //    TEXT("ColorReceiver_AGeneratedCube"));

    CreateCubeThread<FColorGen_Runnable>(ColorGen_Thread, ColorGen_Class, TEXT("ColorGenThread"));
}

void AGeneratedCube::StopColorThread()
{
    StopCubeThread(ColorGen_Thread, ColorGen_Class);
    StopCubeMessage(EP_ColorReceiver);
}
//----------------------------------------------------------------------------------------



/* ---   CubeThread   --- */

template<typename T_Runnable>
void AGeneratedCube::CreateCubeThread(FRunnableThread *irRunnableThread, T_Runnable *irRunnableClass, TCHAR *NameThread)
{
    if (!irRunnableThread)
    {
        if (!irRunnableClass)
            irRunnableClass = new T_Runnable(this);

        irRunnableThread = FRunnableThread::Create(
            irRunnableClass,
            NameThread,
            0,
            EThreadPriority::TPri_Normal);
    }
}

void AGeneratedCube::StopCubeThread(FRunnableThread *irRunnableThread, FRunnable *irRunnableClass)
{
    if (irRunnableThread)
    {
        irRunnableThread->Suspend(false);
        irRunnableThread->Kill(false);

        irRunnableThread = nullptr;

        if (irRunnableClass)
            irRunnableClass = nullptr;
    }
}

void AGeneratedCube::StopAllCubeThread()
{
    StopAgeThread();
    StopColorThread(); // Перестраховка
}
//----------------------------------------------------------------------------------------



/* ---   CubeMessage   --- */
/*
template<typename MT>
void AGeneratedCube::CreateCubeMessage(
    FMessageEndpointPtr iMessageEndpointPtr,
    void (AGeneratedCube:: *HandlerFunc)(const MT &Message, const IMessageContextRef &Context),
    FName NameMessage)
{
    iMessageEndpointPtr = FMessageEndpoint::Builder(NameMessage)
        .Handling<MT>(this, HandlerFunc);

    if (iMessageEndpointPtr.IsValid())
        iMessageEndpointPtr->Subscribe<MT>();
}
*/

void AGeneratedCube::StopCubeMessage(FMessageEndpointPtr &iMessageEndpointPtr)
{
    if (iMessageEndpointPtr.IsValid())
        iMessageEndpointPtr.Reset();
}
//----------------------------------------------------------------------------------------
