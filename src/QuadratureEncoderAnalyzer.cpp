#include "QuadratureEncoderAnalyzer.h"
#include "QuadratureEncoderAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

QuadratureEncoderAnalyzer::QuadratureEncoderAnalyzer()
:	Analyzer2(),  
	mSettings(),
	mSimulationInitilized( false ),
	mPosition( 0 ),
	mLastState( 0 )
{
	SetAnalyzerSettings( &mSettings );
}

QuadratureEncoderAnalyzer::~QuadratureEncoderAnalyzer()
{
	KillThread();
}

void QuadratureEncoderAnalyzer::SetupResults()
{
	// SetupResults is called each time the analyzer is run. Because the same instance can be used for multiple runs, we need to clear the results each time.
	mResults.reset(new QuadratureEncoderAnalyzerResults( this, &mSettings ));
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings.mChannelA );
	mResults->AddChannelBubblesWillAppearOn( mSettings.mChannelB );
	if( mSettings.mChannelZ != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings.mChannelZ );
}

void QuadratureEncoderAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mChannelA = GetAnalyzerChannelData( mSettings.mChannelA );
	mChannelB = GetAnalyzerChannelData( mSettings.mChannelB );
	if( mSettings.mChannelZ != UNDEFINED_CHANNEL )
		mChannelZ = GetAnalyzerChannelData( mSettings.mChannelZ );

	// Initialize state based on current channel values
	mLastState = ( mChannelA->GetBitState() == BIT_HIGH ? 0x02 : 0 ) | ( mChannelB->GetBitState() == BIT_HIGH ? 0x01 : 0 );
	mPosition = 0;

	for( ; ; )
	{
		// Wait for any edge on channel A or B
		U64 channelA_next = mChannelA->GetSampleOfNextEdge();
		U64 channelB_next = mChannelB->GetSampleOfNextEdge();
		U64 next_edge;

		if( channelA_next < channelB_next )
		{
			next_edge = channelA_next;
			mChannelA->AdvanceToAbsPosition( next_edge );
			mChannelB->AdvanceToAbsPosition( next_edge );
		}
		else
		{
			next_edge = channelB_next;
			mChannelA->AdvanceToAbsPosition( next_edge );
			mChannelB->AdvanceToAbsPosition( next_edge );
		}

		// Read current state
		U8 current_state = ( mChannelA->GetBitState() == BIT_HIGH ? 0x02 : 0 ) | ( mChannelB->GetBitState() == BIT_HIGH ? 0x01 : 0 );

		// Quadrature state machine: Gray code sequence
		// Forward:  00 -> 01 -> 11 -> 10 -> 00
		// Backward: 00 -> 10 -> 11 -> 01 -> 00
		S8 direction = 0;
		U8 transition = (mLastState << 2) | current_state;

		switch( transition )
		{
			case 0x01: case 0x07: case 0x0E: case 0x08: // Forward transitions
				direction = 1;
				mPosition++;
				break;
			case 0x02: case 0x0B: case 0x0D: case 0x04: // Backward transitions
				direction = -1;
				mPosition--;
				break;
			default:
				// Invalid transition or no change
				direction = 0;
				break;
		}

		if( direction != 0 )
		{
			// Create a frame for this position change
			Frame frame;
			frame.mData1 = mPosition;
			frame.mData2 = direction;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = next_edge;
			frame.mEndingSampleInclusive = next_edge;

			mResults->AddMarker( next_edge, direction > 0 ? AnalyzerResults::UpArrow : AnalyzerResults::DownArrow, mSettings.mChannelA );
			mResults->AddFrame( frame );
			mResults->CommitResults();
			ReportProgress( frame.mEndingSampleInclusive );
		}

		mLastState = current_state;
	}
}

bool QuadratureEncoderAnalyzer::NeedsRerun()
{
	return false;
}

U32 QuadratureEncoderAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), &mSettings );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 QuadratureEncoderAnalyzer::GetMinimumSampleRateHz()
{
	// For reliable quadrature decoding, we need at least 4x the maximum expected edge rate
	return 10000; // 10 kHz minimum sample rate
}

const char* QuadratureEncoderAnalyzer::GetAnalyzerName() const
{
	return "Quadrature Encoder";
}

const char* GetAnalyzerName()
{
	return "Quadrature Encoder";
}

Analyzer* CreateAnalyzer()
{
	return new QuadratureEncoderAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}