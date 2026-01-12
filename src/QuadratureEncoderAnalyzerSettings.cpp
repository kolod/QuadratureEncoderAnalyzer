#include "QuadratureEncoderAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


QuadratureEncoderAnalyzerSettings::QuadratureEncoderAnalyzerSettings()
:	mChannelA( UNDEFINED_CHANNEL ),
	mChannelB( UNDEFINED_CHANNEL ),
	mChannelZ( UNDEFINED_CHANNEL ),
	mInvertedChannelA( UNDEFINED_CHANNEL ),
	mInvertedChannelB( UNDEFINED_CHANNEL ),
	mInvertedChannelZ( UNDEFINED_CHANNEL ),
	mChannelAInterface(),
	mChannelBInterface(),
	mChannelZInterface(),
	mInvertedChannelAInterface(),
	mInvertedChannelBInterface(),
	mInvertedChannelZInterface()
{
	mChannelAInterface.SetTitleAndTooltip( "Channel A", "Quadrature Encoder Channel A" );
	mChannelAInterface.SetChannel( mChannelA );

	mChannelBInterface.SetTitleAndTooltip( "Channel B", "Quadrature Encoder Channel B" );
	mChannelBInterface.SetChannel( mChannelB );

	mChannelZInterface.SetTitleAndTooltip( "Channel Z", "Quadrature Encoder Index Channel (optional)" );
	mChannelZInterface.SetChannel( mChannelZ );

	mInvertedChannelAInterface.SetTitleAndTooltip( "Inverted Channel A", "Inverted Quadrature Encoder Channel A (optional)" );
	mInvertedChannelAInterface.SetChannel( mInvertedChannelA );

	mInvertedChannelBInterface.SetTitleAndTooltip( "Inverted Channel B", "Inverted Quadrature Encoder Channel B (optional)" );
	mInvertedChannelBInterface.SetChannel( mInvertedChannelB );

	mInvertedChannelZInterface.SetTitleAndTooltip( "Inverted Channel Z", "Inverted Quadrature Encoder Index Channel (optional)" );
	mInvertedChannelZInterface.SetChannel( mInvertedChannelZ );

	AddInterface( &mChannelAInterface );
	AddInterface( &mChannelBInterface );
	AddInterface( &mChannelZInterface );
	AddInterface( &mInvertedChannelAInterface );
	AddInterface( &mInvertedChannelBInterface );
	AddInterface( &mInvertedChannelZInterface );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mChannelA, "Channel A", false );
	AddChannel( mChannelB, "Channel B", false );
	AddChannel( mChannelZ, "Channel Z", false );
	AddChannel( mInvertedChannelA, "Inverted Channel A", false );
	AddChannel( mInvertedChannelB, "Inverted Channel B", false );
	AddChannel( mInvertedChannelZ, "Inverted Channel Z", false );
}

QuadratureEncoderAnalyzerSettings::~QuadratureEncoderAnalyzerSettings()
{
}

bool QuadratureEncoderAnalyzerSettings::SetSettingsFromInterfaces()
{
	mChannelA = mChannelAInterface.GetChannel();
	mChannelB = mChannelBInterface.GetChannel();
	mChannelZ = mChannelZInterface.GetChannel();
	mInvertedChannelA = mInvertedChannelAInterface.GetChannel();
	mInvertedChannelB = mInvertedChannelBInterface.GetChannel();
	mInvertedChannelZ = mInvertedChannelZInterface.GetChannel();

	ClearChannels();
	AddChannel( mChannelA, "Channel A", true );
	AddChannel( mChannelB, "Channel B", true );
	AddChannel( mChannelZ, "Channel Z", true );
	AddChannel( mInvertedChannelA, "Inverted Channel A", true );
	AddChannel( mInvertedChannelB, "Inverted Channel B", true );
	AddChannel( mInvertedChannelZ, "Inverted Channel Z", true );

	return true;
}

void QuadratureEncoderAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mChannelAInterface.SetChannel( mChannelA );
	mChannelBInterface.SetChannel( mChannelB );
	mChannelZInterface.SetChannel( mChannelZ );
	mInvertedChannelAInterface.SetChannel( mInvertedChannelA );
	mInvertedChannelBInterface.SetChannel( mInvertedChannelB );
	mInvertedChannelZInterface.SetChannel( mInvertedChannelZ );
}

void QuadratureEncoderAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mChannelA;
	text_archive >> mChannelB;
	text_archive >> mChannelZ;
	text_archive >> mInvertedChannelA;
	text_archive >> mInvertedChannelB;
	text_archive >> mInvertedChannelZ;

	ClearChannels();
	AddChannel( mChannelA, "Channel A", true );
	AddChannel( mChannelB, "Channel B", true );
	AddChannel( mChannelZ, "Channel Z", true );
	AddChannel( mInvertedChannelA, "Inverted Channel A", true );
	AddChannel( mInvertedChannelB, "Inverted Channel B", true );
	AddChannel( mInvertedChannelZ, "Inverted Channel Z", true );

	UpdateInterfacesFromSettings();
}

const char* QuadratureEncoderAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mChannelA;
	text_archive << mChannelB;
	text_archive << mChannelZ;
	text_archive << mInvertedChannelA;
	text_archive << mInvertedChannelB;
	text_archive << mInvertedChannelZ;

	return SetReturnString( text_archive.GetString() );
}
