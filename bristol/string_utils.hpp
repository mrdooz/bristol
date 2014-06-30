#pragma once
#include <string>
#ifdef _WIN32
#include <atlconv.h>
#endif

namespace bristol
{
#ifdef _WIN32
  bool wide_char_to_utf8(LPCOLESTR unicode, size_t len, std::string *str);
  std::string wide_char_to_utf8(const WCHAR *str);
#endif

  std::string to_string(char const * const format, ... );
  std::string trim(const std::string &str);

  std::string wide_char_to_utf8(const std::wstring &str);

  std::wstring utf8_to_wide(const char *str);

  bool begins_with(const char *str, const char *sub_str);
  bool begins_with(const std::string &str, const std::string &sub_str);
}
