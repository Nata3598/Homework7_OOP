#include <iostream>


bool testStringSize()
{
  bool success = true;

  auto test = [&success] (const char* name, const char* str, Encoding encoding, uint32_t len, uint32_t size)
  {
    bool failed = false;

    uint32_t _len = strLen(str, encoding);
    uint32_t _size = strSz(str, encoding);

    if (len != _len || size != _size)
      failed = true;

    if (!failed) {
      std::cout << "\033[1;32m" << name << "\033[0m" << std::endl;
    } else {
      std::cout << "\033[1;31m" << name << "\033[0m" << std::endl;
      std::cout << "Text:" << str << " \t( Length: " << _len << "\\" << len << "\tSize: " << _size << "\\" << size << " )" << std::endl;
      success = false;
    }
  };

  std::cout << "\n| Test Empty String |\n\n";

  std::cout << "Universal Coded Character Set (UCS)" << std::endl;

  // Test Empty String

  test("[UCS2LE] Test empty string length", (const char*) L"",
       Encoding::UCS2_LE, 0, 2);

  test("[UCS2BE] Test empty string length", (const char*) L"",
       Encoding::UCS2_BE, 0, 2);

  test("[UCS4LE] Test empty string length", (const char*) U"",
       Encoding::UCS4_LE, 0, 4);

  test("[UCS4BE] Test empty string length", (const char*) U"",
       Encoding::UCS4_BE, 0, 4);

  std::cout << "Unicode" << std::endl;

  test("[UTF8] Test empty string length", (const char*) u8"",
       Encoding::UTF8, 0, 1);

  test("[UTF16LE] Test empty string length", (const char*) u"",
       Encoding::UTF16_LE, 0, 2);

  test("[UTF16BE] Test empty string length", (const char*) u"",
       Encoding::UTF16_BE, 0, 2);

  test("[UTF32LE] Test empty string length", (const char*) U"",
       Encoding::UTF32_LE, 0, 4);

  test("[UTF32BE] Test empty string length", (const char*) U"",
       Encoding::UTF32_BE, 0, 4);

  std::cout << "Other" << std::endl;

  test("[CP1251] Test empty string length", "",
       Encoding::CP1251, 0, 1);


  std::cout << "\n| Test charset string |\n\n";


  test("[CP1251] Test length", "Test String",
       Encoding::CP1251, 11, 12);


  test("[UTF8] Test length", (const char*) u8"\u0040\u03A9\u2615\u1F980",
       Encoding::UTF8, 4, 11);

  test("[UTF16LE] Test length", (const char*) u"Test String",
       Encoding::UTF16_LE, 11, 24);





  return success;
}
