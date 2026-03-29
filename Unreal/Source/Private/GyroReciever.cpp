#include "GyroReciever.h"

#include "TDC.h"

namespace
{
	constexpr int32 GyroRecieverTubeCount = 4;
	constexpr float MaxSpreadAngleDegrees = 20.0f;
}

AGyroReciever::AGyroReciever()
{
	PrimaryActorTick.bCanEverTick = true;

	TDCRef = nullptr;
	bTransmissionLinkOperational = true;
	bMachineAmplifierOperational = true;
	bDcMotorOperational = true;
	AngleSettingMode = EGyroRecieverAngleSettingMode::SpreadManualGyroAutomatic;
	SalvoSize = EGyroRecieverSalvoSize::SingleShot;
	HighSpeedSwitchPosition = EGyroRecieverHighSpeedSwitchPosition::Neutral;
	ReceivedGyroAngle = 0.0f;
	ReceivedSpreadAngle = 0.0f;
	CurrentGyroSetting = 0.0f;
	ManualSpreadAngle = 0.0f;
	AlignmentErrorDegrees = 0.0f;
	bAlignmentLampLit = true;
	bNotFollowingLampLit = false;
	bLeftHighSpeedLampLit = false;
	bRightHighSpeedLampLit = false;
	FollowUpSpeedDegPerSec = 2.0f;
	CatchUpSpeedDegPerSec = 2.75f;
	HighSpeedDegPerSec = 4.75f;
	AlignmentToleranceDeg = 0.5f;
	HighSpeedAssistThresholdDeg = 10.0f;
	SpreadZeroToleranceDeg = 0.05f;

	EnsureTubeArrays();
	SelectedTubes[0] = true;
}

void AGyroReciever::BeginPlay()
{
	Super::BeginPlay();

	RecalculateReceiver();
}

void AGyroReciever::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RecalculateReceiver();
}

void AGyroReciever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnsureTubeArrays();

	const float PreviousCommandAngle = ReceivedGyroAngle;
	SyncFromTDC();
	const float CommandRateDegPerSec =
		(DeltaTime > KINDA_SMALL_NUMBER && bTransmissionLinkOperational && IsValid(TDCRef))
			? FMath::Abs(FMath::FindDeltaAngleDegrees(PreviousCommandAngle, ReceivedGyroAngle)) / DeltaTime
			: 0.0f;

	UpdateAutomaticGyro(DeltaTime, CommandRateDegPerSec);
	UpdateIndicatorLamps();
	UpdateTubeOutputs();
}

void AGyroReciever::RecalculateReceiver()
{
	EnsureTubeArrays();
	ManualSpreadAngle = FMath::Clamp(ManualSpreadAngle, 0.0f, MaxSpreadAngleDegrees);
	CurrentGyroSetting = NormalizeAngleDegrees(CurrentGyroSetting);
	SyncFromTDC();
	UpdateIndicatorLamps();
	UpdateTubeOutputs();
}

void AGyroReciever::SyncFromTDC()
{
	if (!bTransmissionLinkOperational || !IsValid(TDCRef))
	{
		return;
	}

	const float NewReceivedGyroAngle = NormalizeAngleDegrees(TDCRef->GyroAngle);
	const float NewReceivedSpreadAngle = FMath::Clamp(TDCRef->SpreadAngle, 0.0f, MaxSpreadAngleDegrees);

	ReceivedGyroAngle = NewReceivedGyroAngle;
	ReceivedSpreadAngle = NewReceivedSpreadAngle;
}

void AGyroReciever::ApplyHandwheelInput(float DeltaDegrees)
{
	switch (AngleSettingMode)
	{
	case EGyroRecieverAngleSettingMode::GyroManual:
		SetManualGyroAngle(CurrentGyroSetting + DeltaDegrees);
		break;

	case EGyroRecieverAngleSettingMode::SpreadManualGyroAutomatic:
	case EGyroRecieverAngleSettingMode::SpreadManual:
		SetManualSpreadAngle(ManualSpreadAngle + DeltaDegrees);
		break;

	default:
		break;
	}
}

