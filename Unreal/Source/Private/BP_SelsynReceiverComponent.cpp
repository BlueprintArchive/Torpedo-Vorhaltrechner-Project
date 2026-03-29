#include "BP_SelsynReceiverComponent.h"

#include "Components/SceneComponent.h"
#include "TDC.h"

UBP_SelsynReceiverComponent::UBP_SelsynReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TDCRef = nullptr;
	DrivenComponent = nullptr;
	SignalType = ESelsynSignalType::GyroAngle;
	DriveAxis = ESelsynDriveAxis::Yaw;
	DisplayedAngle = 0.0f;
	MaxSlewDegPerSec = 90.0f;
	DeadbandDeg = 0.25f;
	CalibrationOffsetDeg = 0.0f;
	bPowered = true;
	bInvertDirection = false;
	BaseRelativeRotation = FRotator::ZeroRotator;
}

void UBP_SelsynReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheBaseRelativeRotation();
	SyncToCommandAngle();
}

void UBP_SelsynReceiverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bPowered || !IsValid(TDCRef) || !IsValid(ResolveDrivenComponent()))
	{
		return;
	}

	const float CommandAngle = NormalizeAngleDegrees(GetCommandAngle() + CalibrationOffsetDeg);
	const float DeltaDegrees = FMath::FindDeltaAngleDegrees(DisplayedAngle, CommandAngle);

	if (FMath::Abs(DeltaDegrees) <= DeadbandDeg)
	{
		return;
	}

	const float MaxStep = FMath::Max(MaxSlewDegPerSec, 0.0f) * FMath::Max(DeltaTime, 0.0f);
	const float Step = FMath::Clamp(DeltaDegrees, -MaxStep, MaxStep);
	DisplayedAngle = NormalizeAngleDegrees(DisplayedAngle + Step);
	ApplyDisplayedAngle();
}

float UBP_SelsynReceiverComponent::GetCommandAngle() const
{
	if (!IsValid(TDCRef))
	{
		return 0.0f;
	}

	switch (SignalType)
	{
	case ESelsynSignalType::GyroAngle:
		return TDCRef->GyroAngle;

	case ESelsynSignalType::TargetBearing:
		return TDCRef->TargetBearing;

	case ESelsynSignalType::AngleOnTheBow:
		return TDCRef->AngleOnTheBow;

	case ESelsynSignalType::ParallaxCorrection:
		return TDCRef->ParallaxCorrection;

	default:
		return 0.0f;
	}
}

void UBP_SelsynReceiverComponent::SyncToCommandAngle()
{
	DisplayedAngle = NormalizeAngleDegrees(GetCommandAngle() + CalibrationOffsetDeg);
	ApplyDisplayedAngle();
}

void UBP_SelsynReceiverComponent::ApplyDisplayedAngle()
{
	USceneComponent* const TargetComponent = ResolveDrivenComponent();
	if (!IsValid(TargetComponent))
	{
		return;
	}

	float FinalAngle = DisplayedAngle;
	if (bInvertDirection)
	{
		FinalAngle = -FinalAngle;
	}

	FRotator NewRotation = BaseRelativeRotation;
	switch (DriveAxis)
	{
	case ESelsynDriveAxis::Yaw:
		NewRotation.Yaw = BaseRelativeRotation.Yaw + FinalAngle;
		break;

	case ESelsynDriveAxis::Pitch:
		NewRotation.Pitch = BaseRelativeRotation.Pitch + FinalAngle;
		break;

	case ESelsynDriveAxis::Roll:
		NewRotation.Roll = BaseRelativeRotation.Roll + FinalAngle;
		break;

	default:
		break;
	}

	TargetComponent->SetRelativeRotation(NewRotation);
}

USceneComponent* UBP_SelsynReceiverComponent::ResolveDrivenComponent() const
{
	if (DrivenComponent)
	{
		return DrivenComponent;
	}

	const AActor* const Owner = GetOwner();
	return Owner ? Owner->GetRootComponent() : nullptr;
}

void UBP_SelsynReceiverComponent::CacheBaseRelativeRotation()
{
	if (const USceneComponent* const TargetComponent = ResolveDrivenComponent())
	{
		BaseRelativeRotation = TargetComponent->GetRelativeRotation();
	}
}

float UBP_SelsynReceiverComponent::NormalizeAngleDegrees(float AngleDegrees)
{
	float NormalizedAngle = FMath::Fmod(AngleDegrees, 360.0f);
	if (NormalizedAngle < 0.0f)
	{
		NormalizedAngle += 360.0f;
	}

	return NormalizedAngle;
}
