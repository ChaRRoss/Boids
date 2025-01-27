// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Predator.generated.h"

UCLASS()
class BOIDS_API APredator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator")
		class USphereComponent* DetectionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator")
		class UStaticMeshComponent* VisualMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator")
		FVector Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator")
		FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator")
		FVector WorldBoundsMin = FVector(-250, -250, 0); // Min world bounds for predator position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator")
		FVector WorldBoundsMax = FVector(250, 250, 0);  // Max world bounds for predator position

	//Predator Control Parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float VisualRange = 32.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float SpeedLimit = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float CenteringFactor = .3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float TurnFactor = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float Margin = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float EatRange = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float TimeTilHungry = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		float TimeToStarve = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Control")
		class UNiagaraSystem* BloodEffect;

	
	TArray<AActor*> NearbyBoids;
	FTimerHandle HungerTimer;
	FTimerDelegate HungerDelegate;
	FTimerHandle StarvationTimer;
	bool bHungry = true;

	//Predator Algorithm Methods
	void CalculateChaseCohesion(TArray<AActor*>& DetectedBoids);
	void UpdateNearbyBoids();
	void LimitSpeed();
	void KeepWithinBounds();
	void UpdateVisualRangeCollider();
	void EatBoid(TArray<AActor*>& boids);
	UFUNCTION()
	void UpdateHunger(bool bIsHungry);
	void Starve();
	void SetFlockManager(AActor* Manager);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class AFlockManager* FlockManager;

};
