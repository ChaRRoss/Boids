// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "Predator.h"
#include "GameFramework/Actor.h"
#include "FlockManager.generated.h"

UCLASS()
class BOIDS_API AFlockManager : public AActor
{
	GENERATED_BODY()
	
public:
	AFlockManager();
	UFUNCTION(BlueprintCallable)
	void SpawnBoids(int32 NewBoids);
	void SpawnBoid();
	UFUNCTION(BlueprintCallable)
	void SpawnPredators(int32 NewPredators);
	void SpawnPredator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		TArray<ABoid*> Boids;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		TSubclassOf<ABoid> actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		TArray<APredator*> Predators;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		TSubclassOf<APredator> PredatorActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		int32 NumBoids = 100; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		int32 NumPredators = 2; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		FVector WorldBoundsMin = FVector(-250, -250, 0); // Min world bounds for initial boid position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Level Settings")
		FVector WorldBoundsMax = FVector(500, 500, 0);  // Max world bounds for initial boid position

	//Baseline property values for the swarm calculations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float AvoidFactor = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float MatchingFactor = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float CenteringFactor = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float VisualRange = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float MinDistance = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float SpeedLimit = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float TurnFactor = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float FleeFactor = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid Controls")
		float Margin = 100.0f;

	//Baseline property values for the Predator calculations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float PredatorCenteringFactor = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float PredatorVisualRange = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float PredatorSpeedLimit = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float PredatorTurnFactor = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float PredatorMargin = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float EatRange = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float TimeTilHungry = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Predator Controls")
		float TimeToStarve = 10.0f;



	void UpdateCenteringFactor(float value);
	void UpdateAvoidFactor(float value);
	void UpdateMatchingFactor(float value);
	void UpdateVisualRange(float value);
	void UpdateAvoidDistance(float value);
	void UpdateMaxSpeed(float value);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
