#include <string>
#include <sstream>
#include <cstring>

#include "encoding.h"
#include "convTables.h"

// BEGIN TEMP
std::string charToHex(const char* buf, int sz)
{
  const unsigned char* tbuf = (unsigned char*)buf;
  std::stringstream ss;
  ss << std::hex;
  for (int i=0; i<sz; i++)
  {
    int b = int(tbuf[i]);
    if (b < 0x10)
      ss << "0";
    ss << b;
  }
  return ss.str();
}

void dump(const char* buf, int sz)
{
  std::cout << charToHex(buf, sz);
}
// END TEMP

uint16_t swap16(uint16_t num)
{
  return (uint16_t)( ((num & 0xFF00) >> 8) | ((num & 0xFF) << 8));
}


uint32_t swap32(uint32_t num)
{
  return (uint32_t)( ((num & 0xFF000000) >> 24) | ((num & 0xFF0000) >> 8) | ((num & 0xFF00) << 8) | ((num & 0xFF) << 24) );
}

// O(1)
uint32_t fastReservSize(uint32_t len, Encoding encoding = Encoding::UTF8)
{
	uint32_t size = len+1;
  switch (encoding)
	{
		case UTF8:
    case UTF32_LE: // aka UCS4_LE
    case UTF32_BE: // aka UCS4_BE
    case UTF16_LE:
    case UTF16_BE:
			return size*4;
			break;
		case UCS2_LE:
    case UCS2_BE:
			return size*2;
			break;
		default:
			return size;
			break;
	}
	return 0;
}

// O(N)
uint32_t smartReservSize(char* str, Encoding encIn, Encoding encOut = Encoding::UTF8)
{
	std::cout << "smartReservSize not relesed" << std::endl;
	return 0;
}


// Return string size in bytes with null terminator
uint32_t strSz(const char* str, Encoding enc)
{
  uint32_t count = 0;
  switch(enc)
  {
    case UCS2_LE:
    case UCS2_BE:
    case UTF16_LE:
    case UTF16_BE:
    {
      const char16_t* ch = (char16_t*) str;
      while (*ch != '\0')
      {
        count++;
        ch++;
      }
      count++; // null-terminator
      return count*2;
    }

    case UTF32_LE: // aka UCS4_LE
    case UTF32_BE: // aka UCS4_BE
    {
      const char32_t* ch = (char32_t*) str;
      while (*ch != '\0')
      {
        count++;
        ch++;
      }
      count++; // null-terminator
      return count*4;
    }

    case UTF8:
    default: // Other one byte encoding
    {
      const char* ch = str;
      while (*ch != '\0')
      {
        count++;
        ch++;
      }
      count++; // null-terminator
      return count;
    }
  }
}


// Return string length in charset without null terminator
uint32_t strLen(const char* str, Encoding enc)
{
  uint32_t count = 0;
  switch(enc)
  {
    case UCS2_LE:
    case UTF16_LE:
    {
      const char16_t* ch = (char16_t*) str;
      while (*ch) {
        char16_t n = swap16(*ch);
//        std::cout << (unsigned int)(n) << "!" << (unsigned int) 0xD800 << "!" << (unsigned int) 0xDFFF << std::endl;

        if (enc == Encoding::UTF16_LE &&
            ((0xD800 <= n) && (n <= 0xDFFF)) )
          ch++;

        count++;
        ch++;
      }
      return count;
    }

    case UTF16_BE:
    case UCS2_BE:
    {
      const char16_t* ch = (char16_t*) str;
      while (*ch) {
//        std::cout << (unsigned int)(n) << "!" << (unsigned int) 0xD800 << "!" << (unsigned int) 0xDFFF << std::endl;

        if (enc == Encoding::UTF16_BE &&
            ((0xD800 <= *ch) && (*ch <= 0xDFFF)) )
          ch++;

        count++;
        ch++;
      }
      return count;
    }

    case UTF32_LE: // aka UCS4_LE
    case UTF32_BE: // aka UCS4_BE
    {
      const char32_t* ch = (char32_t*) str;
      while (*ch != '\0')
      {
        count++;
        ch++;
      }
      return count;
    }

    case UTF8:
    {
      const uint8_t* ch = (uint8_t*) str;
//      while (*ch) count += (*ch++ & 0xc0) != 0x80;

      while (*ch != '\0')
      {
/*        if ((*ch & 0x80) == 0) {
          std::cout << "u1";
        } else */
        if ((*ch & 0xE0) == 0xC0) {
          ch++;
        } else if ((*ch & 0xF0) == 0xE0) {
          ch++;
          ch++;
        } else if ((*ch & 0xF8) == 0xF0) {
          ch++;
          ch++;
          ch++;
        }
        count++;
        ch++;
      }
      return count;
    }
    default: // Other one byte encoding
    {
      const char* ch = str;
      while (*ch != '\0')
      {
        count++;
        ch++;
      }
      return count;
    }
  }

}
/*
uint32_t enc2uni(uint32_t ch, Encoding encoding)
{
	switch (encoding)
	{
	case UTF32_LE:
		return ch;
	case UTF32_BE:
		return swap32(ch);
//    case UCS2_LE:
//    case UCS2_BE:
//    case UTF16_LE:
//    case UTF16_BE:
// 	const uint8_t* b1 = reinterpret_cast<const uint8_t*>(buffer);
	case UTF8:
	{
		if (( ch & (0x80 << 24) ) = 0)
		{
			
		}
	}

	}
}

uint32_t uni2enc(uint32_t ch, Encoding encoding)
{

}
*/


