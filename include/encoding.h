#ifndef __STR_ENCODING__
#define __STR_ENCODING__

enum Encoding
{
  UNKNOWN  = 0,
//  UNIVERSAL = 4,
  INTERNAL  = 4,
  UTF8     = 1,
  UTF16_LE = 2,
  UTF16_BE = 3,
  UTF32_LE = 4,
  UTF32_BE = 5,
  UCS2_LE  = 6, // 2-byte  UTF16_LE
  UCS2_BE  = 7, // 2-byte  UTF16_LE
  UCS4_LE  = 4, // synonim UTF32_LE
  UCS4_BE  = 5, // synonim UTF32_BE
  CP1251   = 10,
  CP866    = 11,
  KOI8R    = 12,
  ISO8859_5 = 13,
};

#endif
