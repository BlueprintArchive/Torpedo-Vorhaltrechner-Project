#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "TDC.h"

namespace
{
	struct FSpreadTableRow
	{
		int32 RowNumber;
		float DistanceHm;
		float TargetLengthMeters;
		float AngleOnTheBowDegrees;
		float TorpedoSpeed;
		float TargetSpeed;
		float ExpectedSpreadDegrees;
	};

	const TArray<FSpreadTableRow> SpreadTableRows = {
		{1, 6.8f, 140.0f, 55.4f, 30.0f, 24.0f, 15.6f},
		{2, 20.0f, 160.0f, 54.4f, 44.0f, 35.2f, 6.0f},
		{3, 8.0f, 110.0f, 56.1f, 30.0f, 24.0f, 10.4f},
		{4, 8.4f, 110.0f, 38.0f, 40.0f, 24.0f, 7.0f},
		{5, 8.0f, 120.0f, 76.4f, 30.0f, 24.0f, 10.9f},
		{6, 5.0f, 100.0f, 60.4f, 30.0f, 24.0f, 15.4f},
		{7, 6.0f, 100.0f, 61.7f, 40.0f, 36.0f, 14.3f},
		{8, 9.0f, 180.0f, 53.9f, 40.0f, 16.0f, 11.6f},
		{9, 10.0f, 100.0f, 87.0f, 44.0f, 22.0f, 5.9f},
		{10, 6.8f, 140.0f, 55.4f, 30.0f, 24.0f, 15.6f},
		{11, 20.0f, 160.0f, 54.4f, 44.0f, 35.2f, 6.0f},
		{12, 8.0f, 110.0f, 56.1f, 30.0f, 24.0f, 10.4f},
		{13, 8.4f, 110.0f, 38.0f, 40.0f, 24.0f, 7.0f},
		{14, 8.0f, 120.0f, 76.4f, 30.0f, 24.0f, 10.9f},
		{15, 5.0f, 100.0f, 60.4f, 30.0f, 24.0f, 15.4f},
		{16, 6.0f, 100.0f, 61.7f, 40.0f, 36.0f, 14.3f},
	};

	struct FTriangleFormulaCase
	{
		float ImpactAngleDegrees;
		float TorpedoSpeed;
		float TargetSpeed;
	};

	const TArray<FTriangleFormulaCase> TriangleFormulaCases = {
		{45.0f, 30.0f, 15.0f},
		{60.0f, 30.0f, 24.0f},
		{82.0f, 44.0f, 22.0f},
		{100.0f, 40.0f, 16.0f},
	};

	struct FGyroFormulaCase
	{
		float TargetBearing;
		float ParallaxCorrection;
		float SignedDeflectionAngle;
		float TurnRatioCorrection;
		float TurnRatioScale;
		float ExpectedGyroAngle;
	};

	struct FGyroTableRow
	{
		int32 RowNumber;
		float TargetBearing;
		float ParallaxCorrection;
		float SignedDeflectionAngle;
		float ExpectedGyroAngle;
	};

	struct FMaximumDistanceCase
	{
		float ImpactAngle;
		float DeflectionAngle;
		float MaximumTorpedoRun;
	};

	struct FIntegratedTableRow
	{
		int32 RowNumber;
		float DistanceHm;
		float TargetLengthMeters;
		float SignedAngleOnTheBowDegrees;
		float TorpedoSpeed;
		float TargetSpeed;
		float TargetBearing;
		float ParallaxCorrection;
		float ExpectedDeflectionDegrees;
		float ExpectedGyroAngle;
		float ExpectedSpreadDegrees;
	};

	const TArray<FGyroFormulaCase> GyroFormulaCases = {
		{30.0f, 2.0f, -40.0f, 0.0f, 0.0f, 352.0f},
		{120.0f, -3.0f, 20.0f, 5.0f, 0.4f, 139.0f},
		{5.0f, 10.0f, -20.0f, 0.0f, 0.0f, 355.0f},
	};

	const TArray<FGyroTableRow> GyroTableRows = {
		{1, 48.08f, 1.9f, -40.0f, 10.0f},
		{2, 58.30f, 0.9f, -40.0f, 20.0f},
		{3, 247.40f, 2.6f, -40.0f, 210.0f},
		{4, 57.37f, 3.2f, -20.0f, 40.0f},
		{5, 116.86f, 3.1f, -50.0f, 70.0f},
		{6, 293.07f, 6.9f, -40.0f, 260.0f},
		{7, 136.60f, 3.4f, -50.0f, 90.0f},
		{8, 36.09f, 4.9f, 20.0f, 60.0f},
		{9, 17.04f, 3.0f, 30.0f, 50.0f},
		{10, 311.92f, -1.9f, 40.0f, 350.0f},
		{11, 300.90f, -0.9f, 40.0f, 340.0f},
		{12, 112.60f, -2.6f, 40.0f, 150.0f},
		{13, 303.23f, -3.2f, 20.0f, 320.0f},
		{14, 243.14f, -3.1f, 50.0f, 290.0f},
		{15, 66.93f, -6.9f, 40.0f, 100.0f},
		{16, 223.40f, -3.4f, 50.0f, 270.0f},
	};

