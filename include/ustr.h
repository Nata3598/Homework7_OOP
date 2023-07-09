#include "encoding.h"
#include "utils.h"

enum ustrpos
{
	BEGIN = 1,
	END   = 2
};

// Universal string (allocate memory and convert to universal format)
class ustr
{
public:
	ustr(const char* buffer = nullptr, uint32_t size = 0, Encoding encoding = Encoding::UTF8);
	~ustr();
	friend std::ostream&  operator<<(std::ostream&  os, const ustr& str);
	friend std::wostream& operator<<(std::wostream& os, const ustr& str);
	friend bool operator== (const ustr& str1, const ustr& str2);
	friend bool operator== (const ustr& str1, const char* str2);

	char* convert(Encoding encoding = Encoding::UTF8);
	// Read
	bool seekg(ustrpos  pos);
	bool seekg(uint32_t pos);
	uint32_t tellg();
	uint32_t getc();

protected:
	char* m_str = nullptr;
	uint32_t m_size = 0;
	Encoding m_encoding = Encoding::INTERNAL;
  // Read
	uint32_t m_pos_sym  = 0;
	uint32_t m_pos_fact = 0;
};

ustr::ustr(const char* buffer, uint32_t size, Encoding encoding)
{
/*	std::cout << "IN:\t";
  dump(buffer, size);
	std::cout << std::endl;*/

	uint32_t len = strLen(buffer, encoding);
//	std::cout << "Length: "<< len;
  m_size = fastReservSize(len, encoding);
	m_str = reinterpret_cast<char*>( malloc( m_size ) );
	conv(buffer, size, encoding,
		m_str, m_size, m_encoding);
/*
	std::cout << "OUT:\t";
  dump(m_str, m_size);
	std::cout << std::endl;*/
}


ustr::~ustr()
{
	if (m_str)
		free(m_str);
}

char* ustr::convert(Encoding encoding)
{
	uint32_t len = strLen(m_str, m_encoding);
  uint32_t size = fastReservSize(len, encoding);
//	std::cout << "Reserv: "<< size;
  char* str = reinterpret_cast<char*>( malloc( size ) );
  conv(m_str, m_size, m_encoding,
    str, size, encoding);
	return str;
}

std::ostream& operator<<(std::ostream& os, const ustr& str)
{
	Encoding encoding = Encoding::UTF8;
  uint32_t len = strLen(str.m_str, str.m_encoding);
  uint32_t size = fastReservSize(len, encoding);
  char* tstr = reinterpret_cast<char*>( malloc( size ) );
  conv(str.m_str, str.m_size, str.m_encoding,
    tstr, size, encoding);
	os << tstr;
	free(tstr);
	return os;
}

/* Debug on Windows MessegeBoxW
std::wostream& operator<<(std::wostream& os, const ustr& str)
{
	Encoding encoding = Encoding::UCS2_LE;
  uint32_t len = strLen(str.m_str, str.m_encoding);
  uint32_t size = reservSize(len, encoding);
  char* tstr = reinterpret_cast<char*>( malloc( size ) );
  conv(str.m_str, str.m_size, str.m_encoding,
    tstr, size, encoding);
	dump(tstr, size);
	os << tstr;
	free(tstr);
	return os;
}
*/

bool operator== (const ustr& str1, const ustr& str2)
{
	if ((str1.m_encoding == str2.m_encoding) && (str1.m_size == str2.m_size))
	{
		if (memcmp(str1.m_str, str2.m_str, str1.m_size) == 0)
			return true;
	}
	return false;
}

bool operator== (const ustr& str1, const char* str2)
{
	uint32_t len = strLen(str2, Encoding::UTF8);
	uint32_t size = fastReservSize(len, Encoding::UTF8);

	if ( (str1.m_encoding == Encoding::UTF8) && (str1.m_size == size) && (memcmp(str1.m_str, str2, size) == 0) )
	{
    return true;
	} else {
	  uint32_t l_size = fastReservSize(len, str1.m_encoding);
		if (str1.m_size == l_size)
		{
		  char* m_str = reinterpret_cast<char*>( malloc( l_size ) );
		  conv(str2, size, Encoding::UTF8,
		    m_str, l_size, str1.m_encoding);
			bool res = (memcmp(str1.m_str, m_str, str1.m_size) == 0);
//			std::cout << "GG";
/*			std::cout << "IN:\t";
			dump(str1.m_str, str1.m_size);
			std::cout << std::endl << "OUT:\t";
			dump(m_str, l_size);
			std::cout << std::endl;*/
			free(m_str);
			return res;
		}
	}
	return false;
}



bool ustr::seekg(ustrpos pos)
{
	switch(pos)
	{
	case BEGIN:
		m_pos_sym  = 0;
		m_pos_fact = 0;
		return true;
		break;
	case END:
		return false;
		break;
	}
	return false;
}

bool ustr::seekg(uint32_t pos)
{
	return false;
}


uint32_t ustr::tellg()
{
	return m_pos_sym;
}

uint32_t ustr::getc()
{
	uint32_t res = 0;
	switch (m_encoding)
	{
	case Encoding::UTF32_LE:
	{
		res = *(reinterpret_cast<uint32_t*>(m_str+m_pos_fact) );
		break;
	}
	default:
	{
		res = *(reinterpret_cast<uint8_t*>(m_str+m_pos_fact) );
	}
	}
	uint32_t ch;
	uint8_t  b;
	getChar(m_str, m_encoding, ch, b);
	m_pos_sym++;
	m_pos_fact += b;
	return res;
}
