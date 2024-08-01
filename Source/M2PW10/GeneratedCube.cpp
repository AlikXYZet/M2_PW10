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

}

void AGeneratedCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
//----------------------------------------------------------------------------------------



/* ---   Age   --- */

void AGeneratedCube::SetAge(const int32 iAge)
{
    Age = iAge;
    TextAge->SetText(FString::Printf(TEXT("%d"), iAge));
}

void AGeneratedCube::UpdateLifetime(const int32 iLifetime)
{
    if (iLifetime < 0)
        this->Destroy();
    else
    {
        TextLifetime->SetText(FString::Printf(TEXT("%d"), iLifetime));
        UpdateLifetime_BP(iLifetime);
    }
}

void AGeneratedCube::UpdateLifetime_BP_Implementation(const int32 iLifetime)
{
    // in BP
}
//----------------------------------------------------------------------------------------



/* ---   Color   --- */

void AGeneratedCube::SetColor(const FLinearColor iColor)
{
    CubeMesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue(TEXT("CubeColor"), iColor);
}
//----------------------------------------------------------------------------------------
