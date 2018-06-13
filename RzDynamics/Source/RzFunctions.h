/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include "math.h"

namespace RZ
{
	// Convert time in seconds to time by sampling period
	inline float getTimeByPeriod(float time, double sample_rate)
	{
		return (float)(1 - exp(-2.2f / (time * sample_rate)));
	}


	// Convert time in seconds to number of samples by sample rate
	inline int getSamplesByTime(float time, double sample_rate)
	{
		return (int)(time * sample_rate);
	}


	// Convert ratio to slope
	inline float ratioToSlope(float ratio)
	{
		return 1 - (1.0f / ratio);
	}


	// Convert dB to linear
	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	T dbToLin(T value_db)
	{
		return (float)pow(10, value_db / 20);
	}


	// Convert multiple inputs to mono output
	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	void toMono(T** inputs, T* output, int length, int channel_number)
	{
		for (int sample = 0; sample < length; ++sample)
		{
			output[sample] = 0;

			for (int channel = 0; channel < channel_number; ++channel)
			{
				output[sample] += inputs[channel][sample];
			}

			output[sample] /= channel_number;
		}
	}


	/*
		Perform a VCA (Voltage controlled amplifier)
		Get multiple inputs, apply control signal, save to outputs
		Amplify by correction ( 1 means no amplification )
	*/
	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	void vca(T**inputs, T** outputs, T* control, int length, int channel_number, float correction = 1.0f)
	{
		for (int channel = 0; channel < channel_number; channel++)
		{
			for (int sample = 0; sample < length; ++sample)
			{
				outputs[channel][sample] = inputs[channel][sample] * control[sample] * correction;
			}
		}
	}
}