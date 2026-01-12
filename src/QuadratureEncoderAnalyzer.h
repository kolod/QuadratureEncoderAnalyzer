#ifndef QUADRATUREENCODER_ANALYZER_H
#define QUADRATUREENCODER_ANALYZER_H

#include <Analyzer.h>
#include "QuadratureEncoderAnalyzerSettings.h"
#include "QuadratureEncoderAnalyzerResults.h"
#include "QuadratureEncoderSimulationDataGenerator.h"
#include <memory>

class ANALYZER_EXPORT QuadratureEncoderAnalyzer : public Analyzer2
{
public:
	QuadratureEncoderAnalyzer();
	virtual ~QuadratureEncoderAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	QuadratureEncoderAnalyzerSettings mSettings;
	std::unique_ptr<QuadratureEncoderAnalyzerResults> mResults;
	AnalyzerChannelData* mChannelA;
	AnalyzerChannelData* mChannelB;
	AnalyzerChannelData* mChannelZ;

	QuadratureEncoderSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Quadrature encoder analysis vars:
	U32 mSampleRateHz;
	S64 mPosition;
	U8 mLastState;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //QUADRATUREENCODER_ANALYZER_H
