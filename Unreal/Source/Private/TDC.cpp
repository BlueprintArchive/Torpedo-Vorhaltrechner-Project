#include "TDC.h"

#include <limits>

#if WITH_EDITOR
#include "UObject/UnrealType.h"
#endif

namespace
{
	constexpr float MetersPerHectometer = 100.0f;

	float QuantizeDeflectionAngleForAcceptance(float DeflectionAngleDegrees)
	{
		const float Quantized = FMath::RoundToFloat(DeflectionAngleDegrees / 10.0f) * 10.0f;
		return FMath::Clamp(Quantized, 0.0f, 180.0f);
	}
}

ATDC::ATDC()
{
	PrimaryActorTick.bCanEverTick = true;

	ParallaxUpdateInterval = 0.1f;
	ParallaxMotorRateDegreesPerSecond = 6.0f;
	MaxParallaxCorrectionDegrees = 25.0f;
	TorpedoSpeed = 30.0f;
	TargetSpeed = 0.0f;
	ParallaxCorrection = 0.0f;
	DistanceToTarget = 3.0f;
	Quotient = 0.0f;
	AngleOnTheBow = 0.0f;
	bBowToTheRight = true;
	BowSideLabel = TEXT("Bow to the right");
	ImpactAngle = 0.0f;
	DeflectionAngle = 0.0f;
	ContinuousDeflectionAngle = 0.0f;
	SignedDeflectionAngle = 0.0f;
	bDisplayImpactAngleOnTriangleDial = true;
	TriangleDialAngle = 0.0f;
	GyroAngle = 0.0f;
	TargetBearing = 0.0f;
	TurnRatioCorrection = 0.0f;
	TurnRatioScale = 0.4f;
	bParallaxForward = true;
	TargetLength = 100.0f;
	SpreadAngle = 0.0f;
	Delta1 = 0.0f;
	Delta2 = 0.0f;
	bEnableParallaxMotor = true;
	ManualParallax = 0.0f;
}

void ATDC::BeginPlay()
{
	Super::BeginPlay();

	RecalculateSolution();
}

void ATDC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RecalculateSolution();
}

#if WITH_EDITOR
void ATDC::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool bAngleOnTheBowChanged =
		PropertyChangedEvent.Property != nullptr &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ATDC, AngleOnTheBow);
	const float EditedAngleOnTheBow = AngleOnTheBow;

	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (bAngleOnTheBowChanged)
	{
		SetAngleOnTheBow(EditedAngleOnTheBow);
		return;
	}

	RecalculateSolution();
}
#endif

void ATDC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float SafeInterval = FMath::Max(ParallaxUpdateInterval, KINDA_SMALL_NUMBER);
	float RemainingTime = FMath::Max(DeltaTime, 0.0f);

	while (RemainingTime > 0.0f)
	{
		const float StepTime = FMath::Min(RemainingTime, SafeInterval);
		UpdateParallax(StepTime);
		RemainingTime -= StepTime;
	}
}

void ATDC::SetAngleOnTheBow(float Angle)
{
	bBowToTheRight = Angle >= 0.0f;
	BowSideLabel = bBowToTheRight ? TEXT("Bow to the right") : TEXT("Bow to the left");
	SolveImpactAngleFromBow(FMath::Clamp(FMath::Abs(Angle), 0.0f, 180.0f));
	CalculateDeflectionAngle();
	UpdateDerivedStateFromDelta1();
	CalculateSpread();
}

void ATDC::SetDistanceToTarget(float Distance)
{
	DistanceToTarget = FMath::Clamp(Distance, 0.0f, 100.0f);
	CalculateSpread();
}

void ATDC::SetTorpedoSpeed(float Speed)
{
	TorpedoSpeed = FMath::Clamp(Speed, 10.0f, 44.0f);
	UpdateQuotient();
	CalculateDeflectionAngle();
	UpdateDerivedStateFromDelta1();
	CalculateSpread();
}

