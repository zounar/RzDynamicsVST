/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include "RzFunctions.h"
#include "RzSignalBlock.h"
#include "RzSignalBlockWithBuffer.h"
#include "RzProcessorElement.h"

namespace RZ
{
	// Abstract class for dynamic filter
	template<typename T>
	class DynamicFilter : public ProcessorElement<T>
	{
	protected:
		DynamicFilter() {}
	public:
		// Set time constants, eg. attack, hold, release
		virtual void setTimeConstants() {}


		// Recount time constants (used to count time-by-period constants)
		virtual void recountTimeConstants(double) {}
	};
	//---------------------------------------------------------------------------------------------


	// Dynamic filter using attack and release constants
	template<typename T>
	class DynamicFilterAR : public DynamicFilter<T>
	{
	protected:
		// Attack time constant in seconds
		float _attack = 1.0f;

		// Attack time constant by period
		float _attackByPeriod = 0.5f;

		// Release time constant in seconds
		float _release = 1.0f;

		// Release time constant by period
		float _releaseByPeriod = 0.5f;

		// If true, this filter is used to filter compressor gain reduction signal
		bool _is_compressor_filter = false;


		// Count output from input signal
		void _processReplacing(SignalBlockWithBuffer<T>& input, SignalBlockWithBuffer<T>& output, int length) override
		{
			// If filter is used to filter compressor gain reduction signal, attack and release are swapped
			if (this->_is_compressor_filter)
			{
				//For each input sample
				for (int i = 0; i < length; ++i)
				{
					//If envelope sample is lower than previous output (gain attacks when signal lowers!)
					if (input[i] < output[i - 1])
					{
						//Signal is attacking
						output[i] = (1 - this->_attackByPeriod) * output[i - 1] + this->_attackByPeriod * input[i];
					}
					else
					{
						//Signal is releasing
						output[i] = (1 - this->_releaseByPeriod) * output[i - 1] + this->_releaseByPeriod * input[i];
					}
				}
			}
			else
			{
				//For each input sample
				for (int i = 0; i < length; ++i)
				{
					//If envelope sample is lower than previous output
					if (input[i] > output[i - 1])
					{
						//Signal is attacking
						output[i] = (1 - this->_attackByPeriod) * output[i - 1] + this->_attackByPeriod * input[i];
					}
					else
					{
						//Signal is releasing
						output[i] = (1 - this->_releaseByPeriod) * output[i - 1] + this->_releaseByPeriod * input[i];
					}
				}
			}
		}
	public:
		// Inner output buffer length
		int getOutputBufferLength() const override
		{
			return 1;
		}


		// Initial buffer value
		virtual T getInitialBufferValue() const override
		{
			return 1;
		}


		// Create AR dynamic filter
		DynamicFilterAR(bool is_compressor_filter = false) : _is_compressor_filter(is_compressor_filter)
		{
		}


		// Create AR dynamic filter
		DynamicFilterAR(double sample_rate, bool is_compressor_filter = false) : _is_compressor_filter(is_compressor_filter)
		{
			this->recountTimeConstants(sample_rate);
		}


		// Create new AR dynamic filter
		DynamicFilterAR(double sample_rate, float attack, float release, bool is_compressor_filter = false) 
			: _is_compressor_filter(is_compressor_filter)
		{
			this->setTimeConstants(attack, release);
		}


		// IF set to true, filter will behave as is filtering compressor gain reduction signal
		void setIsCompressorFilter(bool is_compressor_filter)
		{
			this->_is_compressor_filter = is_compressor_filter;
		}


