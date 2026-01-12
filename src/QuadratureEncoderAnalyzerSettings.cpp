#include "QuadratureEncoderAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


QuadratureEncoderAnalyzerSettings::QuadratureEncoderAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	mBitRate( 9600 ),
	mInputChannelInterface(),
	mBitRateInterface()
{
	mInputChannelInterface.SetTitleAndTooltip( "Serial", "Standard Quadrature Encoder" );
	mInputChannelInterface.SetChannel( mInputChannel );

	mBitRateInterface.SetTitleAndTooltip( "Bit Rate (Bits/S)",  "Specify the bit rate in bits per second." );
	mBitRateInterface.SetMax( 6000000 );
	mBitRateInterface.SetMin( 1 );
	mBitRateInterface.SetInteger( mBitRate );

	AddInterface( &mInputChannelInterface );
	AddInterface( &mBitRateInterface );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Serial", false );
}

QuadratureEncoderAnalyzerSettings::~QuadratureEncoderAnalyzerSettings()
{
}

bool QuadratureEncoderAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface.GetChannel();
	mBitRate = mBitRateInterface.GetInteger();

	ClearChannels();
	AddChannel( mInputChannel, "Quadrature Encoder", true );

	return true;
}

void QuadratureEncoderAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface.SetChannel( mInputChannel );
	mBitRateInterface.SetInteger( mBitRate );
}

void QuadratureEncoderAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> mBitRate;

	ClearChannels();
	AddChannel( mInputChannel, "Quadrature Encoder", true );

	UpdateInterfacesFromSettings();
}

const char* QuadratureEncoderAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mBitRate;

	return SetReturnString( text_archive.GetString() );
}