void AGyroReciever::SetManualSpreadAngle(float NewAngle)
{
	ManualSpreadAngle = FMath::Clamp(NewAngle, 0.0f, MaxSpreadAngleDegrees);
	UpdateTubeOutputs();
}

void AGyroReciever::SetManualGyroAngle(float NewAngle)
{
	CurrentGyroSetting = NormalizeAngleDegrees(NewAngle);
	UpdateIndicatorLamps();
	UpdateTubeOutputs();
}

void AGyroReciever::SetAngleSettingMode(EGyroRecieverAngleSettingMode NewMode)
{
	AngleSettingMode = NewMode;
	UpdateIndicatorLamps();
}

bool AGyroReciever::SetSalvoSize(EGyroRecieverSalvoSize NewSalvoSize)
{
	if (!FMath::IsNearlyZero(ManualSpreadAngle, SpreadZeroToleranceDeg))
	{
		return false;
	}

	SalvoSize = NewSalvoSize;
	UpdateTubeOutputs();
	return true;
}

void AGyroReciever::SetHighSpeedSwitchPosition(EGyroRecieverHighSpeedSwitchPosition NewPosition)
{
	HighSpeedSwitchPosition = NewPosition;
}

void AGyroReciever::SetTubeSelected(int32 TubeIndex, bool bSelected)
{
	EnsureTubeArrays();
	if (!SelectedTubes.IsValidIndex(TubeIndex))
	{
		return;
	}

	SelectedTubes[TubeIndex] = bSelected;
}

float AGyroReciever::GetTubeGyroAngle(int32 TubeIndex) const
{
	return TubeGyroAngles.IsValidIndex(TubeIndex) ? TubeGyroAngles[TubeIndex] : 0.0f;
}

float AGyroReciever::GetTubeSpreadFactor(int32 TubeIndex) const
{
	return TubeSpreadFactors.IsValidIndex(TubeIndex) ? TubeSpreadFactors[TubeIndex] : 0.0f;
}

float AGyroReciever::GetReceivedGyroFineDial() const
{
	return NormalizeFineDial(ReceivedGyroAngle);
}

float AGyroReciever::GetSetGyroFineDial() const
{
	return NormalizeFineDial(CurrentGyroSetting);
}

void AGyroReciever::EnsureTubeArrays()
{
	if (SelectedTubes.Num() != GyroRecieverTubeCount)
	{
		SelectedTubes.SetNum(GyroRecieverTubeCount);
	}

	if (TubeGyroAngles.Num() != GyroRecieverTubeCount)
	{
		TubeGyroAngles.SetNumZeroed(GyroRecieverTubeCount);
	}

	if (TubeSpreadFactors.Num() != GyroRecieverTubeCount)
	{
		TubeSpreadFactors.SetNumZeroed(GyroRecieverTubeCount);
	}
}

void AGyroReciever::UpdateAutomaticGyro(float DeltaTime, float CommandRateDegPerSec)
{
	const bool bCanAutoDrive =
		AngleSettingMode == EGyroRecieverAngleSettingMode::SpreadManualGyroAutomatic &&
		bTransmissionLinkOperational &&
		bMachineAmplifierOperational &&
		bDcMotorOperational &&
		IsValid(TDCRef);

	if (!bCanAutoDrive)
	{
		return;
	}

	const float DeltaToCommand = FMath::FindDeltaAngleDegrees(CurrentGyroSetting, ReceivedGyroAngle);
	const float MotorSpeed = GetMotorSpeed(CommandRateDegPerSec);
	const float MaxStep = MotorSpeed * FMath::Max(DeltaTime, 0.0f);
	const float Step = FMath::Clamp(DeltaToCommand, -MaxStep, MaxStep);
	CurrentGyroSetting = NormalizeAngleDegrees(CurrentGyroSetting + Step);
}