	const TArray<FMaximumDistanceCase> MaximumDistanceCases = {
		{45.0f, 20.0f, 50.0f},
		{60.0f, 40.0f, 80.0f},
		{82.0f, 30.0f, 30.0f},
	};

	const TArray<FIntegratedTableRow> IntegratedTableRows = {
		{1, 6.8f, 140.0f, 55.4f, 30.0f, 24.0f, 48.08f, 1.9f, 40.0f, 10.0f, 15.6f},
		{2, 20.0f, 160.0f, 54.4f, 44.0f, 35.2f, 58.30f, 0.9f, 40.0f, 20.0f, 6.0f},
		{3, 8.0f, 110.0f, 56.1f, 30.0f, 24.0f, 247.40f, 2.6f, 40.0f, 210.0f, 10.4f},
		{4, 8.4f, 110.0f, 38.0f, 40.0f, 24.0f, 57.37f, 3.2f, 20.0f, 40.0f, 7.0f},
		{5, 8.0f, 120.0f, 76.4f, 30.0f, 24.0f, 116.86f, 3.1f, 50.0f, 70.0f, 10.9f},
		{6, 5.0f, 100.0f, 60.4f, 30.0f, 24.0f, 293.07f, 6.9f, 40.0f, 260.0f, 15.4f},
		{7, 6.0f, 100.0f, 61.7f, 40.0f, 36.0f, 136.60f, 3.4f, 50.0f, 90.0f, 14.3f},
		{8, 9.0f, 180.0f, -53.9f, 40.0f, 16.0f, 36.09f, 4.9f, 20.0f, 60.0f, 11.6f},
		{9, 10.0f, 100.0f, -87.0f, 44.0f, 22.0f, 17.04f, 3.0f, 30.0f, 50.0f, 5.9f},
		{10, 6.8f, 140.0f, -55.4f, 30.0f, 24.0f, 311.92f, -1.9f, 40.0f, 350.0f, 15.6f},
		{11, 20.0f, 160.0f, -54.4f, 44.0f, 35.2f, 300.90f, -0.9f, 40.0f, 340.0f, 6.0f},
		{12, 8.0f, 110.0f, -56.1f, 30.0f, 24.0f, 112.60f, -2.6f, 40.0f, 150.0f, 10.4f},
		{13, 8.4f, 110.0f, -38.0f, 40.0f, 24.0f, 303.23f, -3.2f, 20.0f, 320.0f, 7.0f},
		{14, 8.0f, 120.0f, -76.4f, 30.0f, 24.0f, 243.14f, -3.1f, 50.0f, 290.0f, 10.9f},
		{15, 5.0f, 100.0f, -60.4f, 30.0f, 24.0f, 66.93f, -6.9f, 40.0f, 100.0f, 15.4f},
		{16, 6.0f, 100.0f, -61.7f, 40.0f, 36.0f, 223.40f, -3.4f, 50.0f, 270.0f, 14.3f},
	};