bool getChar(const char* buffer, Encoding encoding, uint32_t& res, uint8_t& byte)
{
	const uint8_t* b1 = reinterpret_cast<const uint8_t*>(buffer);
	if (*b1 == 0x00)
		return false;

	switch (encoding)
	{
		case UTF32_LE:
		{
			const uint32_t* ch = reinterpret_cast<const uint32_t*>(buffer);
			res = *ch;
			byte = 4;
			return true;
		}
		case UTF32_BE:
		{
			const uint32_t* ch = reinterpret_cast<const uint32_t*>(buffer);
			res = swap32(*ch);
			byte = 4;
			return true;
		}
/*		case UCS2_LE:
		case UCS2_BE:
		case UTF16_LE:
		case UTF16_BE:*/
		case UTF8:
		{
//			const uint8_t* b1 = reinterpret_cast<const uint8_t*>(buffer);
			//std::cout << "|" << (int)(*b1) << "|" << (int)(*b1 & 0x80) << "|";
			if ((*b1 & 0x80) == 0x00) {
				res = (uint32_t)*b1;
				byte = 1;
				return true;
			} else if ((*b1 & 0xE0) == 0xC0) {
				// some magic
				const uint8_t* b2 = reinterpret_cast<const uint8_t*>(buffer+1);
				// std::cout << (int)((*b1 & 0x1F) << 6) << "|" << (*b2 & 0x3F) << std::endl;
				// std::cout << (int)(((*b1 & 0x1F) << 6) | (*b2 & 0x3F)) << std::endl;
				res = (uint32_t)( ((*b1 & 0x1F) << 6) | (*b2 & 0x3F) );
				byte = 2;
				return true;
			} else if ((*b1 & 0xF0) == 0xE0) {
				const uint8_t* b2 = reinterpret_cast<const uint8_t*>(buffer+1);
				const uint8_t* b3 = reinterpret_cast<const uint8_t*>(buffer+2);
				res = (uint32_t)( ((*b1 & 0x0F) << 12) | ((*b2 & 0x3F) << 6) | (*b3 & 0x3F) );
				byte = 3;
				return true;
			} else if ((*b1 & 0xF8) == 0xF0) {
				const uint8_t* b2 = reinterpret_cast<const uint8_t*>(buffer+1);
				const uint8_t* b3 = reinterpret_cast<const uint8_t*>(buffer+2);
				const uint8_t* b4 = reinterpret_cast<const uint8_t*>(buffer+3);
				res = (uint32_t)( ((*b1 & 0x07) << 18) | ((*b2 & 0x3F) << 12) | ((*b3 & 0x3F) << 6) | (*b4 & 0x3F) );
				byte = 4;
				return true;
			}
			break;
		}

 		default:
		{
//			const uint8_t* b1 = reinterpret_cast<const uint8_t*>(buffer);
			res = (uint32_t)*b1;
			byte = 1;
			return true;
		}
	}
	return false;
}

