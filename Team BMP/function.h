#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "Global.h"

#include <assert.h>

/*
* Struct: Function
* Author: Jared Bills
* Stores a function pointer with included c-string parameter.
*/
struct Function {
	/** function pointer to call */
	FunctionPtr m_func;
	/** c-string parameter for function */
	char * m_param;
	/** default constructor */
	inline Function() : m_func(0), m_param(0) {}
	/** constructor - sets the function's properties */
	inline Function(const FunctionPtr & a_func, const char * & a_param) : m_func(a_func), m_param((char *)a_param) {}
	/** callFunc - calls the function */
	inline void callFunc() { m_func(m_param); }
	/** isValid - returns true if function pointer is not null */
	inline const bool isValid() const { return (m_func != 0); }
};

/*
* Class: StringVar
* Author: Jared Bills
* Contains a reference to a c-string variable for use with Function objects, and
* is able to parse it to a requested C variable and return the data based on a data
* type ID.
*/
class StringVar {
private:
	/** after usage, should the c-string be released? */
	bool m_releaseString;
	/** specifies which container the string variable is parsed into */
	unsigned char m_containerType;
	/** parsed byte container */
	unsigned char m_byte;
	/** parsed word container */
	unsigned short m_word;
	/** parsed double word container */
	unsigned long m_dword;
	/** parsed quad word container */
	unsigned long long m_qword;
	/** parsed float container */
	float m_float;
	/** parsed double container */
	double m_double;
	/** parsed string container */
	char m_string[STRING_BUFFER_SIZE];
	/** parsed address container */
	void * m_address;
	/** c-string variable */
	char * m_strVar;

	/** pow - multiplies a number with itself by a power of an exponent */
	inline const unsigned long long pow(const unsigned long long & a_num, const unsigned int & a_exp) { unsigned long long result = (a_exp?a_num:1); for(unsigned int i = 1; i < a_exp; ++i) result*=a_num; return result; }
	/** parseCharNumeric - converts a char number into an integral number */
	inline const unsigned long long parseCharNumeric(const char & a_char, const int & a_place) { unsigned long long result = a_char - '0'; result *= pow(10, a_place); return result; }
	/** parseCharHex - converts a char hex number into an integral number */
	inline const unsigned long long parseCharHex(const char & a_char, const int & a_place) { unsigned long long result = a_char - (a_char >= '0' && a_char <= '9'? '0' : (a_char >= 'a' && a_char <= 'f'? 'a' : 'A' ) - 10 ); result *= pow(16, a_place); return result; }
public:
	/** default constructor */
	inline StringVar() : m_releaseString(false), m_containerType(PARSE_NONE), m_byte(0), m_word(0), m_dword(0), m_qword(0), m_float(0.0f), m_double(0.0), m_address(0), m_strVar(0) { for(unsigned int i = 0; i < STRING_BUFFER_SIZE; ++i) m_string[i] = 0; }
	/** destructor */
	inline ~StringVar() { resetVar(); }

	/** setString - resets the variable data, and sets the string variable to the given */
	inline void setString(char * a_string) { resetVar(); m_strVar = a_string; }
	/** copyString - resets the variable data, and creates a new string copy of the specified string, and stores it */
	void copyString(const char * a_string);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(unsigned char a_byte);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(unsigned short a_word);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(unsigned long a_dword);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(unsigned long long a_qword);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(float a_float);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(double a_double);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(char * a_string);
	/** setContainer - sets the specified container to the given, and deparses the value into a string variable */
	void setContainer(void * a_address);

	/** resetVar - destroys any needed string and resets the variable back to defaults */
	void resetVar();
	/** parseStr - parses the c-string variable into the appropriate container, and sets a flag signalling which (returns true if no syntax errors) */
	bool parseStr();

	/** getContainerType - returns the type of container the string variable was parsed into */
	inline const unsigned char & getContainerType() const { return m_containerType; }
	/** getString - returns a reference to the c-string variable */
	inline const char * getString() const { return m_strVar; }
	/*
	* getStringCopy - returns a new copy of the c-string variable
	* Note: must be externally managed
	*/
	char * getStringCopy() const;
	/*
	* getVarReference - returns a reference to the appropriate non-string data
	* Note: use DEREF(var, container_type) to dereference properly into your needed variable
	*/
	inline void * getVarReference() {
		assert(m_containerType != PARSE_NONE);
		//hold reference to data
		void * data = 0;
		//check the container type
		switch(m_containerType) {
		//if byte, set data to byte container
		case PARSE_BYTE: data = &m_byte; break;
		//if word, set data to word container
		case PARSE_WORD: data = &m_word; break;
		//if double word, set data to double word container
		case PARSE_DWORD: data = &m_dword; break;
		//if quad word, set data to quad word container
		case PARSE_QWORD: data = &m_qword; break;
		//if float, set data to float container
		case PARSE_FLOAT: data = &m_float; break;
		//if double, set data to double container
		case PARSE_DOUBLE: data = &m_double; break;
		//if string, set data to string container
		case PARSE_STRING: data = m_string; break;
		//if address, set data to address container
		case PARSE_ADDRESS:	data = m_address; break;
		}
		//return the reference to the data
		return data;
	}
};

#endif
