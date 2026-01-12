#include "QuadratureEncoderSimulationDataGenerator.h"
#include "QuadratureEncoderAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

QuadratureEncoderSimulationDataGenerator::QuadratureEncoderSimulationDataGenerator()
:	mTransitionCount( 0 ),
	mCurrentState( 0 )
{
}

QuadratureEncoderSimulationDataGenerator::~QuadratureEncoderSimulationDataGenerator()
{
}

void QuadratureEncoderSimulationDataGenerator::Initialize( U32 simulation_sample_rate, QuadratureEncoderAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mChannelASimulationData.SetChannel( mSettings->mChannelA );
	mChannelASimulationData.SetSampleRate( simulation_sample_rate );
	mChannelASimulationData.SetInitialBitState( BIT_LOW );

	mChannelBSimulationData.SetChannel( mSettings->mChannelB );
	mChannelBSimulationData.SetSampleRate( simulation_sample_rate );
	mChannelBSimulationData.SetInitialBitState( BIT_LOW );

	if( mSettings->mChannelZ != UNDEFINED_CHANNEL )
	{
		mChannelZSimulationData.SetChannel( mSettings->mChannelZ );
		mChannelZSimulationData.SetSampleRate( simulation_sample_rate );
		mChannelZSimulationData.SetInitialBitState( BIT_LOW );
	}

	mCurrentState = 0; // Start at state 00
}

U32 QuadratureEncoderSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mChannelASimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		// Alternate between forward and backward movement
		bool forward = (mTransitionCount / 20) % 2 == 0;
		CreateQuadratureTransition( forward );
		mTransitionCount++;
	}

	simulation_channels[0] = &mChannelASimulationData;
	simulation_channels[1] = &mChannelBSimulationData;

	if( mSettings->mChannelZ != UNDEFINED_CHANNEL )
	{
		simulation_channels[2] = &mChannelZSimulationData;
		return 3;
	}

	return 2;
}

void QuadratureEncoderSimulationDataGenerator::CreateQuadratureTransition( bool forward )
{
	// Generate quadrature encoder signals
	// Forward sequence:  00 -> 01 -> 11 -> 10 -> 00 (Gray code)
	// Backward sequence: 00 -> 10 -> 11 -> 01 -> 00

	U32 samples_per_transition = mSimulationSampleRateHz / 1000; // 1 kHz transitions

	// Advance time
	mChannelASimulationData.Advance( samples_per_transition );
	mChannelBSimulationData.Advance( samples_per_transition );
	if( mSettings->mChannelZ != UNDEFINED_CHANNEL )
		mChannelZSimulationData.Advance( samples_per_transition );

	// Update state based on direction
	if( forward )
	{
		// Forward: 00 -> 01 -> 11 -> 10 -> 00
		mCurrentState = (mCurrentState + 1) & 0x03;
		if( mCurrentState == 0 ) // State 00, 01, 10, 11
		{
			switch( (mTransitionCount >> 2) % 4 )
			{
				case 0: mCurrentState = 0; break; // 00
				case 1: mCurrentState = 1; break; // 01
				case 2: mCurrentState = 3; break; // 11
				case 3: mCurrentState = 2; break; // 10
			}
		}
	}
	else
	{
		// Backward: 00 -> 10 -> 11 -> 01 -> 00
		switch( mCurrentState )
		{
			case 0: mCurrentState = 2; break; // 00 -> 10
			case 2: mCurrentState = 3; break; // 10 -> 11
			case 3: mCurrentState = 1; break; // 11 -> 01
			case 1: mCurrentState = 0; break; // 01 -> 00
		}
	}

	// Set channel states
	BitState stateA = (mCurrentState & 0x02) ? BIT_HIGH : BIT_LOW;
	BitState stateB = (mCurrentState & 0x01) ? BIT_HIGH : BIT_LOW;

	mChannelASimulationData.TransitionIfNeeded( stateA );
	mChannelBSimulationData.TransitionIfNeeded( stateB );

	// Generate index pulse every revolution (e.g., every 100 transitions)
	if( mSettings->mChannelZ != UNDEFINED_CHANNEL )
	{
		if( mTransitionCount % 100 == 0 )
		{
			mChannelZSimulationData.Transition();
			mChannelZSimulationData.Advance( samples_per_transition / 10 );
			mChannelZSimulationData.Transition();
		}
	}
}
