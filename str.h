#include <iostream>
#include <cstring>

#include "encoding.h"
#include "utils.h"

class str {
public:
	str(const char* buffer, uint32_t size, Encoding encoding = Encoding::UTF8, bool link = false, bool universal = false);
	~str();

	uint32_t len();
	uint32_t sz();

	uint32_t getChar();
  void seek();

protected:
	Encoding m_encoding = Encoding::UTF8;
	char* m_str = nullptr;
	const char* m_link = nullptr;
	uint32_t m_size = 0;
};


str::str(const char* buffer, uint32_t size, Encoding encoding, bool link, bool universal)
{
	m_encoding = encoding;

	// Link ( Without copy )
	if (link) {
		m_link = buffer;
		m_size = size;
		return;
	}


	if (universal)	// Convert to universal format
	{

	} else {	// Stay in current format
		m_str = reinterpret_cast<char*>(std::malloc(size));
		std::memcpy(m_str, buffer, size);
		m_size = size;
	}

}

str::~str()
{
	if (m_str != nullptr)
		free(m_str);
}


uint32_t str::len()
{
	if (m_link) {
		return strLen(m_link, m_encoding);
	}
	return strLen(m_str, m_encoding);
}

uint32_t str::sz()
{
	if (m_link) {
		return strSz(m_link, m_encoding);
	}
	return strSz(m_str, m_encoding);
}

uint32_t str::getChar()
{

}
