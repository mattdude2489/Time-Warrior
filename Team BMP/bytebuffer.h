#pragma once

// this buffer was made specifically for Scokets data management
#include "winsock2.h"
#include "templatevector2.h"

/**
 * a contiguous memory data structure for managing network input and output
 * based on a TemplateVector
 */
class ByteBuffer : public TemplateVector<unsigned char>
{
public:
	void reset();
	void clean();
	/** helper function for standard C-strings */
	void add(const char * data);
	void add(ByteBuffer * a_buffer);
	void set(const char * data);
	void set(const int a_index, const unsigned char a_value);
	const int unusedSize();
	int socket_recv(SOCKET a_socket);
	int socket_send(SOCKET a_socket);
	~ByteBuffer();
};