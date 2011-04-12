#include "function.h"

/** copyString - resets the variable data, and creates a new string copy of the specified string, and stores it */
void StringVar::copyString(const char * a_string) {
	//reset the string variable
	resetVar();
	//hold a variable for the size of the string
	unsigned int size;
	//go through the specified string to record its size
	for(size = 0; a_string[size]; ++size);
	//create new memory for the string variable
	m_strVar = new char [size+1];
	//go through the strings
	for(unsigned int i = 0; i < size; ++i)
		//copy the data from the string to the string variable
		m_strVar[i] = a_string[i];
	//cap off the string variable with a null terminator
	m_strVar[size] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(unsigned char a_byte) {
	//create a digit buffer
	int digitBuffer[STRING_BUFFER_SIZE];
	//create a variable to hold the container value
	unsigned char val = a_byte;
	//create a temp variable
	unsigned int temp;
	//create a variable for the size of digits
	unsigned int size = 0;
	//go through all the digits in the container
	for(unsigned int i = 0; !i || a_byte; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//divide the container by ten and store it in the temp variable
		temp = a_byte / 10;
		//record the current digit as the container minus the temp variable
		digitBuffer[i] = a_byte - (temp * 10);
		//set the container to the temp variable
		a_byte = temp;
		//increment size
		++size;
	}
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_byte = val;
	//set the string variable to new memory
	m_strVar = new char [size + 2];
	//set the first character as the container type
	m_strVar[0] = 'c';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i)
		//set the character to the appropriate digit in the buffer
		m_strVar[i] = digitBuffer[size-i] + '0';
	//cap the string variable off with a null terminator
	m_strVar[size+1] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(unsigned short a_word) {
	//create a digit buffer
	int digitBuffer[STRING_BUFFER_SIZE];
	//create a variable to hold the container value
	unsigned short val = a_word;
	//create a temp variable
	unsigned int temp;
	//create a variable for the size of digits
	unsigned int size = 0;
	//go through all the digits in the container
	for(unsigned int i = 0; !i || a_word; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//divide the container by ten and store it in the temp variable
		temp = a_word / 10;
		//record the current digit as the container minus the temp variable
		digitBuffer[i] = a_word - (temp * 10);
		//set the container to the temp variable
		a_word = temp;
		//increment size
		++size;
	}
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_word = val;
	//set the string variable to new memory
	m_strVar = new char [size + 2];
	//set the first character as the container type
	m_strVar[0] = 's';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i)
		//set the character to the appropriate digit in the buffer
		m_strVar[i] = digitBuffer[size-i] + '0';
	//cap the string variable off with a null terminator
	m_strVar[size+1] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(unsigned long a_dword) {
	//create a digit buffer
	int digitBuffer[STRING_BUFFER_SIZE];
	//create a variable to hold the container value
	unsigned long val = a_dword;
	//create a temp variable
	unsigned int temp;
	//create a variable for the size of digits
	unsigned int size = 0;
	//go through all the digits in the container
	for(unsigned int i = 0; !i || a_dword; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//divide the container by ten and store it in the temp variable
		temp = a_dword / 10;
		//record the current digit as the container minus the temp variable
		digitBuffer[i] = a_dword - (temp * 10);
		//set the container to the temp variable
		a_dword = temp;
		//increment size
		++size;
	}
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_dword = val;
	//set the string variable to new memory
	m_strVar = new char [size + 2];
	//set the first character as the container type
	m_strVar[0] = 'i';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i)
		//set the character to the appropriate digit in the buffer
		m_strVar[i] = digitBuffer[size-i] + '0';
	//cap the string variable off with a null terminator
	m_strVar[size+1] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(unsigned long long a_qword) {
	//create a digit buffer
	int digitBuffer[STRING_BUFFER_SIZE];
	//create a variable to hold the container value
	unsigned long long val = a_qword;
	//create a temp variable
	unsigned int temp;
	//create a variable for the size of digits
	unsigned int size = 0;
	//go through all the digits in the container
	for(unsigned int i = 0; !i || a_qword; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//divide the container by ten and store it in the temp variable
		temp = (unsigned int)a_qword / 10;
		//record the current digit as the container minus the temp variable
		digitBuffer[i] = (unsigned int)a_qword - (temp * 10);
		//set the container to the temp variable
		a_qword = temp;
		//increment size
		++size;
	}
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_qword = val;
	//set the string variable to new memory
	m_strVar = new char [size + 2];
	//set the first character as the container type
	m_strVar[0] = 'l';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i)
		//set the character to the appropriate digit in the buffer
		m_strVar[i] = digitBuffer[size-i] + '0';
	//cap the string variable off with a null terminator
	m_strVar[size+1] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(float a_float) {
	//create a digit buffer
	int digitBuffer[STRING_BUFFER_SIZE];
	//create a variable for the integer portion of the float
	unsigned int integer = (int)a_float;
	//create a variable for the decimal portion of the float
	float decimal = a_float - integer;
	//create a temp variable
	unsigned int temp;
	//create a temp float variable
	float fTemp;
	//create a variable for the size of digits
	unsigned int size = 0;
	//create a variable for the size of integer digits and decimal digits
	unsigned int bufSize = 0;
	//go through all the digits in the integer
	for(unsigned int i = 0; !i || integer; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//divide the container by ten and store it in the temp variable
		temp = integer / 10;
		//record the current digit as the container minus the temp variable
		digitBuffer[i] = integer - (temp * 10);
		//set the container to the temp variable
		integer = temp;
		//increment size
		++size;
		//increment the buffer size
		++bufSize;
	}
	//go through all the decimal digits
	for(unsigned int i = 0; !i || decimal; ++i) {
		assert(bufSize < STRING_BUFFER_SIZE);
		//set the float temp to the decimal times 10
		fTemp = decimal * 10;
		//set the decimal digit to the float temp, with decimal places cut off
		digitBuffer[bufSize] = (int)fTemp + '0';
		//set decimal to float temp minus the decimal digit
		decimal = fTemp - digitBuffer[bufSize];
		//increment the buffer size
		++bufSize;
	}
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_float = a_float;
	//set the string variable to new memory
	m_strVar = new char [bufSize + 3];
	//set the first character as the container type
	m_strVar[0] = 'f';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i)
		//set the character to the appropriate digit in the buffer
		m_strVar[i] = digitBuffer[size-i] + '0';
	//set the decimal character
	m_strVar[size+1] = '.';
	//go through the rest of the digits
	for(unsigned int i = size; i <= bufSize; ++i)
		//set the character in the string to the appropriate digit in the buffer
		m_strVar[i+2] = digitBuffer[i] + '0';
	//cap the string variable off with a null terminator
	m_strVar[bufSize+2] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(double a_double) {
	//create a digit buffer
	int digitBuffer[STRING_BUFFER_SIZE];
	//create a variable for the integer portion of the float
	unsigned int integer = (int)a_double;
	//create a variable for the decimal portion of the float
	double decimal = a_double - integer;
	//create a temp variable
	unsigned int temp;
	//create a temp float variable
	double dTemp;
	//create a variable for the size of digits
	unsigned int size = 0;
	//create a variable for the size of integer digits and decimal digits
	unsigned int bufSize = 0;
	//go through all the digits in the integer
	for(unsigned int i = 0; !i || integer; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//divide the container by ten and store it in the temp variable
		temp = integer / 10;
		//record the current digit as the container minus the temp variable
		digitBuffer[i] = integer - (temp * 10);
		//set the container to the temp variable
		integer = temp;
		//increment size
		++size;
		//increment the buffer size
		++bufSize;
	}
	//go through all the decimal digits
	for(unsigned int i = 0; !i || decimal; ++i) {
		assert(bufSize < STRING_BUFFER_SIZE);
		//set the float temp to the decimal times 10
		dTemp = decimal * 10;
		//set the decimal digit to the float temp, with decimal places cut off
		digitBuffer[bufSize] = (int)dTemp + '0';
		//set decimal to float temp minus the decimal digit
		decimal = dTemp - digitBuffer[bufSize];
		//increment the buffer size
		++bufSize;
	}
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_double = a_double;
	//set the string variable to new memory
	m_strVar = new char [bufSize + 3];
	//set the first character as the container type
	m_strVar[0] = 'd';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i)
		//set the character to the appropriate digit in the buffer
		m_strVar[i] = digitBuffer[size-i] + '0';
	//set the decimal character
	m_strVar[size+1] = '.';
	//go through the rest of the digits
	for(unsigned int i = size; i <= bufSize; ++i)
		//set the character in the string to the appropriate digit in the buffer
		m_strVar[i+2] = digitBuffer[i] + '0';
	//cap the string variable off with a null terminator
	m_strVar[bufSize+2] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(char * a_string) {
	//create a variable for specified string size
	unsigned int size;
	//go through the specified string and record its size
	for(size = 0; a_string[size]; ++size);
	//reset the string variable
	resetVar();
	//record the new size of the string variable
	unsigned int newSize = size + 3;
	//set the string variable to new memory
	m_strVar = new char [newSize];
	//set the container type
	m_strVar[0] = '\"';
	//go through the strings
	for(unsigned int i = 0; i < size; ++i) {
		assert(i < STRING_BUFFER_SIZE);
		//copy the string data from the specified string
		m_string[i] = a_string[i];
		//copy the data from the specified string to the string variable
		m_strVar[i+1] = a_string[i];
	}
	//set the container type
	m_strVar[newSize-2] = '\"';
	//cap the string variable off with a null terminator
	m_strVar[newSize-1] = 0;
	//set the string to be released
	m_releaseString = true;
}

/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
void StringVar::setContainer(void * a_address) {
	//create a hex digit buffer
	int hexDigitBuffer[STRING_BUFFER_SIZE];
	//record the address numerically
	unsigned long long addressNumerical = (unsigned long long)a_address;
	//create a variable for the size of digits
	unsigned int size;
	//go through all the digits hex-wise in the container
	for(size = 0; !size || addressNumerical; addressNumerical = addressNumerical >> 4)
		//record all the hex digits
		{ assert(size < STRING_BUFFER_SIZE); hexDigitBuffer[size++] = (unsigned int)(addressNumerical & 0xf); }
	//reset the string variable
	resetVar();
	//set the container to the specified
	m_address = a_address;
	//set the string variable to new memory
	m_strVar = new char [size + 4];
	//set the first character as the container type
	m_strVar[0] = '&';
	m_strVar[1] = '0';
	m_strVar[2] = 'x';
	//go through the string variable
	for(unsigned int i = 1; i <= size; ++i) {
		//record the current hex digit
		unsigned int hexDigit = hexDigitBuffer[size-i];
		//set the character to the appropriate digit in the buffer
		m_strVar[i+2] = hexDigit + (hexDigit < 10?'0':'a' - 10);
	}
	//cap the string variable off with a null terminator
	m_strVar[size+3] = 0;
	//set the string to be released
	m_releaseString = true;
}



/** resetVar - destroys any needed string and resets the variable back to defaults */
void StringVar::resetVar() {
	if(m_containerType == PARSE_NONE)
		return;
	//if the string needs to be released
	if(m_releaseString) {
		//release it
		delete [] m_strVar;
		m_strVar = 0;
		m_releaseString = false;
	}
	//check the container type for the string variable
	switch(m_containerType) {
	//reset that associated container
	case PARSE_BYTE: m_byte = 0; break;
	case PARSE_WORD: m_word = 0; break;
	case PARSE_DWORD: m_dword = 0; break;
	case PARSE_QWORD: m_qword = 0; break;
	case PARSE_FLOAT: m_float = 0.0f; break;
	case PARSE_DOUBLE: m_double = 0.0; break;
	case PARSE_STRING: for(unsigned int i = 0; m_string[i]; ++i) { assert(i < STRING_BUFFER_SIZE); m_string[i] = 0; } break;
	case PARSE_ADDRESS:	m_address = 0; break;
	}
	//reset the container type
	m_containerType = PARSE_NONE;
}

/** parseStr - parses the c-string variable into the appropriate container, and sets a flag signalling which */
bool StringVar::parseStr() {
	assert(m_strVar != 0);
	//if string variable needs to be reset
	if(m_containerType != PARSE_NONE)
		//reset it
		resetVar();
	//hold a variable for the parse mode for the current character
	unsigned long parseMode = PARSE_WHITESPACE;
	//hold a variable for the next mode of parsing
	unsigned long nextMode = PARSE_HEADER;
	//flag system for parsing situations
	unsigned long parseFlags = 0;
	//variable for the post header type
	char postHeaderType = 0;
	//variable for the currently read c-string character (c-string variables: "\'\0\'; c0; s0; i0; l0; f0.0; d0.0; \"\"; &0")
	char cur;
	//variable for the current char index
	unsigned int index = 0;
	//variable for the data index
	unsigned int dataIndex = 0;
	//variable for amount of data characters
	unsigned int dataSize = 0;
	//hold a variable for the decimal place (for real numbers)
	unsigned int decimalPlace = 0;
	//hold a variable for real number precision
	unsigned int realNumPrecision = 0;
	//read data until the end of the string is reached or there is a syntax error
	for(unsigned int i = 0; (cur = m_strVar[i]) && !( parseFlags & (PARSE_SYNTAX_ERROR | PARSE_TRAILING_DATA) ); ++i) {
		//check the parsing mode
		switch(parseMode) {
		//if whitespace needs to be read
		case PARSE_WHITESPACE:
			//check the current character
			switch(cur) {
			//if whitespace
			case '\t':
			case '\n':
			case ' ':
				//move on
				break;
			//else
			default:
				//set the parse mode to the next one
				parseMode = nextMode;
				//reread this character later
				--i;
				break;
			}
			break;
		//if the header needs to be read
		case PARSE_HEADER:
			//check the current character
			switch(cur) {
			//if double quote
			case '\"':
				//set the parse type to string
				m_containerType = PARSE_STRING;
				//mark char flag
				parseFlags |= PARSE_CHAR;
				//set the parse mode to parsing data
				parseMode = PARSE_DATA;
				//set the post header type to the current header
				postHeaderType = cur;
				break;
			//if ampersand
			case '&':
				//set the parse type to address
				m_containerType = PARSE_ADDRESS;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				break;
			//if single quote
			case '\'':
				//set the parse type to byte
				m_containerType = PARSE_BYTE;
				//mark char flag
				parseFlags |= PARSE_CHAR;
				//set the parse mode to parsing data
				parseMode = PARSE_DATA;
				//set the post header type to the current header
				postHeaderType = cur;
				break;
			//if c
			case 'c':
				//set the parse type to byte
				m_containerType = PARSE_BYTE;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				break;
			//if d
			case 'd':
				//set the parse type to double
				m_containerType = PARSE_DOUBLE;
				//mark real flag
				parseFlags |= PARSE_REAL;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				//set the real number precision to double's precision
				realNumPrecision = DOUBLE_PRECISION;
				break;
			//if f
			case 'f':
				//set the parse type to float
				m_containerType = PARSE_FLOAT;
				//mark real flag
				parseFlags |= PARSE_REAL;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				//set the real number precision to float's precision
				realNumPrecision = FLOAT_PRECISION;
				break;
			//if i
			case 'i':
				//set the parse type to double word
				m_containerType = PARSE_DWORD;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				break;
			case 'l':
				//set the parse type to quad word
				m_containerType = PARSE_QWORD;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				break;
			case 's':
				//set the parse type to word
				m_containerType = PARSE_WORD;
				//set white space to be parsed before going to the next mode
				parseMode = PARSE_WHITESPACE;
				break;
			//else
			default:
				//flag a syntax error
				parseFlags |= PARSE_SYNTAX_ERROR;
				//set the parse mode as ended
				parseMode = PARSE_END;
				break;
			}
			//set the parsing mode to parse whitespace, before the next mode
			nextMode = PARSE_DATA;
			break;
		//if data needs to be read
		case PARSE_DATA:
			//if this is the first char of data
			if(!dataIndex) {
				//if data is not a char
				if( !(parseFlags & (PARSE_CHAR) ) ) {
					//if first data char is '-'
					if(cur == '-') {
						//flag data as negative
						parseFlags |= PARSE_NEGATIVE;
						//if the next character is not the null terminator
						if(m_strVar[i+1])
							//set the current character to the next one
							cur = m_strVar[++i];
						//else
						else
							//move on
							break;
					}
					//go until whitespace or null terminator is hit
					for(char c = cur; c && c != '\t' && c != '\n' && c != ' ';)
						//increment data size
						c = m_strVar[i+(++dataSize)];
				}
				//if data is not a char or real, and data size is greater than 0
				if( !( parseFlags & (PARSE_CHAR | PARSE_REAL) ) && dataSize ) {
					//create a temp string for the next two characters
					char temp[2];
					//get the current character
					temp[0] = cur;
					//get the next one
					temp[1] = m_strVar[++i];
					//if these characters are "0x"
					if(temp[0] == '0' && temp[1] == 'x') {
						//decrement data size by 2
						dataSize -= 2;
						//flag the data as hex
						parseFlags |= PARSE_HEX;
						//if the next character is not the null terminator
						if(dataSize)
							//set the current character to the next one
							cur = m_strVar[++i];
						//else
						else
							//move on
							break;
					}
					//else
					else {
						//go back an index
						--i;
						//flag the data as numeric
						parseFlags |= PARSE_NUMERIC;
					}
				}
			}
			//check the container type
			switch(m_containerType) {
			//if a byte has to be parsed
			case PARSE_BYTE:
				//if char is flagged
				if( parseFlags & PARSE_CHAR ) {
					//set the byte data to the current character
					m_byte = cur;
					//set the parse mode to parse the post header
					parseMode = PARSE_POST_HEADER;
				}
				//else if char is not flagged
				else {
					//if data is numeric
					if(parseFlags & PARSE_NUMERIC) {
						//if current character is number
						if(cur >= '0' && cur <= '9')
							//add the data to the container
							m_byte += (unsigned char)parseCharNumeric(cur, (dataSize-1) - dataIndex);
						//else if current character is not number
						else
							//flag a syntax error
							parseFlags |= PARSE_SYNTAX_ERROR;
					}
					//else if data is hex
					else if(parseFlags & PARSE_HEX) {
						//if current character is hex
						if( (cur >= '0' && cur <= '9') || (cur >= 'A' && cur <= 'F') || (cur >= 'a' && cur <= 'f') )
							//add the data to the container
							m_byte += (unsigned char)parseCharHex(cur, (dataSize-1) - dataIndex);
						//else if current character is not hex
						else
							//flag a syntax error
							parseFlags |= PARSE_SYNTAX_ERROR;
					}
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if a word has to be parsed
			case PARSE_WORD:
				//if data is numeric
				if(parseFlags & PARSE_NUMERIC) {
					//if current character is number
					if(cur >= '0' && cur <= '9')
						//add the data to the container
						m_word += (unsigned short)parseCharNumeric(cur, (dataSize-1) - dataIndex);
					//else if current character is not number
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//else if data is hex
				else if(parseFlags & PARSE_HEX) {
					//if current character is hex
					if( (cur >= '0' && cur <= '9') || (cur >= 'A' && cur <= 'F') || (cur >= 'a' && cur <= 'f') )
						//add the data to the container
						m_word += (unsigned short)parseCharHex(cur, (dataSize-1) - dataIndex);
					//else if current character is not hex
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if a double word has to be parsed
			case PARSE_DWORD:
				//if data is numeric
				if(parseFlags & PARSE_NUMERIC) {
					//if current character is number
					if(cur >= '0' && cur <= '9')
						//add the data to the container
						m_dword += (unsigned long)parseCharNumeric(cur, (dataSize-1) - dataIndex);
					//else if current character is not number
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//else if data is hex
				else if(parseFlags & PARSE_HEX) {
					//if current character is hex
					if( (cur >= '0' && cur <= '9') || (cur >= 'A' && cur <= 'F') || (cur >= 'a' && cur <= 'f') )
						//add the data to the container
						m_dword += (unsigned long)parseCharHex(cur, (dataSize-1) - dataIndex);
					//else if current character is not hex
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if a quad word has to be parsed
			case PARSE_QWORD:
				//if data is numeric
				if(parseFlags & PARSE_NUMERIC) {
					//if current character is number
					if(cur >= '0' && cur <= '9')
						//add the data to the container
						m_qword += parseCharNumeric(cur, (dataSize-1) - dataIndex);
					//else if current character is not number
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//else if data is hex
				else if(parseFlags & PARSE_HEX) {
					//if current character is hex
					if( (cur >= '0' && cur <= '9') || (cur >= 'A' && cur <= 'F') || (cur >= 'a' && cur <= 'f') )
						//add the data to the container
						m_qword += parseCharHex(cur, (dataSize-1) - dataIndex);
					//else if current character is not hex
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if a float has to be parsed
			case PARSE_FLOAT:
				//if decimal point is not yet reached
				if(!decimalPlace) {
					//if current character is decimal point
					if(cur == '.') {
						//divide the float by 10 to the power of the remainder of characters left to process
						m_float /= (float)pow(10, dataSize - dataIndex);
						//increment the decimal place offset
						++decimalPlace;
					}
					//else if current character is data
					else {
						//if current character is number
						if(cur >= '0' && cur <= '9')
							//add the data to the container
							m_float += (float)parseCharNumeric(cur, (dataSize-1) - dataIndex);
						//else if current character is not number
						else
							//flag a syntax error
							parseFlags |= PARSE_SYNTAX_ERROR;
					}
				}
				//else if decimal point is reached
				else {
					//if current character is number
					if(cur >= '0' && cur <= '9') {
						//if the decimal place isn't past the precision limit
						if(decimalPlace <= realNumPrecision)
							//add the data to the container
							m_float += (float)parseCharNumeric(cur, 0) / pow(10, decimalPlace++);
						else {
							//flag parsing as finished
							parseFlags |= (PARSE_FINISHED | PARSE_TRAILING_DATA);
							//set the parse mode to end
							parseMode = PARSE_END;
						}
					}
					//else if current character is not number
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if a double has to be parsed
			case PARSE_DOUBLE:
				//if decimal point is not yet reached
				if(!decimalPlace) {
					//if current character is decimal point
					if(cur == '.') {
						//divide the float by 10 to the power of the remainder of characters left to process
						m_double /= (double)pow(10, dataSize - dataIndex);
						//increment the decimal place offset
						++decimalPlace;
					}
					//else if current character is data
					else {
						//if current character is number
						if(cur >= '0' && cur <= '9')
							//add the data to the container
							m_double += (double)parseCharNumeric(cur, (dataSize-1) - dataIndex);
						//else if current character is not number
						else
							//flag a syntax error
							parseFlags |= PARSE_SYNTAX_ERROR;
					}
				}
				//else if decimal point is reached
				else {
					//if current character is number
					if(cur >= '0' && cur <= '9') {
						//if the decimal place isn't past the precision limit
						if(decimalPlace <= realNumPrecision)
							//add the data to the container
							m_double += (double)parseCharNumeric(cur, 0) / pow(10, decimalPlace++);
						else {
							//flag parsing as finished
							parseFlags |= (PARSE_FINISHED | PARSE_TRAILING_DATA);
							//set the parse mode to end
							parseMode = PARSE_END;
						}
					}
					//else if current character is not number
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if a string has to be parsed
			case PARSE_STRING:
				//if the current character is not the post header
				if( cur != postHeaderType ) {
					assert(dataIndex < STRING_BUFFER_SIZE);
					//set the associated string character to the current character
					m_string[dataIndex] = cur;
				}
				//else if current character is the post header
				else {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			//if an address has to be parsed
			case PARSE_ADDRESS:
				//if data is numeric
				if(parseFlags & PARSE_NUMERIC) {
					//if current character is number
					if(cur >= '0' && cur <= '9')
						//add the data to the container
						m_address = (void *)((unsigned long long)m_address + parseCharNumeric(cur, (dataSize-1) - dataIndex));
					//else if current character is not number
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//else if data is hex
				else if(parseFlags & PARSE_HEX) {
					//if current character is hex
					if( (cur >= '0' && cur <= '9') || (cur >= 'A' && cur <= 'F') || (cur >= 'a' && cur <= 'f') )
						//add the data to the container
						m_address = (void *)((unsigned long long)m_address + parseCharHex(cur, (dataSize-1) - dataIndex));
					//else if current character is not hex
					else
						//flag a syntax error
						parseFlags |= PARSE_SYNTAX_ERROR;
				}
				//if data index is the last index
				if(dataIndex == dataSize-1) {
					//flag parsing as finished
					parseFlags |= PARSE_FINISHED;
					//set the parse mode to end
					parseMode = PARSE_END;
				}
				break;
			}
			//increment the data index
			++dataIndex;
			break;
		//if a post header needs to be checked
		case PARSE_POST_HEADER:
			//if the current character is not the post header
			if(cur != postHeaderType)
				//flag a syntax error
				parseFlags |= PARSE_SYNTAX_ERROR;
			else
				parseFlags |= PARSE_FINISHED;
			//set the parse mode to end
			parseMode = PARSE_END;
			break;
		//if there's still characters at the end parse mode
		case PARSE_END:
			//flag data as trailing
			parseFlags |= (PARSE_TRAILING_DATA);
			break;
		}
	}

	//if negative flag was raised
	if( parseFlags & PARSE_NEGATIVE ) {
		//check the container type
		switch(m_containerType) {
		//if byte, set data to negative
		case PARSE_BYTE: m_byte = (unsigned)(-(signed)m_byte); break;
		//if word, set data to negative
		case PARSE_WORD: m_word = (unsigned)(-(signed)m_word); break;
		//if double word, set data to negative
		case PARSE_DWORD: m_dword = (unsigned)(-(signed)m_dword); break;
		//if quad word, set data to negative
		case PARSE_QWORD: m_qword = (unsigned)(-(signed)m_qword); break;
		//if float, set data to negative
		case PARSE_FLOAT: m_float = -m_float; break;
		//if double, set data to negative
		case PARSE_DOUBLE: m_double = -m_double; break;
		//if address, set data to negative
		case PARSE_ADDRESS:	m_address = (void *)(-(long long)m_address); break;
		}
	}

	//if parsing did not finish
	if( !( parseFlags & PARSE_FINISHED ) )
		//flag a syntax error
		parseFlags |= PARSE_SYNTAX_ERROR;
		
	//if there was a syntax error
	if( parseFlags & PARSE_SYNTAX_ERROR )
		//reset the variable
		resetVar();

	//return true if no syntax errors or trailing data
	return !( parseFlags & (PARSE_SYNTAX_ERROR | PARSE_TRAILING_DATA) );
}

/*
* getStringCopy - returns a new copy of the c-string variable
* Note: must be externally managed
*/
char * StringVar::getStringCopy() const {
	unsigned int size;
	for(size = 0; m_strVar[size]; ++size);
	char * copy = new char [size+1];
	for(unsigned int i = 0; i < size; ++i)
		copy[i] = m_strVar[i];
	copy[size] = 0;
	return copy;
}