void ATDC::SetTargetSpeed(float Speed)
{
	TargetSpeed = FMath::Clamp(Speed, 0.0f, 55.0f);
	UpdateQuotient();
	CalculateDeflectionAngle();
	UpdateDerivedStateFromDelta1();
	CalculateSpread();
}

void ATDC::SetTargetBearing(float Angle)
{
	TargetBearing = NormalizeDegrees(Angle);
	UpdateDerivedStateFromDelta1();
}

void ATDC::SetTurnRatioCorrection(float Correction)
{
	TurnRatioCorrection = Correction;
	UpdateDerivedStateFromDelta1();
}

void ATDC::SetParallaxForward(bool bNewValue)
{
	bParallaxForward = bNewValue;
	UpdateDerivedStateFromDelta1();
}

void ATDC::SetTargetLength(float NewValue)
{
	TargetLength = FMath::Max(NewValue, 1.0f);
	CalculateSpread();
}

void ATDC::SetParallaxMotorEnabled(bool bEnabled)
{
	bEnableParallaxMotor = bEnabled;
}

void ATDC::RecalculateSolution()
{
	RefreshSolutionFromInputs();
	SolveParallaxToEquilibrium();
	UpdateTriangleDialAngle();
	CalculateSpread();
}

float ATDC::GetDelta1() const
{
	return ParallaxCorrection;
}

float ATDC::GetDelta2() const
{
	return bParallaxForward ? RadiansToDegrees(Delta2) : -RadiansToDegrees(Delta2);
}

float ATDC::GetMaximumDistanceToTarget() const
{
	return ComputeMaximumDistanceFactorFromInputs(ImpactAngle, ContinuousDeflectionAngle);
}

float ATDC::GetTriangleDialAngle() const
{
	return TriangleDialAngle;
}

float ATDC::DegreesToRadians(float Degrees)
{
	return FMath::DegreesToRadians(Degrees);
}

float ATDC::RadiansToDegrees(float Radians)
{
	return FMath::RadiansToDegrees(Radians);
}

float ATDC::TruncateDecimals(float Number, int32 Digits)
{
	if (Digits <= 0)
	{
		return Number >= 0.0f ? FMath::FloorToFloat(Number) : FMath::CeilToFloat(Number);
	}

	const double Scale = FMath::Pow(10.0, static_cast<double>(Digits));
	const double ScaledValue = static_cast<double>(Number) * Scale;
	const double Truncated = ScaledValue >= 0.0 ? FMath::FloorToDouble(ScaledValue) / Scale : FMath::CeilToDouble(ScaledValue) / Scale;
	return static_cast<float>(Truncated);
}

float ATDC::ComputeQuotientFromSpeeds(float InTargetSpeed, float InTorpedoSpeed)
{
	if (FMath::IsNearlyZero(InTorpedoSpeed))
	{
		return 0.0f;
	}

	return InTargetSpeed / InTorpedoSpeed;
}

float ATDC::ComputeDeflectionAngleFromInputs(float InImpactAngle, float InTargetSpeed, float InTorpedoSpeed)
{
	const float LocalQuotient = ComputeQuotientFromSpeeds(InTargetSpeed, InTorpedoSpeed);
	const double ImpactRadians = FMath::DegreesToRadians(static_cast<double>(InImpactAngle));
	const double SinImpact = FMath::Sin(ImpactRadians);
	const double TanImpact = FMath::Tan(ImpactRadians);

	if (FMath::IsNearlyZero(LocalQuotient) || FMath::IsNearlyZero(SinImpact) || !FMath::IsFinite(TanImpact))
	{
		return 0.0f;
	}

	const double CtgBeta = (1.0 / (static_cast<double>(LocalQuotient) * SinImpact)) - (1.0 / TanImpact);
	if (!FMath::IsFinite(CtgBeta))
	{
		return 0.0f;
	}

	double InverseCtg = 0.0;
	if (FMath::IsNearlyZero(CtgBeta))
	{
		InverseCtg = CtgBeta >= 0.0 ? std::numeric_limits<double>::max() : -std::numeric_limits<double>::max();
	}
	else
	{
		InverseCtg = 1.0 / CtgBeta;
	}

	return static_cast<float>(FMath::RadiansToDegrees(FMath::Atan(InverseCtg)));
}

