/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include "RzFunctions.h"
#include "RzSignalBlock.h"
#include "RzProcessorElement.h"

using std::isinf;

namespace RZ
{
	// Interface for FX core - the main part of all effects
	template<typename T>
	class FxCore : public ProcessorElement<T>
	{
	protected:
		FxCore() {}
	public:
		// Set FX params
		virtual void setParams() {}


		// Inner input buffer length
		virtual int getInputBufferLength() const
		{
			return 0;
		}


		// Inner output buffer length
		virtual int getOutputBufferLength() const
		{
			return 0;
		}


		// Initial buffer value
		virtual T getInitialBufferValue() const
		{
			return 0;
		}


		// Recount parameters
		virtual void recountParams(double) {}
	};
	//---------------------------------------------------------------------------------------------


	// Compressor FX core
	template<typename T>
	class FxCoreCompressor : public FxCore<T>
	{
	protected:
		// Gain reduction ratio
		float _ratio = 3.0f;

		// Gain reduction slope
		float _slope = 0.5f;

		// FX threshold level in dB
		float _threshold_db = -20.0f;

		// FX threshold level in log
		float _threshold = -2.0f;


		// Count output from input signal
		void _processReplacing(SignalBlock<T>& input, SignalBlock<T>& output, int length)
		{
			for (int i = 0; i < length; ++i)
			{
				// If signal is above threshold
				if (input[i] > this->_threshold)
				{
					output[i] = this->_slope * (this->_threshold - input[i]);
				}
				else
				{
					output[i] = 0;
				}
			}
		}
	public:
		// Create new compressor core
		FxCoreCompressor()
		{
		}


		// Create new compressor core
		FxCoreCompressor(double sample_rate)
		{
			this->recountParams(sample_rate);
		}


		// Create new compressor core
		FxCoreCompressor(float threshold_db, float ratio)
		{
			this->setParams(threshold_db, ratio);
		}


		// Set compressor parameters
		void setParams(double sample_rate, float threshold_db, float ratio)
		{
			this->_threshold_db = threshold_db;
			this->_ratio = ratio;

			this->recountParams(sample_rate);
		}


		// Recount parameters
		void recountParams(double) override
		{
			this->_slope = RZ::ratioToSlope(this->_ratio);
			this->_threshold = this->_threshold_db / 20;
		}
	};
	//---------------------------------------------------------------------------------------------


	// Gate FX core
	template<typename T>
	class FxCoreExpander : public FxCore<T>
	{
	protected:
		// Gain reduction ratio
		float _ratio = 0.3f;

		// Gain reduction slope
		float _slope = 2.0f;

		// FX threshold level in dB
		float _threshold_db = -20.0f;

		// FX threshold level in log
		float _threshold = -2.0f;

		// Fx range in dB
		float _range_db = -72.0f;

		// Fx range in log
		float _range = -7.2f;

		// Fx threshold bottom in log
		float _threshold_bottom = -9.2f;

		// Count output from input signal
		void _processReplacing(SignalBlock<T>& input, SignalBlock<T>& output, int length)
		{
			for (int i = 0; i < length; ++i)
			{
				// If signal is under threshold and above range
				if (input[i] < this->_threshold)
				{
					if (input[i] > this->_threshold_bottom)
					{
						output[i] = this->_slope * (this->_threshold - input[i]);
					}
					else
					{
						output[i] = this->_range;
					}
				}
				else
				{
					output[i] = 0;
				}
			}
		}
	public:
		// Create new expander
		FxCoreExpander()
		{
		}


		// Create new expander core
		FxCoreExpander(double sample_rate)
		{
			this->recountParams(sample_rate);
		}


		// Create new expander core
		FxCoreExpander(float sample_rate, float threshold_db, float ratio, float range_db = -72.0f)
		{
			this->setParams(sample_rate, threshold_db, ratio, range_db);
		}


		// Set expander parameters
		void setParams(double sample_rate, float threshold_db, float ratio, float range_db = -72.0f)
		{
			this->_threshold_db = threshold_db;
			this->_ratio = ratio;
			this->_range_db = range_db;

			this->recountParams(sample_rate);
		}


		// Recount parameters
		void recountParams(double) override
		{
			this->_slope = RZ::ratioToSlope(this->_ratio);
			this->_threshold = this->_threshold_db / 20;
			this->_range = this->_range_db / 20;
			this->_threshold_bottom = (this->_threshold - (this->_range + this->_threshold) / this->_ratio) / this->_slope - this->_range;
			
			//Check against infinity (division by zero)
			if (isinf(this->_threshold_bottom))
			{
				this->_threshold_bottom = -1000000000;
			}
		}
	};
	//---------------------------------------------------------------------------------------------
}