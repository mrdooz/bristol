#pragma once
#pragma warning(disable : 4996)
#include "output_buffer.hpp"

#if PARSER_WITH_VECTOR_TYPES
#endif

#if PARSER_WITH_MATRIX_TYPES
#endif

namespace parser
{
  struct InputBuffer;

  bool ParseBool(InputBuffer& buf, bool* res);
  bool ParseFloat(InputBuffer& buf, float* res, bool* success = nullptr);
#if PARSER_WITH_VECTOR_TYPES
  bool ParseColor(InputBuffer& buf, color* res);
  bool ParseVec2(InputBuffer& buf, vec2* res);
  bool ParseVec3(InputBuffer& buf, vec3* res);
  bool ParseVec4(InputBuffer& buf, vec4* res);
#endif
  bool ParseInt(InputBuffer& buf, int* res);
  bool ParseString(InputBuffer& buf, std::string* res);
  bool ParseIdentifier(InputBuffer& buf, std::string* res, bool colon = true);

  void Serialize(OutputBuffer& buf, bool value);
  void Serialize(OutputBuffer& buf, int value);
  void Serialize(OutputBuffer& buf, float value);
  void Serialize(OutputBuffer& buf, const std::string& value);
#if PARSER_WITH_VECTOR_TYPES
  void Serialize(OutputBuffer& buf, const color& value);
  void Serialize(OutputBuffer& buf, const vec2& value);
  void Serialize(OutputBuffer& buf, const vec3& value);
  void Serialize(OutputBuffer& buf, const vec4& value);
#endif
  void Serialize(OutputBuffer& buf, int indent, const char* member, bool value);
  void Serialize(OutputBuffer& buf, int indent, const char* member, int value);
  void Serialize(OutputBuffer& buf, int indent, const char* member, float value);
  void Serialize(OutputBuffer& buf, int indent, const char* member, const std::string& value);
#if PARSER_WITH_VECTOR_TYPES
  void Serialize(OutputBuffer& buf, int indent, const char* member, const color& value);
  void Serialize(OutputBuffer& buf, int indent, const char* member, const vec2& value);
  void Serialize(OutputBuffer& buf, int indent, const char* member, const vec3& value);
  void Serialize(OutputBuffer& buf, int indent, const char* member, const vec4& value);
#endif

  template <typename T>
  void Serialize(OutputBuffer& buf, int indent, const char* member, const std::vector<T>& value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 16 + 8 * value.size());
    buf._ofs += sprintf(buf.Cur(), "%*s: [", len + indent, member);
    for (size_t i = 0, e = value.size(); i < e; ++i)
    {
      Serialize(buf, value[i]);
      if (i != e - 1)
      {
        buf._ofs += sprintf(buf.Cur(), ", ");
      }
    }
    buf._ofs += sprintf(buf.Cur(), "];\n");
  }
}