float ATDC::ComputeSignedDeflectionAngle(float InDeflectionAngleMagnitude, float InTargetBearing)
{
	const float BetaMagnitude = FMath::Abs(InDeflectionAngleMagnitude);
	if (FMath::IsNearlyZero(BetaMagnitude))
	{
		return 0.0f;
	}

	const float NormalizedBearing = FMath::Fmod(InTargetBearing, 360.0f) < 0.0f
		? FMath::Fmod(InTargetBearing, 360.0f) + 360.0f
		: FMath::Fmod(InTargetBearing, 360.0f);
	const float BearingSign = NormalizedBearing <= 180.0f ? -1.0f : 1.0f;
	return BetaMagnitude * BearingSign;
}

float ATDC::ComputeGyroAngleFromInputs(float InTargetBearing, float InParallaxCorrection, float InSignedDeflectionAngle, float InTurnRatioCorrection, float InTurnRatioScale)
{
	const float CorrectedBearing = InTargetBearing + (InTurnRatioCorrection * InTurnRatioScale);
	float Result = CorrectedBearing + InParallaxCorrection + InSignedDeflectionAngle;
	Result = FMath::Fmod(Result, 360.0f);
	if (Result < 0.0f)
	{
		Result += 360.0f;
	}

	return Result;
}

float ATDC::ComputeMaximumDistanceFactorFromInputs(float InImpactAngle, float InDeflectionAngle)
{
	const double ImpactRadians = FMath::DegreesToRadians(static_cast<double>(InImpactAngle));
	const double SumRadians = FMath::DegreesToRadians(static_cast<double>(InImpactAngle + InDeflectionAngle));
	const double Denominator = FMath::Sin(SumRadians);
	double Result = 1.0;

	if (!FMath::IsNearlyZero(Denominator))
	{
		Result = FMath::Sin(ImpactRadians) / Denominator;
	}

	if (!FMath::IsFinite(Result))
	{
		Result = 1.0;
	}

	return static_cast<float>(Result);
}

float ATDC::ComputeMaximumDistanceFromInputs(float InImpactAngle, float InDeflectionAngle, float InMaximumTorpedoRun)
{
	return ComputeMaximumDistanceFactorFromInputs(InImpactAngle, InDeflectionAngle) * InMaximumTorpedoRun;
}

float ATDC::ComputeSpreadAngleFromInputs(float InTargetLength, float InDistanceToTarget, float InAngleOnTheBow, float InTargetSpeed, float InTorpedoSpeed)
{
	const double DistanceMeters = static_cast<double>(FMath::Max(InDistanceToTarget, 0.1f)) * MetersPerHectometer;
	const float LocalQuotient = ComputeQuotientFromSpeeds(InTargetSpeed, InTorpedoSpeed);
	if (FMath::IsNearlyZero(LocalQuotient))
	{
		return 0.0f;
	}

	const double BowRadians = FMath::DegreesToRadians(static_cast<double>(FMath::Abs(InAngleOnTheBow)));
	const double Sine = FMath::Sin(BowRadians);
	const double Cosine = FMath::Cos(BowRadians);
	const double QuotientTerm = 1.0 / static_cast<double>(LocalQuotient);
	const double Radical = FMath::Square(QuotientTerm) - FMath::Square(Sine);

	if (Radical <= 0.0)
	{
		return 0.0f;
	}

	const double Angle = (static_cast<double>(InTargetLength) / DistanceMeters) * Sine *
		((Cosine / FMath::Sqrt(Radical)) + 1.0);
	const double SpreadDegrees = FMath::RadiansToDegrees(Angle);

	if (!FMath::IsFinite(SpreadDegrees))
	{
		return 0.0f;
	}

	return SpreadDegrees > 20.0 ? 20.0f : static_cast<float>(SpreadDegrees);
}

float ATDC::ComputeTriangleDialAngleFromInputs(float InImpactAngle, float InDeflectionAngle, bool bInDisplayImpactAngleOnTriangleDial)
{
	return bInDisplayImpactAngleOnTriangleDial ? InImpactAngle : InDeflectionAngle;
}

