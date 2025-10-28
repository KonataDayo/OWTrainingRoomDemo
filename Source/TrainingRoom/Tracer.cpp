// Fill out your copyright notice in the Description page of Project Settings.


#include "Tracer.h"
#include "TracerRecall.h"
#include "SkillComponent.h"
#include "DrawDebugHelpers.h"

void ATracer::UpdateRecallInfo(float DeltaTime)
{
	if (!GetSkillComponent() || !GetSkillComponent()->Skills.Find(ESkillSlot::ESS_Primary))
	{
		UE_LOG(LogTemp,Warning,TEXT("Cannot find RECALL!"));
		return;
	}
	UTracerRecall* RecallComp = Cast<UTracerRecall>(GetSkillComponent()->Skills[ESkillSlot::ESS_Primary]);
	if (!RecallComp) return;
	TimeSinceLastSampled += DeltaTime;
	if (TimeSinceLastSampled >= SampleInterval)
	{
		FRecallInfo thisInfo;
		thisInfo.Location = GetActorLocation();
		thisInfo.Rotation = GetActorRotation();
		thisInfo.HP = GetCurrentHP();

		if (RecallComp->RecallInfos.Num() >= MAX_INFO_SIZE) RecallComp->RecallInfos.RemoveAt(0);
		RecallComp->RecallInfos.Add(thisInfo);
		// UE_LOG(LogTemp,Warning,TEXT("LOCATION: %f,%f,%f"),thisInfo.Location.X,thisInfo.Location.Y,thisInfo.Location.Z);

		// debug spheres
		DrawDebugSphere(
			GetWorld(),
			thisInfo.Location,
			10.f,
			6,
			FColor::Green,
			false,
			3.f,
			0,
			1.f
		);
		TimeSinceLastSampled = 0.f;
	}
}

void ATracer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateRecallInfo(DeltaSeconds);
}

void ATracer::ResetRecallTimer()
{
	RecallTimer = 0.f;
}

ATracer::ATracer()
{
	MAX_INFO_SIZE = 3.f / SampleInterval;
}
