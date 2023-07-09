#include <iostream>

#include "include/ustr.h"

int main()
{
  const char* lol = "♪𐍈LOL";
  ustr str(lol, 11, Encoding::UTF8);
	uint32_t ch;
	while ((ch = str.getc()) && (ch != 0))
	{
//		std::cout << (char)ch;
	}
/*	Encoding enc = Encoding::UTF8;
	char* res = str.convert(enc);
	uint32_t len = strSz(res, enc);
	std::cout << "Size:\t" << strSz(res, enc) << std::endl;
	std::cout << "Len:\t" << strLen(res, enc) << std::endl;
  dump(res, len);*/
//	std::cout << ((str == "♪𐍈sLOL") ? "True" : "False") << std::endl;
//	std::cout << str << std::endl;
  return 0;
}