void AGyroReciever::UpdateIndicatorLamps()
{
	if (!bTransmissionLinkOperational || !IsValid(TDCRef))
	{
		AlignmentErrorDegrees = 0.0f;
		bAlignmentLampLit = false;
		bNotFollowingLampLit = false;
		bLeftHighSpeedLampLit = false;
		bRightHighSpeedLampLit = false;
		return;
	}

	const float DeltaToCommand = FMath::FindDeltaAngleDegrees(CurrentGyroSetting, ReceivedGyroAngle);
	AlignmentErrorDegrees = FMath::Abs(DeltaToCommand);
	bAlignmentLampLit = AlignmentErrorDegrees <= AlignmentToleranceDeg;
	bNotFollowingLampLit = !bAlignmentLampLit;

	const bool bNeedsHighSpeedAssist = AlignmentErrorDegrees > HighSpeedAssistThresholdDeg;
	bLeftHighSpeedLampLit = bNeedsHighSpeedAssist && DeltaToCommand < 0.0f;
	bRightHighSpeedLampLit = bNeedsHighSpeedAssist && DeltaToCommand > 0.0f;
}

void AGyroReciever::UpdateTubeOutputs()
{
	EnsureTubeArrays();

	for (int32 TubeIndex = 0; TubeIndex < GyroRecieverTubeCount; ++TubeIndex)
	{
		const float SpreadFactor = GetSpreadFactorForTube(TubeIndex);
		TubeSpreadFactors[TubeIndex] = SpreadFactor;
		TubeGyroAngles[TubeIndex] = NormalizeAngleDegrees(CurrentGyroSetting + (ManualSpreadAngle * SpreadFactor));
	}
}

float AGyroReciever::GetMotorSpeed(float CommandRateDegPerSec) const
{
	const float HighSpeedSwitchSign = GetHighSpeedSwitchSign();
	const float DeltaToCommand = FMath::FindDeltaAngleDegrees(CurrentGyroSetting, ReceivedGyroAngle);

	if (!FMath::IsNearlyZero(HighSpeedSwitchSign) && FMath::Sign(DeltaToCommand) == HighSpeedSwitchSign)
	{
		return HighSpeedDegPerSec;
	}

	return CommandRateDegPerSec > FollowUpSpeedDegPerSec ? CatchUpSpeedDegPerSec : FollowUpSpeedDegPerSec;
}

float AGyroReciever::GetHighSpeedSwitchSign() const
{
	switch (HighSpeedSwitchPosition)
	{
	case EGyroRecieverHighSpeedSwitchPosition::Left:
		return -1.0f;

	case EGyroRecieverHighSpeedSwitchPosition::Right:
		return 1.0f;

	case EGyroRecieverHighSpeedSwitchPosition::Neutral:
	default:
		return 0.0f;
	}
}

float AGyroReciever::GetSpreadFactorForTube(int32 TubeIndex) const
{
	switch (SalvoSize)
	{
	case EGyroRecieverSalvoSize::SingleShot:
		return 0.0f;

	case EGyroRecieverSalvoSize::TwoTorpedoSalvo:
		return TubeIndex <= 1 ? 0.5f : -0.5f;

	case EGyroRecieverSalvoSize::ThreeTorpedoSalvo:
		if (TubeIndex == 0)
		{
			return 1.0f;
		}
		if (TubeIndex == 3)
		{
			return -1.0f;
		}
		return 0.0f;

	case EGyroRecieverSalvoSize::FourTorpedoSalvo:
		switch (TubeIndex)
		{
		case 0:
			return 1.5f;
		case 1:
			return -0.5f;
		case 2:
			return 0.5f;
		case 3:
			return -1.5f;
		default:
			return 0.0f;
		}

	default:
		return 0.0f;
	}
}

float AGyroReciever::NormalizeAngleDegrees(float AngleDegrees)
{
	float NormalizedAngle = FMath::Fmod(AngleDegrees, 360.0f);
	if (NormalizedAngle < 0.0f)
	{
		NormalizedAngle += 360.0f;
	}

	return NormalizedAngle;
}

float AGyroReciever::NormalizeFineDial(float AngleDegrees)
{
	float FineAngle = FMath::Fmod(AngleDegrees, 10.0f);
	if (FineAngle < 0.0f)
	{
		FineAngle += 10.0f;
	}

	return FineAngle;
}
