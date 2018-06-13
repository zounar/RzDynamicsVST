/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include <iostream>

#include "RzSignalBlock.h"
#include "RzBuffer.h"

using std::ostream;

namespace RZ
{
	/*
		Signal block with buffer class
		This class DOES NOT take ownership of the input data
		Its just a wrapper to simplify accessing and passing arrays to functions
	*/
	template<typename T>
	class SignalBlockWithBuffer : public SignalBlock<T>
	{
	protected:
		// Buffer
		Buffer<T>* _buffer;
	public:
		/**
			Create new signal block with buffer
			Input data are NOT copied
		*/
		SignalBlockWithBuffer(T* samples, int length, Buffer<T>* buffer) : SignalBlock(samples, length)
		{
			this->_buffer = buffer;
		}


		// Create new signal block same sized as temp block, fill with values
		SignalBlockWithBuffer(const SignalBlock<T>& temp, Buffer<T>* buffer)
		{
			this->_length = temp._length;
			this->_samples = temp._samples;
			this->_buffer = buffer;
		}


		// Create new signal block same sized as temp block, fill with values
		SignalBlockWithBuffer(const SignalBlock<T>& temp, T fill, Buffer<T>* buffer)
		{
			this->_length = temp._length;
			this->_samples = new T[temp._length];
			this->_buffer = buffer;

			this->fill(fill);
		}


		// Get sample by index
		T& operator[] (int i)
		{
			if (i < 0)
			{
				return (*this->_buffer)[i + 1];
			}
			else
			{
				return this->_samples[i];
			}	
		}


		// Write signal to output stream
		void writeTo(ostream& stream, const char* separator = " ") const
		{
			if (this->_buffer->getLength() > 0)
			{
				stream << *this->_buffer << ";" << separator;
			}

			for (int i = 0; i < this->_length; ++i)
			{
				stream << this->_samples[i] << separator;
			}
		}
	};
}