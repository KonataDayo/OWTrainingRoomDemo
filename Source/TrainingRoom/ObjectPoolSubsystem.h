#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

UCLASS()
class TRAININGROOM_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<UClass*,class UObjectPool*> Pools;

	AActor* GetActor(TSubclassOf<AActor> Class);

public:

	template<typename T>
	T* Get(TSubclassOf<AActor> Class)
	{
		return Cast<T>(GetActor(Class));
	}

	bool Return(AActor* ReturningObj);

	bool isActivated(AActor* Obj);
};


