#include "RobCoG.h"
#include "RPid.h"

// Constructor
FRPid::FRPid(float _P, float _I, float _D,
	float _OutMax, float _OutMin) :
	P(_P), I(_I), D(_D), OutMax(_OutMax), OutMin(_OutMin)
{
	IErr = 0.0f;
	PrevErr = 0.0f;
}

// Destructor
FRPid::~FRPid()
{
}

// Init
void FRPid::Init(float _P, float _I, float _D,
	float _OutMax, float _OutMin)
{
	P = _P;
	I = _I;
	D = _D;
	OutMax = _OutMax;
	OutMin = _OutMin;

	FRPid::Reset();
}

// Update
float FRPid::Update(const float Error, const float DeltaT)
{
	if (DeltaT == 0.0f || FMath::IsNaN(Error))
	{
		return 0.0f;
	}

	// Calculate proportional output
	const float POut = P * Error;

	// Calculate integral error / output
	IErr += DeltaT * Error;
	const float IOut = I * IErr;

	// Calculate the derivative error / output
	const float DErr = (Error - PrevErr) / DeltaT;
	const float DOut = D * DErr;

	// Set previous error
	PrevErr = Error;

	// Calculate the output
	const float Out = POut + IOut + DOut;

	// Clamp output to max/min values
	if (Out > OutMax)
		return OutMax;
	else if (Out < OutMin)
		return OutMin;

	return Out;
}

// Reset
void FRPid::Reset()
{
	PrevErr = 0.0f;
	IErr = 0.0f;
}