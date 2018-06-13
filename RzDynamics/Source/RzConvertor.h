/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include "RzFunctions.h"
#include "RzSignalBlock.h"
#include "RzProcessorElement.h"

namespace RZ
{

	// Linear to logarythmic signal convertor
	template<typename T>
	class ConvertorLinToLog : public ProcessorElement<T>
	{
	protected:
		// Count output from input signal
		void _processReplacing(SignalBlock<T>& input, SignalBlock<T>& output, int length) override
		{
			for (int i = 0; i < length; ++i)
			{
				output[i] = log10(input[i]);
			}
		}
	};
	//---------------------------------------------------------------------------------------------


	// Linear squared signal to logarythmic signal convertor
	template<typename T>
	class ConvertorLinSquareToLog : public ProcessorElement<T>
	{
	protected:
		// Count output from input signal
		void _processReplacing(SignalBlock<T>& input, SignalBlock<T>& output, int length) override
		{
			for (int i = 0; i < length; ++i)
			{
				output[i] = log10(input[i]) / 2;
			}
		}
	};
	//---------------------------------------------------------------------------------------------


	// Logarythmic to linear signal convertor
	template<typename T>
	class ConvertorLogToLin : public ProcessorElement<T>
	{
	protected:
		// Count output from input signal
		void _processReplacing(SignalBlock<T>& input, SignalBlock<T>& output, int length) override
		{
			for (int i = 0; i < length; ++i)
			{
				output[i] = pow<T>(10, input[i]);
			}
		}
	};
	//---------------------------------------------------------------------------------------------
}