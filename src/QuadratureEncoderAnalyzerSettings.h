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

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	AnalyzerSettingInterfaceChannel	mInputChannelInterface;
	AnalyzerSettingInterfaceInteger	mBitRateInterface;
};

#endif //QUADRATUREENCODER_ANALYZER_SETTINGS