FVector2D ATDC::ComputeEquivalentPointCartesianFromGyro(float InGyroAngleDegrees, bool bForwardTube)
{
	const auto MakePoint = [](double LocalAngleDegrees, bool bMirrorX, bool bMirrorY) -> FVector2D
	{
		const double AngleRadians = FMath::DegreesToRadians(LocalAngleDegrees);
		const double BaseX = 27.0 + 9.5 + 95.0 * FMath::Sin(AngleRadians) - (AngleRadians * 95.0 + 9.5) * FMath::Cos(AngleRadians);
		const double BaseY = 95.0 * (1.0 - FMath::Cos(AngleRadians)) - (AngleRadians * 95.0 + 9.5) * FMath::Sin(AngleRadians);

		return FVector2D(
			static_cast<float>(bMirrorX ? -BaseX : BaseX),
			static_cast<float>(bMirrorY ? -BaseY : BaseY));
	};

	if (bForwardTube)
	{
		if (InGyroAngleDegrees >= 0.0f && InGyroAngleDegrees <= 180.0f)
		{
			return MakePoint(InGyroAngleDegrees, false, false);
		}

		return MakePoint(360.0 - InGyroAngleDegrees, false, true);
	}

	if (InGyroAngleDegrees >= 0.0f && InGyroAngleDegrees <= 180.0f)
	{
		return MakePoint(180.0 - InGyroAngleDegrees, true, false);
	}

	return MakePoint(InGyroAngleDegrees - 180.0, true, true);
}

FVector2D ATDC::ComputeEquivalentPointPolarFromGyro(float InGyroAngleDegrees, bool bForwardTube)
{
	const FVector2D Cartesian = ComputeEquivalentPointCartesianFromGyro(InGyroAngleDegrees, bForwardTube);
	const double Distance = FMath::Sqrt(FMath::Square(static_cast<double>(Cartesian.X)) + FMath::Square(static_cast<double>(Cartesian.Y)));
	const double Angle = FMath::Atan2(static_cast<double>(Cartesian.Y), static_cast<double>(Cartesian.X));
	return FVector2D(static_cast<float>(Distance), static_cast<float>(Angle));
}

void ATDC::ApplyInputClamps()
{
	TorpedoSpeed = FMath::Clamp(TorpedoSpeed, 10.0f, 44.0f);
	TargetSpeed = FMath::Clamp(TargetSpeed, 0.0f, 55.0f);
	DistanceToTarget = FMath::Clamp(DistanceToTarget, 0.0f, 100.0f);
	ImpactAngle = FMath::Clamp(ImpactAngle, 0.0f, 180.0f);
	TargetBearing = NormalizeDegrees(TargetBearing);
	TurnRatioScale = FMath::Max(TurnRatioScale, 0.0f);
	TargetLength = FMath::Max(TargetLength, 1.0f);
	ManualParallax = FMath::Clamp(ManualParallax, -MaxParallaxCorrectionDegrees, MaxParallaxCorrectionDegrees);
}

void ATDC::RefreshSolutionFromInputs()
{
	ApplyInputClamps();
	UpdateQuotient();
	CalculateDeflectionAngle();
	UpdateDerivedStateFromDelta1();
	CalculateSpread();
}

void ATDC::UpdateQuotient()
{
	Quotient = ComputeQuotientFromSpeeds(TargetSpeed, TorpedoSpeed);
}

