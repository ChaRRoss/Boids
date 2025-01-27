// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoidWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOIDS_API UBoidWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void MenuSetup();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AFlockManager* FlockManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USlider* CohesionSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* SeparationSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* AlignmentSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* VisualRangeSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* AvoidRangeSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* MaxSpeedSlider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* BoidButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* PredatorButton;
protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:


	UFUNCTION()
		void CohesionSliderChanged(float value);
	UFUNCTION()
		void SeparationSliderChanged(float value);
	UFUNCTION()
		void AlignmentSliderChanged(float value);
	UFUNCTION()
		void VisualRangeSliderChanged(float value);
	UFUNCTION()
		void AvoidRangeSliderChanged(float value);
	UFUNCTION()
		void MaxSpeedSliderChanged(float value);
	UFUNCTION()
		void SpawnBoid();
	UFUNCTION()
		void SpawnPredator();
	void SetFlockManager();
	
};
