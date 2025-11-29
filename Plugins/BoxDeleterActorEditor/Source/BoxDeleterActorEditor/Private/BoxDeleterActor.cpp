#include "BoxDeleterActor.h"

#if WITH_EDITOR
#include "EngineUtils.h"
#include"Editor.h"
#include"ScopedTransaction.h"
#include "Components/BoxComponent.h"
#endif

ABoxDeleterActor::ABoxDeleterActor()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
}
#if WITH_EDITOR
void ABoxDeleterActor::DeleteActorsInBox()
{
    if (!GetWorld()) return;

    const FScopedTransaction Transaction(FText::FromString(TEXT("Delete Actors In Box")));
    Modify();

    FBox Box = BoxComponent->Bounds.GetBox();

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor == this) continue;

        // Check ExcludedClasses
        bool bMatchesExcludedClass = false;
        for (TSubclassOf<AActor> ExcludedClass : FilteredClasses)
        {
            if (Actor->IsA(ExcludedClass))
            {
                bMatchesExcludedClass = true;
                break;
            }
        }
        if (bMatchesExcludedClass) continue;

        // Check Excluded Actors
        if (FilteredActors.Contains(Actor)) continue;
        // Check Z Exclude
        if (bEnableZFilter && Actor->GetActorLocation().Z <= MinZ) continue;

        // Delete Inside of the Box
        if (Box.IsInsideOrOn(Actor->GetActorLocation()))
        {
            Actor->Modify();
            Actor->Destroy();
            UE_LOG(LogTemp, Warning, TEXT("Deleted Actor: %s"), *Actor->GetName());
        }
    }
}
#endif