bool writeSymbol(uint32_t res, char* buffer, Encoding encoding, uint8_t& bytes)
{
	switch(encoding)
	{
	case UTF16_LE:
	{
		if ( ((res >= 0xD800) and (res <= 0xDFFF)) || (res > 0x10FFFF) )
		{
			// Write question
			bytes = 2;
			uint16_t unknown = 0xFFFD;
			std::memcpy(buffer, (char*)(&unknown), bytes);
			return true;
		}

		if ( (res <= 0xD7FF) || ((res >= 0xE000) && (res <= 0xFFFF)) )
		{
			bytes = 2;
			std::memcpy(buffer, (char*)(&res), bytes);
		}
		else if ((res >= 0x10000) && (res <= 0x10FFFF))
		{
			bytes = 4;
			res -= 0x10000;
			uint16_t b1 = (0xD800 | (res & 0xFFC00));
			uint16_t b2 = (0xDC00 | (res & 0x3FF));
			std::memcpy(buffer,   (char*)(&b1), 2);
			std::memcpy(buffer+2, (char*)(&b2), 2);
		}

		return true;
	}
	case UTF16_BE:
	{
		if ( ((res >= 0xD800) and (res <= 0xDFFF)) || (res > 0x10FFFF) )
		{
			// Write question
			bytes = 2;
			uint16_t unknown = 0xFDFF;
			std::memcpy(buffer, (char*)(&unknown), bytes);
			return true;
		}

		if ( (res <= 0xD7FF) || ((res >= 0xE000) && (res <= 0xFFFF)) )
		{
			bytes = 2;
			uint16_t inverted = swap16( (uint16_t)res );
			std::memcpy(buffer, (char*)(&inverted), bytes);
		}
		else if ((res >= 0x10000) && (res <= 0x10FFFF))
		{
			bytes = 4;
			res -= 0x10000;
			uint16_t b1 = swap16(0xD800 | (res & 0xFFC00));
			uint16_t b2 = swap16(0xDC00 | (res & 0x3FF));
			std::memcpy(buffer,   (char*)(&b1), 2);
			std::memcpy(buffer+2, (char*)(&b2), 2);
		}

		return true;
	}
	case UCS2_LE:
	{
		if ( ( (res >= 0xD800) and (res <= 0xDFFF) ) or (res > 0xFFFF) )
		{
			// Write question
			bytes = 2;
			uint16_t unknown = 0xFFFD;
			std::memcpy(buffer, (char*)(&unknown), bytes);
			return true;
		}

		bytes = 2;
		std::memcpy(buffer, (char*)(&res), bytes);
		return true;
	}
	case UCS2_BE:
	{
		/* if ( (res >= 0xD800) and (res <= 0xDFFF) )
		{
			// Write question
			bytes = 2;
			uint16_t unknown = 0xFDFF;
			std::memcpy(buffer, (char*)(&unknown), bytes);
			return true;
		} */

		bytes = 2;
		uint16_t inverted = swap16((uint16_t)res);
		std::memcpy(buffer, (char*)(&inverted), bytes);
		return true;
	}
	case UTF32_LE: //	aka INTERNAL
	{
		bytes = 4;
		std::memcpy(buffer, (char*)(&res), bytes);
		return true;
	}
	case UTF32_BE:
	{
		bytes = 4;
		uint32_t reverseRes = swap32(res);
		std::memcpy(buffer, (char*)(&reverseRes), bytes);
		return true;
	}
	case UTF8:
	{
		if (res <= 0x007F)
		{
			bytes = 1;
			std::memcpy(buffer, (char*)(&res), bytes);
			return true;
		}
		else if ( (res > 0x007F) && (res <= 0x07FF) )
		{
			uint8_t b1 =  0xC0 | ((res & 0x7C0) >> 6);
			uint8_t b2 =  0x80 | (res & 0x3F);
			bytes = 2;
			std::memcpy(buffer, (char*)(&b1), 1);
			std::memcpy(buffer+1, (char*)(&b2), 1);
			return true;
		}
		else if ( (res > 0x07FF) and (res <= 0xFFFF) )
		{
			uint8_t b1 =  0xE0 | ((res & 0xF000) >> 12);
			uint8_t b2 =  0x80 | (res & 0x0FC0) >> 6;
			uint8_t b3 =  0x80 | (res & 0x3F);
			bytes = 3;
			std::memcpy(buffer, (char*)(&b1), 1);
			std::memcpy(buffer+1, (char*)(&b2), 1);
			std::memcpy(buffer+2, (char*)(&b3), 1);
			return true;
		}
		else if ( (res > 0xFFFF) && (res <= 0x1FFFFF) )
		{
			uint8_t b1 =  0xF0 | ((res & 0x1C0000) >> 18);
			uint8_t b2 =  0x80 | ((res & 0x3F000) >> 12);
			uint8_t b3 =  0x80 | ((res & 0x0FC0) >> 6);
			uint8_t b4 =  0x80 | (res & 0x3F);
			bytes = 4;
			std::memcpy(buffer, (char*)(&b1), 1);
			std::memcpy(buffer+1, (char*)(&b2), 1);
			std::memcpy(buffer+2, (char*)(&b3), 1);
			std::memcpy(buffer+3, (char*)(&b4), 1);
			return true;
		}
		break;
	}
	default:
		return false;
	}
	return false;
}

bool conv(const char* srcStr, uint32_t srcSize, Encoding srcEncoding,
     char* dstStr, uint32_t dstSize, Encoding dstEncoding)
{
	// Read universal code
//	uint32_t i = 0;
	const char* input = srcStr;
	char* output = dstStr;

	uint32_t res;
	uint8_t inbytes;

	uint8_t outbytes;
	while ( getChar(input, srcEncoding, res, inbytes) )
	{
		input += inbytes;

		writeSymbol(res, output, dstEncoding, outbytes);
		output += outbytes;
	}

	return true;
}