void ATDC::UpdateParallax(float StepTime)
{
	const float MaxCorrectionRadians = DegreesToRadians(MaxParallaxCorrectionDegrees);

	if (!bEnableParallaxMotor)
	{
		Delta1 = FMath::Clamp(DegreesToRadians(ManualParallax), -MaxCorrectionRadians, MaxCorrectionRadians);
		UpdateDerivedStateFromDelta1();
		Delta2 = CalculateDelta2FromCurrentState();
	}
	else
	{
		UpdateDerivedStateFromDelta1();
		const float TargetDelta = FMath::Clamp(CalculateDelta2FromCurrentState(), -MaxCorrectionRadians, MaxCorrectionRadians);
		const float MaxStepRadians = DegreesToRadians(FMath::Max(ParallaxMotorRateDegreesPerSecond, 0.0f)) * FMath::Max(StepTime, 0.0f);
		const float MotorStep = FMath::Clamp(TargetDelta - Delta1, -MaxStepRadians, MaxStepRadians);
		Delta1 = FMath::Clamp(Delta1 + MotorStep, -MaxCorrectionRadians, MaxCorrectionRadians);
		UpdateDerivedStateFromDelta1();
		Delta2 = CalculateDelta2FromCurrentState();
	}
}

void ATDC::CalculateDeflectionAngle()
{
	ContinuousDeflectionAngle = ComputeDeflectionAngleFromInputs(ImpactAngle, TargetSpeed, TorpedoSpeed);
	DeflectionAngle = QuantizeDeflectionAngleForAcceptance(ContinuousDeflectionAngle);
	UpdateTriangleDialAngle();
}

void ATDC::CalculateSpread()
{
	SpreadAngle = ComputeSpreadAngleFromInputs(TargetLength, DistanceToTarget, AngleOnTheBow, TargetSpeed, TorpedoSpeed);
}

void ATDC::UpdateTriangleDialAngle()
{
	TriangleDialAngle = ComputeTriangleDialAngleFromInputs(ImpactAngle, DeflectionAngle, bDisplayImpactAngleOnTriangleDial);
}

float ATDC::NormalizeDegrees(float AngleDegrees) const
{
	float Normalized = FMath::Fmod(AngleDegrees, 360.0f);
	if (Normalized < 0.0f)
	{
		Normalized += 360.0f;
	}

	return Normalized;
}

float ATDC::GetCorrectedTargetBearingDegrees() const
{
	return TargetBearing + (TurnRatioCorrection * TurnRatioScale);
}

bool ATDC::SolveImpactAngleFromBow(float DesiredAngleOnTheBow, int32 MaxIterations, float ToleranceDegrees)
{
	const float TargetBowMagnitude = FMath::Clamp(DesiredAngleOnTheBow, 0.0f, 180.0f);
	const int32 IterationCount = FMath::Max(MaxIterations, 1);
	auto EvaluateBowMagnitude = [this](float CandidateImpactAngle) -> float
	{
		const float CandidateDeflection = ComputeDeflectionAngleFromInputs(CandidateImpactAngle, TargetSpeed, TorpedoSpeed);
		return FMath::Clamp(180.0f - (CandidateImpactAngle + CandidateDeflection), 0.0f, 180.0f);
	};

	float LowerImpact = 0.0f;
	float UpperImpact = 180.0f;
	float LowerBow = EvaluateBowMagnitude(LowerImpact);
	float UpperBow = EvaluateBowMagnitude(UpperImpact);

	// If the requested bow angle lies outside the reachable range, clamp to the nearest physical solution.
	if (TargetBowMagnitude >= LowerBow)
	{
		ImpactAngle = LowerImpact;
		return FMath::IsNearlyEqual(LowerBow, TargetBowMagnitude, ToleranceDegrees);
	}

	if (TargetBowMagnitude <= UpperBow)
	{
		ImpactAngle = UpperImpact;
		return FMath::IsNearlyEqual(UpperBow, TargetBowMagnitude, ToleranceDegrees);
	}

	float BestImpact = FMath::Clamp(ImpactAngle, LowerImpact, UpperImpact);
	float BestError = FMath::Abs(EvaluateBowMagnitude(BestImpact) - TargetBowMagnitude);

	for (int32 Iteration = 0; Iteration < IterationCount; ++Iteration)
	{
		const float MidImpact = 0.5f * (LowerImpact + UpperImpact);
		const float MidBow = EvaluateBowMagnitude(MidImpact);
		const float MidError = FMath::Abs(MidBow - TargetBowMagnitude);

		if (MidError < BestError)
		{
			BestImpact = MidImpact;
			BestError = MidError;
		}

		if (MidError <= ToleranceDegrees)
		{
			ImpactAngle = MidImpact;
			return true;
		}

		if (MidBow > TargetBowMagnitude)
		{
			LowerImpact = MidImpact;
		}
		else
		{
			UpperImpact = MidImpact;
		}
	}

	ImpactAngle = BestImpact;
	return false;
}

