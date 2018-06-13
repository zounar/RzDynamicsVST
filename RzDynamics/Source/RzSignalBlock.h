/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include <iostream>

using std::ostream;

namespace RZ
{
	/*
	Signal block class
	This class DOES NOT take ownership of the input data
	Its just a wrapper to simplify accessing and passing arrays to functions
	*/
	template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	class SignalBlock
	{
		template<typename T> friend class SignalBlockWithBuffer;

	protected:
		SignalBlock() {}

		// Signal length
		int _length;

		// Signal samples
		T* _samples;

		// If true, samples will be deleted on destructor
		bool _flush_samples = false;
	public:
		/**
			Create new signal block
			Input data are NOT copied
		*/
		SignalBlock(T* samples, int length)
		{
			this->_samples = samples;
			this->_length = length;
		}


		// Create new signal block same sized as temp block, fill with values
		SignalBlock(int length, T fill)
		{
			this->_length = length;
			this->_samples = new T[length];
			
			this->fill(fill);
		}


		// Copy instance
		SignalBlock(const SignalBlock<T>& other)
		{
			this->_length = other._length;
			this->_samples = new T[other._length];
			
			memcpy(this->_samples, other._samples, sizeof(T) * other._length);

			this->_flush_samples = true;
		}


		// Delete samples only if were copied
		~SignalBlock()
		{
			if (this->_flush_samples)
			{
				delete[] this->_samples;
			}
		}


		// Fill signal block with values
		void fill(T value)
		{
			for (int i = 0; i < this->_length; ++i)
			{
				this->_samples[i] = value;
			}
		}


		// Get block length
		int getLength() const
		{
			return this->_length;
		}


		// Get data pointer
		T* getData() const
		{
			return this->_samples;
		}


		// Get data copy
		T* getDataCopy() const
		{
			T* out = new T[this->_length];

			memcpy_s(out, sizeof(T) * this->_length, this->_samples, sizeof(T) * this->_length);

			return out;
		}


		// Get sample by index
		T& operator[] (int i)
		{
			return this->_samples[i];
		}
		

		// Write signal to output stream
		virtual void writeTo(ostream& stream, const char* separator = " ") const
		{
			for (int i = 0; i < this->_length; ++i)
			{
				stream << this->_samples[i] << separator;
			}
		}


		// Overloaded output stream operator
		friend ostream& operator << (ostream& stream, const SignalBlock<T>& item)
		{
			item.writeTo(stream);

			return stream;
		}


		// Overloaded comparison operator
		bool operator != (const SignalBlock<T>& other) const
		{
			return this->_samples != other._samples || this->_length != other._length;
		}


		// Overloaded comparison operator
		bool operator == (const SignalBlock<T>& other) const
		{
			return this->_samples == other._samples && this->_length == other._length;
		}
	};
	//---------------------------------------------------------------------------------------------
}