	FString FormatRowLabel(int32 RowNumber)
	{
		return FString::Printf(TEXT("Row %d"), RowNumber);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCSpreadTableTest, "BlueprintArchive.TDC.Table.Spread", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCTriangleFormulaTest, "BlueprintArchive.TDC.Source.Triangle", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCGyroFormulaTest, "BlueprintArchive.TDC.Source.Gyro", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCTriangleDialDisplayTest, "BlueprintArchive.TDC.Source.TriangleDialDisplay", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCGyroTableTest, "BlueprintArchive.TDC.Table.Gyro", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCMaximumDistanceFormulaTest, "BlueprintArchive.TDC.Source.MaximumDistance", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTDCEquivalentPointSymmetryTest, "BlueprintArchive.TDC.Source.EquivalentPoint", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTDCSpreadTableTest::RunTest(const FString& Parameters)
{
	constexpr float SpreadToleranceDegrees = 0.15f;

	for (const FSpreadTableRow& Row : SpreadTableRows)
	{
		const float ComputedSpread = ATDC::ComputeSpreadAngleFromInputs(
			Row.TargetLengthMeters,
			Row.DistanceHm,
			Row.AngleOnTheBowDegrees,
			Row.TargetSpeed,
			Row.TorpedoSpeed);

		TestTrue(
			FString::Printf(TEXT("%s spread %.2f should match table %.2f"), *FormatRowLabel(Row.RowNumber), ComputedSpread, Row.ExpectedSpreadDegrees),
			FMath::IsNearlyEqual(ComputedSpread, Row.ExpectedSpreadDegrees, SpreadToleranceDegrees));
	}

	return true;
}

bool FTDCTriangleFormulaTest::RunTest(const FString& Parameters)
{
	constexpr float FormulaTolerance = 0.001f;

	for (const FTriangleFormulaCase& TestCase : TriangleFormulaCases)
	{
		const float DeflectionDegrees = ATDC::ComputeDeflectionAngleFromInputs(
			TestCase.ImpactAngleDegrees,
			TestCase.TargetSpeed,
			TestCase.TorpedoSpeed);
		const float Quotient = ATDC::ComputeQuotientFromSpeeds(TestCase.TargetSpeed, TestCase.TorpedoSpeed);
		const float LeftSide = FMath::Sin(FMath::DegreesToRadians(DeflectionDegrees));
		const float RightSide = Quotient * FMath::Sin(FMath::DegreesToRadians(TestCase.ImpactAngleDegrees + DeflectionDegrees));

		TestTrue(
			FString::Printf(TEXT("alpha %.2f should satisfy sin(beta) = u * sin(alpha + beta)"), TestCase.ImpactAngleDegrees),
			FMath::IsNearlyEqual(LeftSide, RightSide, FormulaTolerance));
	}

	return true;
}

bool FTDCMaximumDistanceFormulaTest::RunTest(const FString& Parameters)
{
	constexpr float FormulaTolerance = 0.001f;

	for (const FMaximumDistanceCase& TestCase : MaximumDistanceCases)
	{
		const float Factor = ATDC::ComputeMaximumDistanceFactorFromInputs(TestCase.ImpactAngle, TestCase.DeflectionAngle);
		const float ComputedDistance = ATDC::ComputeMaximumDistanceFromInputs(TestCase.ImpactAngle, TestCase.DeflectionAngle, TestCase.MaximumTorpedoRun);
		const float ExpectedFactor = FMath::Sin(FMath::DegreesToRadians(TestCase.ImpactAngle)) /
			FMath::Sin(FMath::DegreesToRadians(TestCase.ImpactAngle + TestCase.DeflectionAngle));

		TestTrue(
			FString::Printf(TEXT("maximum distance factor %.4f should match sine law %.4f"), Factor, ExpectedFactor),
			FMath::IsNearlyEqual(Factor, ExpectedFactor, FormulaTolerance));
		TestTrue(
			FString::Printf(TEXT("maximum distance %.4f should equal factor * run"), ComputedDistance),
			FMath::IsNearlyEqual(ComputedDistance, Factor * TestCase.MaximumTorpedoRun, FormulaTolerance));
	}

	return true;
}

bool FTDCEquivalentPointSymmetryTest::RunTest(const FString& Parameters)
{
	const TArray<float> SampleAngles = {15.0f, 45.0f, 90.0f, 135.0f, 210.0f, 330.0f};
	constexpr float Tolerance = 0.001f;

	for (const float Angle : SampleAngles)
	{
		const FVector2D ForwardPoint = ATDC::ComputeEquivalentPointCartesianFromGyro(Angle, true);
		const FVector2D RearPoint = ATDC::ComputeEquivalentPointCartesianFromGyro(Angle, false);

		TestTrue(
			FString::Printf(TEXT("forward-tube equivalent point x should stay positive for gyro %.2f"), Angle),
			ForwardPoint.X > 0.0f);
		TestTrue(
			FString::Printf(TEXT("rear-tube equivalent point x should stay negative for gyro %.2f"), Angle),
			RearPoint.X < 0.0f);
		TestTrue(
			FString::Printf(TEXT("equivalent point polar radius should remain positive for gyro %.2f"), Angle),
			ATDC::ComputeEquivalentPointPolarFromGyro(Angle, true).X > Tolerance && ATDC::ComputeEquivalentPointPolarFromGyro(Angle, false).X > Tolerance);
	}

	return true;
}

bool FTDCGyroFormulaTest::RunTest(const FString& Parameters)
{
	for (const FGyroFormulaCase& TestCase : GyroFormulaCases)
	{
		const float ComputedGyro = ATDC::ComputeGyroAngleFromInputs(
			TestCase.TargetBearing,
			TestCase.ParallaxCorrection,
			TestCase.SignedDeflectionAngle,
			TestCase.TurnRatioCorrection,
			TestCase.TurnRatioScale);

		TestTrue(
			FString::Printf(TEXT("gyro %.2f should match expected %.2f"), ComputedGyro, TestCase.ExpectedGyroAngle),
			FMath::IsNearlyEqual(ComputedGyro, TestCase.ExpectedGyroAngle, KINDA_SMALL_NUMBER));
	}

	return true;
}

bool FTDCTriangleDialDisplayTest::RunTest(const FString& Parameters)
{
	const float ImpactAngle = 82.0f;
	const float DeflectionAngle = ATDC::ComputeDeflectionAngleFromInputs(ImpactAngle, 22.0f, 44.0f);
	TestTrue(
		TEXT("later-model triangle dial should display impact angle"),
		FMath::IsNearlyEqual(ATDC::ComputeTriangleDialAngleFromInputs(ImpactAngle, DeflectionAngle, true), ImpactAngle, KINDA_SMALL_NUMBER));

	TestTrue(
		TEXT("early-model triangle dial should display deflection angle"),
		FMath::IsNearlyEqual(ATDC::ComputeTriangleDialAngleFromInputs(ImpactAngle, DeflectionAngle, false), DeflectionAngle, KINDA_SMALL_NUMBER));

	return true;
}

bool FTDCGyroTableTest::RunTest(const FString& Parameters)
{
	constexpr float GyroToleranceDegrees = 1.0f;
	constexpr float SpreadToleranceDegrees = 0.15f;
	constexpr float DeflectionToleranceDegrees = 2.5f;
	constexpr float AngleToleranceDegrees = 0.15f;
	constexpr float ParallaxToleranceDegrees = 0.15f;

	for (const FIntegratedTableRow& Row : IntegratedTableRows)
	{
		ATDC* TDC = NewObject<ATDC>();
		TestNotNull(*FString::Printf(TEXT("%s should create calculator instance"), *FormatRowLabel(Row.RowNumber)), TDC);
		if (TDC == nullptr)
		{
			return false;
		}

		TDC->SetParallaxMotorEnabled(false);
		TDC->bParallaxForward = Row.ParallaxCorrection >= 0.0f;
		TDC->ManualParallax = FMath::Abs(Row.ParallaxCorrection);
		TDC->SetTorpedoSpeed(Row.TorpedoSpeed);
		TDC->SetTargetSpeed(Row.TargetSpeed);
		TDC->SetDistanceToTarget(Row.DistanceHm);
		TDC->SetTargetLength(Row.TargetLengthMeters);
		TDC->SetTargetBearing(Row.TargetBearing);
		TDC->SetAngleOnTheBow(Row.SignedAngleOnTheBowDegrees);
		TDC->RecalculateSolution();

		TestTrue(
			FString::Printf(TEXT("%s angle on the bow %.2f should match row %.2f"), *FormatRowLabel(Row.RowNumber), TDC->AngleOnTheBow, Row.SignedAngleOnTheBowDegrees),
			FMath::IsNearlyEqual(TDC->AngleOnTheBow, Row.SignedAngleOnTheBowDegrees, AngleToleranceDegrees));
		TestTrue(
			FString::Printf(TEXT("%s parallax %.2f should match row %.2f"), *FormatRowLabel(Row.RowNumber), TDC->ParallaxCorrection, Row.ParallaxCorrection),
			FMath::IsNearlyEqual(TDC->ParallaxCorrection, Row.ParallaxCorrection, ParallaxToleranceDegrees));
		TestTrue(
			FString::Printf(TEXT("%s deflection %.2f should match row %.2f"), *FormatRowLabel(Row.RowNumber), TDC->DeflectionAngle, Row.ExpectedDeflectionDegrees),
			FMath::IsNearlyEqual(TDC->DeflectionAngle, Row.ExpectedDeflectionDegrees, DeflectionToleranceDegrees));
		TestTrue(
			FString::Printf(TEXT("%s gyro %.2f should match row %.2f"), *FormatRowLabel(Row.RowNumber), TDC->GyroAngle, Row.ExpectedGyroAngle),
			FMath::IsNearlyEqual(TDC->GyroAngle, Row.ExpectedGyroAngle, GyroToleranceDegrees));
		TestTrue(
			FString::Printf(TEXT("%s spread %.2f should match row %.2f"), *FormatRowLabel(Row.RowNumber), TDC->SpreadAngle, Row.ExpectedSpreadDegrees),
			FMath::IsNearlyEqual(TDC->SpreadAngle, Row.ExpectedSpreadDegrees, SpreadToleranceDegrees));
	}

	return true;
}

#endif
