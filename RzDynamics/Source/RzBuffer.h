/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*/

#pragma once

#include <iostream>

using std::ostream;

namespace RZ
{
	// FIFO like buffer with ability to access all elements like in array
	template<typename T> 
	class Buffer
	{
	protected:
		// Array of all buffered values
		T* _values;

		// Buffer length
		const int _length;

		// Fill value
		T _fill;
	public:
		// Create new buffer with positive length and fill with values
		Buffer(int length, T fill = NULL) : _length(length)
		{
			this->_fill = fill;
			this->_values = new T[length];

			this->fill(fill);
		}


		// Create buffer copy
		Buffer(const Buffer<T>& other) : _length(other._length)
		{
			other._fill = this->_fill;
			other._values = new T[this->_length];

			memcpy(other._values, this->_values, sizeof(T) * this->_length);
		}


		// Destructor
		~Buffer()
		{
			delete[] this->_values;
		}


		// Fill buffer with values
		void fill(T value)
		{
			for (int i = 0; i < this->_length; ++i)
			{
				this->_values[i] = value;
			}
		}


		/*
			Push buffer from the beginning
			(shifts all values, removes last element)
		*/
		void push(T value)
		{
			if (this->_length > 1)
			{
				memmove_s(this->_values + 1, sizeof(T) * this->_length - 1, this->_values, sizeof(T) * this->_length - 1);
			}

			this->_values[0] = value;
		}


		// Get buffer length
		int getLength() const
		{
			return this->_length;
		}


		/*
			Get value copy by index
			Returns fill value (or NULL) if out of bounds
		*/
		T get(int index) const
		{
			if (index < this->_length)
			{
				return this->_values[index];
			}
			else
			{
				return this->_fill;
			}
		}


		/*
			Get value reference by index
			Returns fill value (or NULL) if out of bounds
		*/
		T& operator[](int index)
		{
			if (index < this->_length)
			{
				return this->_values[index];
			}
			else
			{
				return this->_fill;
			}
		}


		// Write buffer to output stream
		void writeTo(ostream& stream, const char* separator = " ") const
		{
			for (int i = 0; i < this->_length; ++i)
			{
				stream << this->_values[i] << separator ;
			}
		}


		// Overloaded output stream operator
		template<typename T>
		friend ostream& operator << (ostream& stream, const Buffer<T>& item)
		{
			item.writeTo(stream);

			return stream;
		}
	};
};