void ATDC::SolveParallaxToEquilibrium(int32 MaxIterations, float ToleranceRadians)
{
	const float StepTime = FMath::Max(ParallaxUpdateInterval, KINDA_SMALL_NUMBER);
	if (!bEnableParallaxMotor)
	{
		UpdateParallax(StepTime);
		return;
	}

	for (int32 Iteration = 0; Iteration < MaxIterations; ++Iteration)
	{
		UpdateParallax(StepTime);
		const float TargetDelta = FMath::Clamp(CalculateDelta2FromCurrentState(), -DegreesToRadians(MaxParallaxCorrectionDegrees), DegreesToRadians(MaxParallaxCorrectionDegrees));
		if (FMath::Abs(TargetDelta - Delta1) <= ToleranceRadians)
		{
			break;
		}
	}
}

void ATDC::UpdateDerivedStateFromDelta1()
{
	UpdateTriangleDialAngle();
	ParallaxCorrection = FMath::Clamp(RadiansToDegrees(Delta1), -MaxParallaxCorrectionDegrees, MaxParallaxCorrectionDegrees);
	if (!bParallaxForward)
	{
		ParallaxCorrection = -ParallaxCorrection;
	}

	const float AngleOnTheBowMagnitude = FMath::Clamp(180.0f - (ImpactAngle + ContinuousDeflectionAngle), 0.0f, 180.0f);
	AngleOnTheBow = bBowToTheRight ? AngleOnTheBowMagnitude : -AngleOnTheBowMagnitude;
	BowSideLabel = bBowToTheRight ? TEXT("Bow to the right") : TEXT("Bow to the left");
	SignedDeflectionAngle = bBowToTheRight ? -FMath::Abs(DeflectionAngle) : FMath::Abs(DeflectionAngle);
	GyroAngle = ComputeGyroAngleFromInputs(TargetBearing, ParallaxCorrection, SignedDeflectionAngle, TurnRatioCorrection, TurnRatioScale);
}

float ATDC::CalculateDelta2FromCurrentState() const
{
	const double SafeDistance = FMath::Max(static_cast<double>(GetDistanceToTargetMeters()), 1.0);
	const double TargetBearingRadians = FMath::DegreesToRadians(static_cast<double>(GetCorrectedTargetBearingDegrees()));
	const FVector2D Cartesian = CalculateCartesianCoordinateOfIdealTorpedoStartPoint(GyroAngle);
	const FVector2D Polar = CalculatePolarCoordinateOfIdealTorpedoStartPoint(Cartesian.X, Cartesian.Y);
	const double X = Polar.X;
	const double O = Polar.Y;

	if (bParallaxForward)
	{
		return static_cast<float>(X * FMath::Sin(TargetBearingRadians + Delta1 - O) / SafeDistance);
	}

	return static_cast<float>(X * FMath::Sin(TargetBearingRadians - Delta1 - O) / SafeDistance);
}

FVector2D ATDC::CalculateCartesianCoordinateOfIdealTorpedoStartPoint(float AngleDegrees) const
{
	return ComputeEquivalentPointCartesianFromGyro(AngleDegrees, bParallaxForward);
}

FVector2D ATDC::CalculatePolarCoordinateOfIdealTorpedoStartPoint(float X, float Y) const
{
	const double Distance = FMath::Sqrt(FMath::Square(static_cast<double>(X)) + FMath::Square(static_cast<double>(Y)));
	const double Angle = FMath::Atan2(static_cast<double>(Y), static_cast<double>(X));
	return FVector2D(static_cast<float>(Distance), static_cast<float>(Angle));
}

float ATDC::GetDistanceToTargetMeters() const
{
	return DistanceToTarget * MetersPerHectometer;
}
