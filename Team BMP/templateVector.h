#pragma once

#define DEFAULT_INITIAL_SIZE		10

// the templating requires the I modify m_data, add, get, 

template <class SOMETHING>
class templateVector
{
private:
	/** where in memory the array is stored */
	SOMETHING * m_data;
	/** allocated is around double the size */
	int m_allocated;	
	/** however many elements the user wants */
	int m_size;
public:
	templateVector()
		// initializer list
		:m_data(0), m_allocated(0), m_size(0)
	{
//		m_data = 0;
//		m_allocated = 0;
//		m_size = 0;
	}

	/** @return the size of the array */
	int getSize()
	{
		return m_size;
	}

	/** 
	 * @return the value at the given index
	 */
	SOMETHING & get(int a_index)
	{
		return m_data[a_index];
	}

	/**
	 * @param a_value the value to add to the end of the vector
	 */
	void add(SOMETHING a_value)
	{
		// if the vector has not been allocated
		if(!m_data)
		{
			m_data = new SOMETHING[DEFAULT_INITIAL_SIZE];
			m_allocated = DEFAULT_INITIAL_SIZE;
		}
		if(m_size >= m_allocated)
		{
			// this is the size of the new array we want
			int newSize = m_allocated + DEFAULT_INITIAL_SIZE;
			// get that bigger block of memory
			SOMETHING * biggerBlock = new SOMETHING[newSize];
			// copy the old data into the new block
			for(int i = 0; i < m_allocated; ++i)
			{
				biggerBlock[i] = m_data[i];
			}
			// now we don't need the old block anymore...
			delete m_data;
			// replace the old block with the new
			m_data = biggerBlock;
			m_allocated = newSize;
		}
		m_data[m_size] = a_value;
		m_size++;
	}

	/** destructor callable w/o killing the data structure */
	void release()
	{
		delete m_data;
		m_data = 0;
		m_allocated = 0;
		m_size = 0;
	}

	/** destructor */
	~templateVector()
	{
		release();
	}

	/** @note very dangerous function for a vector to have. */
	void setSize(int a_size)
	{
		m_size = a_size;
	}

	/** Swap function.
	@param: int a, int b
	Swaps the two positions of a and b in the vector. **/

	void swap(int a_first, int a_second)
	{
		SOMETHING p_swapHelp;
		//make sure it can't go out of bounds.
		if(a_first > m_size || a_second > m_size || a_first < 0 || a_second < 0)
			return;
		p_swapHelp = m_data[a_first];
		m_data[a_first] = m_data[a_second];
		m_data[a_second] = p_swapHelp;
	}
};
