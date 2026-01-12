#ifndef QUADRATUREENCODER_ANALYZER_SETTINGS
#define QUADRATUREENCODER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class QuadratureEncoderAnalyzerSettings : public AnalyzerSettings
{
public:
	QuadratureEncoderAnalyzerSettings();
	virtual ~QuadratureEncoderAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	// Channels
	Channel mChannelA;
	Channel mChannelB;
	Channel mChannelZ; // Index channel (optional)

	// Inverted channels (optional)
	Channel mInvertedChannelA;
	Channel mInvertedChannelB;
	Channel mInvertedChannelZ;

protected:
	AnalyzerSettingInterfaceChannel	mChannelAInterface;
	AnalyzerSettingInterfaceChannel	mChannelBInterface;
	AnalyzerSettingInterfaceChannel	mChannelZInterface;
	AnalyzerSettingInterfaceChannel	mInvertedChannelAInterface;
	AnalyzerSettingInterfaceChannel	mInvertedChannelBInterface;
	AnalyzerSettingInterfaceChannel	mInvertedChannelZInterface;
};

#endif //QUADRATUREENCODER_ANALYZER_SETTINGS
