#ifndef QUADRATUREENCODER_SIMULATION_DATA_GENERATOR
#define QUADRATUREENCODER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class QuadratureEncoderAnalyzerSettings;

class QuadratureEncoderSimulationDataGenerator
{
public:
	QuadratureEncoderSimulationDataGenerator();
	~QuadratureEncoderSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, QuadratureEncoderAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	QuadratureEncoderAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateQuadratureTransition( bool forward );
	U32 mTransitionCount;
	U8 mCurrentState;

	SimulationChannelDescriptor mChannelASimulationData;
	SimulationChannelDescriptor mChannelBSimulationData;
	SimulationChannelDescriptor mChannelZSimulationData;

};
#endif //QUADRATUREENCODER_SIMULATION_DATA_GENERATOR