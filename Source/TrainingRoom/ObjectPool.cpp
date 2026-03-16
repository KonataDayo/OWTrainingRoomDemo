#include "ObjectPool.h"
#include "Poolable.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void UObjectPool::Initialize(UWorld* WorldContext,TSubclassOf<AActor> Class)
{
	Type = Class;
	// Create inactive objects
	for (int32 i = 0; i < MaxCapacity; i++)
	{
		auto TempObj = SpawnObject(WorldContext,Class);
		InactiveObjects.Add(TempObj);
	}
#if WITH_EDITOR
	UE_LOG(LogTemp,Display,TEXT("[%s] Initialization Succeeded, Inactive Objects: %d"),*GetName(),InactiveObjects.Num());
#endif
}

AActor* UObjectPool::SpawnObject(UWorld* WorldContext, UClass* ClassType)
{
	if (!WorldContext || !ClassType) return nullptr;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return WorldContext->SpawnActor<AActor>(ClassType,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParams);
}

AActor* UObjectPool::GetFromPool()
{
	if (InactiveObjects.Num() == 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("[%s] ObjectPool doesn't have inactive object now"),*GetName());
		// Dynamically adjusting capacity
		UWorld* World = GetWorld();
		if (!World)
		{
			UE_LOG(LogTemp,Error,TEXT("[%s] Failed To GetWorld, Failed to adjust capacity, now returning NULLPTR"),*GetName());
			return nullptr;
		}
		int32 Addition = MaxCapacity / 2;
		for (int32 i = 0; i < Addition; i++)
		{
			auto TempObj = SpawnObject(World,Type);
			InactiveObjects.Add(TempObj);
		}
	}
	// Get Actor From InactiveObjectStack
	AActor* Obj = InactiveObjects.Last();
	InactiveObjects.Pop();
	// Activate
	if (Obj->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
	{
		IPoolable::Execute_OnActivate(Obj);
	}
	return Obj;
}

bool UObjectPool::ReturnToPool(AActor* ReturningObj)
{
	if (!ReturningObj) return false;
	// Deactivate
	if (ReturningObj->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
	{
		IPoolable::Execute_OnDeactivate(ReturningObj);
	}
	// return to pool
	InactiveObjects.Add(ReturningObj);
	return true;
}

bool UObjectPool::ContainsInactiveObject(AActor* Obj)
{
	if (!Obj) return false;
	return InactiveObjects.Contains(Obj);
}

