#include "MatchOneEnttPlayerController.h"
#include "Components/UserInputComponent.h"
#include "Components/BurstModeComponent.h"
#include "Components/AssetLibraryComponent.h"
#include "Components/WorldComponent.h"
#include "Components/ViewContainerComponent.h"
#include "Game/Actors/GamePiece.h"
#include "Game/Actors/ViewContainer.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

AMatchOneEnttPlayerController::AMatchOneEnttPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    entt::ServiceLocator<entt::DefaultRegistry>::set();

    Systems = std::make_unique<GameSystems>();
}

void AMatchOneEnttPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &AMatchOneEnttPlayerController::OnClick);
        InputComponent->BindAction("B", IE_Pressed, this, &AMatchOneEnttPlayerController::OnPressedB);
    }
}

void AMatchOneEnttPlayerController::BeginPlay()
{
    Super::BeginPlay();

    entt::DefaultRegistry &Registry = entt::ServiceLocator<entt::DefaultRegistry>::ref();

    Systems->Initialize(Registry);

    auto ViewContainer = GetWorld()->SpawnActor<AViewContainer>(AViewContainer::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    Registry.attach<ViewContainerComponent>(Registry.create(), ViewContainer);
    Registry.attach<WorldComponent>(Registry.create(), GetWorld());
}

void AMatchOneEnttPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    entt::DefaultRegistry &Registry = entt::ServiceLocator<entt::DefaultRegistry>::ref();

    if (IsInputKeyDown(EKeys::LeftMouseButton))
    {
        if (Registry.has<BurstModeComponent>())
        {
            CheckHit();
        }
    }

    Systems->Update(Registry);
}

void AMatchOneEnttPlayerController::BeginDestroy()
{
    Super::BeginDestroy();

    entt::ServiceLocator<entt::DefaultRegistry>::reset();
}

void AMatchOneEnttPlayerController::SetAssetLibrary(UAssetLibrary* AssetLibrary)
{
    entt::DefaultRegistry &Registry = entt::ServiceLocator<entt::DefaultRegistry>::ref();
    Registry.attach<AssetLibraryComponent>(Registry.create(), AssetLibrary);
}

void AMatchOneEnttPlayerController::OnClick()
{
    CheckHit();
}

void AMatchOneEnttPlayerController::OnPressedB()
{
    entt::DefaultRegistry &Registry = entt::ServiceLocator<entt::DefaultRegistry>::ref();

    if (Registry.has<BurstModeComponent>())
    {
        Registry.remove<BurstModeComponent>();
    }
    else
    {
        Registry.attach<BurstModeComponent>(Registry.create());
    }
}

void AMatchOneEnttPlayerController::CheckHit()
{
    float LocationX;
    float LocationY;

    GetMousePosition(LocationX, LocationY);

    FVector2D MousePosition(LocationX, LocationY);
    FHitResult HitResult;
    const bool TraceComplex = false;

    if (GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, TraceComplex, HitResult) == true)
    {
        auto GamePiece = Cast<AGamePiece>(HitResult.GetActor());

        if (GamePiece != nullptr)
        {
            auto Location = GamePiece->GetActorLocation();

            entt::DefaultRegistry &Registry = entt::ServiceLocator<entt::DefaultRegistry>::ref();
            Registry.create(UserInputComponent{ int(Location.Y / 125.0f), int(Location.Z / 125.0f) });
        }
    }
}