		// Set time constants
		void setTimeConstants(double sample_rate, float attack, float release)
		{
			this->_attack = attack;
			this->_release = release;

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


	// Dynamic filter using attack, release and hold constants
	template<typename T>
	class DynamicFilterARH : public DynamicFilter<T>
	{
	protected:
		// Attack time constant in seconds
		float _attack = 1.0f;

		// Attack time constant by period
		float _attackByPeriod = 0.5f;

		// Release time constant in seconds
		float _release = 1.0f;

		// Release time constant by period
		float _releaseByPeriod = 0.5f;

		// Hold time constant in seconds
		float _hold = 1.0f;

		// Hold time by sample number
		int _holdForSamples = 0;

		// Number of samples when in hold time
		int _under_cnt = 0;

		// If true, this filter is used to filter compressor gain reduction signal
		bool _is_compressor_filter = false;


		// Count output from input signal
		void _processReplacing(SignalBlockWithBuffer<T>& input, SignalBlockWithBuffer<T>& output, int length) override
		{
			// If filter is used to filter compressor gain reduction signal
			if (this->_is_compressor_filter)
			{
				//For each input sample
				for (int i = 0; i < length; ++i)
				{
					//Signal is attacking (compressor - signal attacks when lowers)
					if (input[i] <= output[i - 1])
					{
						output[i] = (1 - this->_attackByPeriod) * output[i - 1] + this->_attackByPeriod * input[i];
						this->_under_cnt = 0;
					}
					//Signal is releasing
					else
					{
						if (this->_under_cnt < this->_holdForSamples)
						{
							output[i] = output[i - 1];
							this->_under_cnt++;
						}
						else
						{
							output[i] = (1 - this->_releaseByPeriod) * output[i - 1] + this->_releaseByPeriod * input[i];
						}
					}
				}
			}
			else
			{
				//For each input sample
				for (int i = 0; i < length; ++i)
				{
					//Signal is attacking
					if (input[i] >= output[i - 1])
					{
						output[i] = (1 - this->_attackByPeriod) * output[i - 1] + this->_attackByPeriod * input[i];
						this->_under_cnt = 0;
					}
					else
					{
						// If expander/gate was oppened and still in hold
						if (output[i - 1] > 0.982 && this->_under_cnt < this->_holdForSamples)
						{
							output[i] = output[i - 1];
							this->_under_cnt++;
						}
						//In release
						else
						{
							output[i] = (1 - this->_releaseByPeriod) * output[i - 1] + this->_releaseByPeriod * input[i];
						}
					}
				}
			}
		}
	public:
		// Inner output buffer length
		int getOutputBufferLength() const override
		{
			return 1;
		}


		// Initial buffer value
		virtual T getInitialBufferValue() const override
		{
			return 1;
		}


		// Create ARH dynamic filter
		DynamicFilterARH(bool is_compressor_filter = false) : _is_compressor_filter(is_compressor_filter)
		{
		}


		// Create ARH dynamic filter
		DynamicFilterARH(double sample_rate, bool is_compressor_filter = false) : _is_compressor_filter(is_compressor_filter)
		{
			this->recountTimeConstants(sample_rate);
		}


		// Create new ARH dynamic filter
		DynamicFilterARH(double sample_rate, float attack, float release, float hold, bool is_compressor_filter = false) 
			: _is_compressor_filter(is_compressor_filter)
		{
			this->setTimeConstants(sample_rate, attack, release, hold);
		}


		// IF set to true, filter will behave as is filtering compressor gain reduction signal
		void setIsCompressorFilter(bool is_compressor_filter)
		{
			this->_is_compressor_filter = is_compressor_filter;
		}


		// Set time constants
		void setTimeConstants(double sample_rate, float attack, float release, float hold)
		{
			this->_attack = attack;
			this->_release = release;
			this->_hold = hold;

			this->recountTimeConstants(sample_rate);
		}


		// Recount time constants
		void recountTimeConstants(double sample_rate) override
		{
			this->_attackByPeriod = getTimeByPeriod(this->_attack, sample_rate);
			this->_releaseByPeriod = getTimeByPeriod(this->_release, sample_rate);
			this->_holdForSamples = getSamplesByTime(this->_hold, sample_rate);
		}
	};
	//---------------------------------------------------------------------------------------------
}