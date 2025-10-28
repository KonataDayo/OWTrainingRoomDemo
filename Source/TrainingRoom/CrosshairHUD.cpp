// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Texture2D.h"

void ACrosshairHUD::DrawHUD()
{
	Super::DrawHUD();
	if (CrosshairTexture)
	{
		FVector2D Pos;
		FVector2D Size(16,16);
		GEngine->GameViewport->GetViewportSize(Pos);
		FVector2D CrosshairLoc = 0.5f * Pos - (Size * 0.5f);
		FCanvasTileItem MyCrosshair(CrosshairLoc,CrosshairTexture->Resource,Size,FLinearColor::White);
		MyCrosshair.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(MyCrosshair);
	}
}
