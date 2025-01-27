// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockManager.h"

// Sets default values
AFlockManager::AFlockManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AFlockManager::UpdateCenteringFactor(float value)
{
	this->CenteringFactor = value;
	for (ABoid* boid : Boids)
	{
		boid->CenteringFactor = this->CenteringFactor;
	}

}

void AFlockManager::UpdateAvoidFactor(float value)
{
	this->AvoidFactor = value;
	for (ABoid* boid : Boids)
	{
		boid->AvoidFactor = this->AvoidFactor;
	}
}

void AFlockManager::UpdateMatchingFactor(float value)
{
	this->MatchingFactor = value;
	for (ABoid* boid : Boids)
	{
		boid->MatchingFactor = this->MatchingFactor;
	}
}

void AFlockManager::UpdateVisualRange(float value)
{
	this->VisualRange = value;
	for (ABoid* boid : Boids)
	{
		boid->VisualRange = this->VisualRange;
		//boid->UpdateVisualRangeCollider();
	}
}

void AFlockManager::UpdateAvoidDistance(float value)
{
	this->MinDistance = value;
	for (ABoid* boid : Boids)
	{
		boid->MinDistance = this->MinDistance;
	}
}

void AFlockManager::UpdateMaxSpeed(float value)
{
	this->SpeedLimit = value;
	for (ABoid* boid : Boids)
	{
		boid->SpeedLimit = this->SpeedLimit;
	}

}



// Called when the game starts or when spawned
void AFlockManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnBoids(NumBoids);
	SpawnPredators(NumPredators);
}

void AFlockManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFlockManager::SpawnPredators(int32 NewPredators)
{
	for (int32 i = 0; i < NewPredators; i++)
	{
		SpawnPredator();
	}
}

void AFlockManager::SpawnPredator()
{
	//Get random position in bounds for spawn and random initial velocity
	FVector StartPosition = FMath::RandPointInBox(FBox(WorldBoundsMin, WorldBoundsMax));
	FVector StartingVelocity = FVector(FMath::RandRange(-500.f, 500.f), FMath::RandRange(-500.f, 500.f), 0.f);

	// Spawn the predator actor
	APredator* NewPredator = GetWorld()->SpawnActor<APredator>(PredatorActor, StartPosition, FRotator::ZeroRotator);
	if (NewPredator)
	{
		// Setup new Predator properties
		NewPredator->Position = StartPosition;
		NewPredator->Velocity = StartingVelocity;
		NewPredator->Margin = this->PredatorMargin;
		NewPredator->CenteringFactor = this->PredatorCenteringFactor;
		NewPredator->SpeedLimit = this->PredatorSpeedLimit;
		NewPredator->TurnFactor = this->PredatorTurnFactor;
		NewPredator->VisualRange = this->PredatorVisualRange;
		NewPredator->UpdateVisualRangeCollider();
		NewPredator->EatRange = this->EatRange;
		NewPredator->TimeTilHungry = this->TimeTilHungry;
		NewPredator->SetFlockManager(this);
		// Add the Predator to the Predator array to track and update them later
		Predators.Add(NewPredator);
	}
}

void AFlockManager::SpawnBoids(int32 NewBoids)
{
	for (int32 i = 0; i < NumBoids; i++)
	{
		SpawnBoid();
	}
}

void AFlockManager::SpawnBoid()
{
	//Get random position in bounds for spawn and random initial velocity
	FVector StartPosition = FMath::RandPointInBox(FBox(WorldBoundsMin, WorldBoundsMax));
	FVector StartingVelocity = FVector(FMath::RandRange(-500.f, 500.f), FMath::RandRange(-500.f, 500.f), 0.f);

	// Spawn the boid actor
	ABoid* NewBoid = GetWorld()->SpawnActor<ABoid>(actor, StartPosition, FRotator::ZeroRotator);
	if (NewBoid)
	{
		// Setup new boid properties
		NewBoid->Position = StartPosition;
		NewBoid->Velocity = StartingVelocity;
		NewBoid->Margin = this->Margin;
		NewBoid->MinDistance = this->MinDistance;
		NewBoid->CenteringFactor = this->CenteringFactor;
		NewBoid->AvoidFactor = this->AvoidFactor;
		NewBoid->MatchingFactor = this->MatchingFactor;
		NewBoid->SpeedLimit = this->SpeedLimit;
		NewBoid->TurnFactor = this->TurnFactor;
		NewBoid->FleeFactor = this->FleeFactor;
		NewBoid->VisualRange = this->VisualRange;
		NewBoid->SetFlockManager(this);
		// Add the boid to the Boids array to track boids and update them later
		Boids.Add(NewBoid);
	}
}

