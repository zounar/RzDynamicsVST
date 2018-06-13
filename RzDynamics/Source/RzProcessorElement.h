/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include "assert.h"

#include "RzSignalBlock.h"
#include "RzSignalBlockWithBuffer.h"

namespace RZ
{
	// Interface for one processor element (block, that represents elementary IO device)
	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	class ProcessorElement
	{

	protected:
		ProcessorElement() {}

		// Input buffer (is initialized only if element needs buffer)
		Buffer<T>* _input_buffer = nullptr;

		// Output buffer (is initialized only if element needs buffer)
		Buffer<T>* _output_buffer = nullptr;

		// True if buffers were initialized
		bool _buffers_initialized = false;


		// Push buffer with values from input
		void _saveBuffer(SignalBlock<T>& values, Buffer<T>& buffer)
		{
			int val_length = values.getLength();

			for (int i = val_length - buffer.getLength() - 1; i < val_length; ++i)
			{
				buffer.push(values[i]);
			}
		}


		// Get output values from input
		virtual void _processReplacing(SignalBlock<T>& input, SignalBlock<T>& output, int length)
		{
			for (int i = 0; i < length; ++i)
			{
				output[i] = input[i];
			}
		}


		// Get output values from input
		virtual void _processReplacing(SignalBlockWithBuffer<T>& input, SignalBlockWithBuffer<T>& output, int length)
		{
			for (int i = 0; i < length; ++i)
			{
				output[i] = input[i];
			}
		}
	public:
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


		// Count output from input signal
		void countOutput(SignalBlock<T>* input, SignalBlock<T>* output)
		{
			//Test NULL pointer
			if (input == nullptr || output == nullptr)
			{
				return;
			}

			//Copy input to temp if equals
			SignalBlock<T>* temp = nullptr;
			if (input == output)
			{
				temp = new SignalBlock<T>(*input);
				input = temp;
			}

			//Get shorter length from input&output
			int length = input->getLength() > output->getLength() ? output->getLength() : input->getLength();
			
			if (0 == this->getInputBufferLength() && 00 == this->getOutputBufferLength())
			{
				//Count output (input, output, length)
				this->_processReplacing(*input, *output, length);
			}
			else
			{
				if (nullptr == this->_input_buffer)
				{
					//Create input buffer if not exists
					this->_input_buffer = new Buffer<T>(this->getInputBufferLength(), this->getInitialBufferValue());

					//Create output buffer if not exists
					this->_output_buffer = new Buffer<T>(this->getOutputBufferLength(), this->getInitialBufferValue());
				}
				
				//Prepend input with buffer
				SignalBlockWithBuffer<T>* input_buffered = new SignalBlockWithBuffer<T>(*input, this->_input_buffer);
				//Prepend output with buffer
				SignalBlockWithBuffer<T>* output_buffered = new SignalBlockWithBuffer<T>(*output, this->_output_buffer);
				
				//Count output (input, output, length)
				this->_processReplacing(*input_buffered, *output_buffered, length);
				
				//Save input buffer
				this->_saveBuffer(*input_buffered, *this->_input_buffer);
				//Save output buffer
				this->_saveBuffer(*output_buffered, *this->_output_buffer);

				//Delete spare instances
				delete input_buffered;
				delete output_buffered;
			}

			if (temp != nullptr)
			{
				delete temp;
			}
		}


		// Count output from input signal, replacing input signal
		void countOutput(SignalBlock<T>* input_output)
		{
			this->countOutput(input_output, input_output);
		}


		// Get output from input signal
		SignalBlock<T>& countAndReturnOutput(SignalBlock<T>* input)
		{
			SignalBlock<T>* output = new SignalBlock<T>(*input);

			this->countOutput(input, output);

			return *output;
		}
	};
	//---------------------------------------------------------------------------------------------
}