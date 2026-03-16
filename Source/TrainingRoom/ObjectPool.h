#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "ObjectPool.generated.h"

UCLASS()
class TRAININGROOM_API UObjectPool : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> InactiveObjects;

	UPROPERTY(EditAnywhere)
	int32 MaxCapacity = 100;

	UPROPERTY()
	TSubclassOf<AActor> Type;

	AActor* SpawnObject(UWorld* WorldContext, UClass* ClassType);

public:
	void Initialize(UWorld* WorldContext,TSubclassOf<AActor> Class);

	AActor* GetFromPool();

	bool ReturnToPool(AActor* ReturningObj);

	bool ContainsInactiveObject(AActor* Obj);
};
