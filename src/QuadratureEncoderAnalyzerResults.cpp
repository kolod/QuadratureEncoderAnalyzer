#include "QuadratureEncoderAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "QuadratureEncoderAnalyzer.h"
#include "QuadratureEncoderAnalyzerSettings.h"
#include <iostream>
#include <fstream>

QuadratureEncoderAnalyzerResults::QuadratureEncoderAnalyzerResults( QuadratureEncoderAnalyzer* analyzer, QuadratureEncoderAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

QuadratureEncoderAnalyzerResults::~QuadratureEncoderAnalyzerResults()
{
}

void QuadratureEncoderAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char position_str[128];
	char direction_str[16];
	char combined_str[256];

	// frame.mData1 = position, frame.mData2 = direction
	S64 position = (S64)frame.mData1;
	S8 direction = (S8)frame.mData2;

	snprintf( position_str, sizeof(position_str), "%lld", position );
	snprintf( direction_str, sizeof(direction_str), "%s", direction > 0 ? "+" : "-" );
	snprintf( combined_str, sizeof(combined_str), "%s%lld", direction > 0 ? "+" : "-", position );

	AddResultString( direction_str );
	AddResultString( position_str );
	AddResultString( combined_str );
}

void QuadratureEncoderAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Position,Direction" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		S64 position = (S64)frame.mData1;
		S8 direction = (S8)frame.mData2;

		file_stream << time_str << "," << position << "," << (direction > 0 ? "Forward" : "Backward") << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void QuadratureEncoderAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	S64 position = (S64)frame.mData1;
	S8 direction = (S8)frame.mData2;

	char position_str[128];
	snprintf( position_str, sizeof(position_str), "Pos: %lld, Dir: %s", position, direction > 0 ? "Fwd" : "Bwd" );
	AddTabularText( position_str );
#endif
}

void QuadratureEncoderAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void QuadratureEncoderAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}