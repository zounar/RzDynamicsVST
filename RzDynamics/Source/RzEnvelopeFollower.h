/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include "RzFunctions.h"
#include "RzSignalBlock.h"
#include "RzSignalBlockWithBuffer.h"
#include "RzProcessorElement.h"

using std::abs;

namespace RZ
{
	// Abstract class for envelope follower
	template<typename T>
	class EnvelopeFollower : public ProcessorElement<T>
	{
	protected:
		EnvelopeFollower() {}
	public:
		// Set time constants
		virtual void setTimeConstants() {}


		// Inner input buffer length
		virtual int getInputBufferLength() const override
		{
			return 0;
		}


		// Inner output buffer length
		virtual int getOutputBufferLength() const override
		{
			return 0;
		}


		// Initial buffer value
		virtual T getInitialBufferValue() const override
		{
			return 0;
		}


		// Recount time constants (used to count time-by-period constants)
		virtual void recountTimeConstants(double) {}
	};
	//---------------------------------------------------------------------------------------------


	/**
	*	RMS envelope follower
	*/
	template<typename T>
	class EnvelopeFollowerRms : public EnvelopeFollower<T>
	{
	protected:
		// Averaging time constant in seconds
		float _averaging = 1.0f;

		// Averaging time constant by period
		float _averagingByPeriod = 0.5f;


		// Count output from input signal
		void _processReplacing(SignalBlockWithBuffer<T>& input, SignalBlockWithBuffer<T>& output, int length) override
		{
			//For each input sample
			for (int i = 0; i < length; ++i)
			{
				output[i] = this->_averagingByPeriod * input[i] * input[i] + (1 - this->_averagingByPeriod) * output[i - 1];
			}
		}
	public:
		// Inner output buffer length
		int getOutputBufferLength() const override
		{
			return 1;
		}


		// Create new RMS envelope follower
		EnvelopeFollowerRms()
		{
		}


		// Create new RMS envelope follower
		EnvelopeFollowerRms(double sample_rate, float averaging = 1)
		{
			this->setTimeConstants(sample_rate, averaging);
		}


		// Set averaging time constant
		void setTimeConstants(double sample_rate, float averaging)
		{
			this->_averaging = averaging;

			this->recountTimeConstants(sample_rate);
		}


		// Recount time constants
		void recountTimeConstants(double sample_rate) override
		{
			this->_averagingByPeriod = getTimeByPeriod(this->_averaging, sample_rate);
		}
	};
	//---------------------------------------------------------------------------------------------


	// Peak envelope follower
	template<typename T>
	class EnvelopeFollowerPeak : public EnvelopeFollower<T>
	{
		// Attack time constant in seconds
		float _attack = 1.0f;

		// Attack time constant by period
		float _attackByPeriod = 0.5f;

		// Release time constant in seconds
		float _release = 1.0f;

		// Release time constant by period
		float _releaseByPeriod = 0.5f;

		// Correction gain
		float _correction_gain = 1;


		// Count output from input signal
		void _processReplacing(SignalBlockWithBuffer<T>& input, SignalBlockWithBuffer<T>& output, int length) override
		{
			// For each output - gain correction and rectlify
			for (int i = 0; i < length; ++i)
			{
				input[i] = abs(input[i] * this->_correction_gain);
			}

			//For each input
			for (int i = 0; i < length; ++i)
			{
				//If signal is above the previous envelope sample level
				if (input[i] > output[i - 1])
				{
					output[i] = this->_attackByPeriod * input[i] + (1 - this->_attackByPeriod - this->_releaseByPeriod) * output[i - 1];
				}
				else
				{
					output[i] = (1 - this->_releaseByPeriod) * output[i - 1];
				}
			}
		}
	public:
		// Inner output buffer length
		int getOutputBufferLength() const override
		{
			return 1;
		}


		// Create new peak envelope follower
		EnvelopeFollowerPeak()
		{
		}


		// Create new peak envelope follower
		EnvelopeFollowerPeak(double sample_rate) 
		{
			this->recountTimeConstants(sample_rate);
		}


		// Create new peak envelope follower
		EnvelopeFollowerPeak(double sample_rate, float attack, float release, float correction_gain_db = 0)
		{
			this->setTimeConstants(sample_rate, attack, release, correction_gain);
		}


		// Set time constants
		void setTimeConstants(double sample_rate, float attack, float release, float correction_gain_db = 0)
		{
			this->_attack = attack;
			this->_release = release;
			this->_correction_gain = dbToLin(correction_gain_db);

			this->recountTimeConstants(sample_rate);
		}


		// Recount time constants
		void recountTimeConstants(double sample_rate) override
		{
			this->_attackByPeriod = getTimeByPeriod(this->_attack, sample_rate);
			this->_releaseByPeriod = getTimeByPeriod(this->_release, sample_rate);
		}
	};
	//---------------------------------------------------------------------------------------------
}