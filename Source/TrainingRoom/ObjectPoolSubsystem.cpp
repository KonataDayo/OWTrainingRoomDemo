#include "ObjectPoolSubsystem.h"
#include "ObjectPool.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/Actor.h"

AActor* UObjectPoolSubsystem::GetActor(TSubclassOf<AActor> Class)
{
	if (!Class)
	{
		UE_LOG(LogTemp,Error,TEXT("[UObjectPoolSubsystem::Get()] Param: Class NULL"));
		return nullptr;
	}
	if (!Pools.Contains(Class))
	{
		// add new categories
		auto NewObjectPool = NewObject<UObjectPool>(this);
		NewObjectPool->Initialize(GetWorld(),Class);
		Pools.Add(Class,NewObjectPool);
	}
	auto ObjPool = Pools[Class];
	AActor* Obj = ObjPool->GetFromPool();
	return Obj;
}

bool UObjectPoolSubsystem::Return(AActor* ReturningObj)
{
	if (!ReturningObj)
	{
		UE_LOG(LogTemp,Error,TEXT("[ObjectPoolSubsystem] (FUNC)Return: ReturningObj or Class null"));
		return false;
	}
	if (isActivated(ReturningObj))
	{
		UObjectPool* Pool = Pools[ReturningObj->GetClass()];
		Pool->ReturnToPool(ReturningObj);
		return true;
	}
	return false;
}

bool UObjectPoolSubsystem::isActivated(AActor* Obj)
{
	if (!Obj) 
	{
#if WITH_EDITOR
		UE_LOG(LogTemp,Error,TEXT("[UObjectPoolSubsystem] (FUNC)isActivated Obj param is null"));
#endif
		return false;
	}
	UClass* ClassType = Obj->GetClass();
	if (!Pools.Contains(ClassType))
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("[UObjectPoolSubsystem] (FUNC)isActivated Obj doesn't belong to any of the pools"));
#endif
		return false;
	}
	return !Pools[ClassType]->ContainsInactiveObject(Obj);
}
