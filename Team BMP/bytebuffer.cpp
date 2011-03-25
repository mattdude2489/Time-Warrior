#include "bytebuffer.h"

#pragma comment(lib, "ws2_32.lib")	// includes the compiled WinSock2 code

//unsigned char * ByteBuffer::getBytes()
//{
//	return m_bytes;
//}
//unsigned char * ByteBuffer::getLastByte()
//{
//	return m_bytes+m_size;
//}
//unsigned char & ByteBuffer::get(const int & a_index)
//{
//	return m_bytes[a_index];
//}
//ByteBuffer::ByteBuffer():m_bytes(0),m_size(0),m_allocated(0){}

void ByteBuffer::reset()
{
	m_size = 0;
}

void ByteBuffer::clean()
{
	for(int i = 0; i < m_allocated; ++i)
	{
		m_data[i] = 0;
	}
	m_size = 0;
}

//void ByteBuffer::release()
//{
//	if(m_bytes)
//	{
//		delete m_bytes;
//	}
//	m_bytes = 0;
//	m_allocated = 0;
//	m_size = 0;
//}

//bool ByteBuffer::ensureCapacity(const int & a_byteCount)
//{
//	if(a_byteCount > m_allocated)
//	{
//		if(!m_allocated)
//		{
//			unsigned char * moreBytes = new unsigned char[a_byteCount];
//			if(!moreBytes)
//				return false;
//			m_bytes = moreBytes;
//			m_allocated = a_byteCount;
//		}
//		else
//		{
//			unsigned char * newBytes = new unsigned char[a_byteCount];
//			if(!newBytes)
//				return false;
//			for(int i = 0; i < m_size; ++i)
//			{
//				newBytes[i] = m_bytes[i];
//			}
//			delete m_bytes;
//			m_bytes = newBytes;
//			m_allocated = a_byteCount;
//		}
//		return true;
//	}
//	return false;
//}

//void ByteBuffer::add(const unsigned char * data, const int len)
//{
//	ensureCapacity(m_size+len);
//	if(data)
//	{
//		for(int i = 0; i < len; ++i)
//		{
//			m_bytes[m_size+i] = data[i];
//		}
//	}
//	m_size += len;
//}

void ByteBuffer::add(const char * data)
{
	TemplateVector<unsigned char>::add(strlen(data), (const unsigned char *)data);
}

void ByteBuffer::add(ByteBuffer * a_buffer)
{
	TemplateVector<unsigned char>::addVector(*a_buffer);
}

void ByteBuffer::set(const char * data)
{
	reset();
	add(data);
}

void ByteBuffer::set(const int a_index, const unsigned char a_value)
{
	TemplateVector<unsigned char>::set(a_index, a_value);
}

/**
 * @param a_size the new size this buffer thinks it has
 * @note: no error checking.
 */
//void ByteBuffer::setSize(const int a_size)
//{
//	m_size = a_size;
//}

//const int & ByteBuffer::size()
//{
//	return m_size;
//}

//const int & ByteBuffer::sizeAllocated()
//{
//	return m_allocated;
//}

const int ByteBuffer::unusedSize()
{
	return (m_allocated - m_size);
}

int ByteBuffer::socket_recv(SOCKET a_socket)
{
	unsigned long howMuchInBuffer = 0;
	ioctlsocket(a_socket, FIONREAD, &howMuchInBuffer);
	ensureCapacity(size()+howMuchInBuffer);
	int result = recv(a_socket, (char*)(&getLast()+1), unusedSize(), 0);
	if(result > 0)
	{
		m_size += result;
	}
	return result;
}

int ByteBuffer::socket_send(SOCKET a_socket)
{
	int result = send(a_socket, (const char *)getRawList(), size(), 0);
	if (result != 0 && result != SOCKET_ERROR)
	{
		reset();
	}
	return result;
}

ByteBuffer::~ByteBuffer()
{
	release();
}
