// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "FlockManager.h"
#include <Kismet/GameplayStatics.h>

void UBoidWidget::MenuSetup()
{
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* PlayerController = world->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	SetFlockManager();
	
	if (FlockManager)
	{
		CohesionSlider->Value = FlockManager->CenteringFactor;
		SeparationSlider->Value = FlockManager->AvoidFactor;
		AlignmentSlider->Value = FlockManager->MatchingFactor;
		VisualRangeSlider->Value = FlockManager->VisualRange;
		AvoidRangeSlider->Value = FlockManager->MinDistance;
		MaxSpeedSlider->Value = FlockManager->SpeedLimit;
	}

}

bool UBoidWidget::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (CohesionSlider)
		CohesionSlider->OnValueChanged.AddDynamic(this, &ThisClass::CohesionSliderChanged);
	if (SeparationSlider)
		SeparationSlider->OnValueChanged.AddDynamic(this, &ThisClass::SeparationSliderChanged);
	if (AlignmentSlider)
		AlignmentSlider->OnValueChanged.AddDynamic(this, &ThisClass::AlignmentSliderChanged);
	if (VisualRangeSlider)
		VisualRangeSlider->OnValueChanged.AddDynamic(this, &ThisClass::VisualRangeSliderChanged);
	if (AvoidRangeSlider)
		AvoidRangeSlider->OnValueChanged.AddDynamic(this, &ThisClass::AvoidRangeSliderChanged);
	if (MaxSpeedSlider)
		MaxSpeedSlider->OnValueChanged.AddDynamic(this, &ThisClass::MaxSpeedSliderChanged);
	if (BoidButton)
		BoidButton->OnClicked.AddDynamic(this, &ThisClass::SpawnBoid);
	if (PredatorButton)
		PredatorButton->OnClicked.AddDynamic(this, &ThisClass::SpawnPredator);
	MenuSetup();

	return true;
}

void UBoidWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBoidWidget::CohesionSliderChanged(float value)
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->UpdateCenteringFactor(value);
}

void UBoidWidget::SeparationSliderChanged(float value)
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->UpdateAvoidFactor(value);
}

void UBoidWidget::AlignmentSliderChanged(float value)
{
	if (!FlockManager)
		SetFlockManager();
	if(FlockManager)
		FlockManager->UpdateMatchingFactor(value);
}

void UBoidWidget::VisualRangeSliderChanged(float value)
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->UpdateVisualRange(value);
}

void UBoidWidget::AvoidRangeSliderChanged(float value)
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->UpdateAvoidDistance(value);
}

void UBoidWidget::MaxSpeedSliderChanged(float value)
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->UpdateMaxSpeed(value);
}

void UBoidWidget::SpawnBoid()
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->SpawnBoid();
}

void UBoidWidget::SpawnPredator()
{
	if (!FlockManager)
		SetFlockManager();
	if (FlockManager)
		FlockManager->SpawnPredator();
}

void UBoidWidget::SetFlockManager()
{
	FlockManager = Cast<AFlockManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlockManager::StaticClass()));
}
