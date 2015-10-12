#include <ctype.h>
#include <math.h>
#include "parse_base.hpp"
#include "input_buffer.hpp"
#include "output_buffer.hpp"

#pragma warning(disable : 4996)
using namespace std;

namespace parser
{
  //-----------------------------------------------------------------------------
  bool ParseBool(InputBuffer& buf, bool* res)
  {
    size_t start = buf._idx;
<<<<<<< Updated upstream
    size_t end;
<<<<<<< HEAD
    buf.SkipWhile(InputBuffer::IsAlphaNum, &end);
=======
    CHECKED_OP(buf.SkipWhile(InputBuffer::IsAlphaNum, &end));
=======
    buf.SkipWhile(InputBuffer::IsAlphaNum);
    size_t end = buf._idx;
>>>>>>> Stashed changes
>>>>>>> updates

    string str;
    CHECKED_OP(buf.SubStr(start, end - start, &str));

    if (str == "true")
    {
      *res = true;
      return true;
    }

    if (str == "false")
    {
      *res = false;
      return true;
    }

    return false;
  }

  //-----------------------------------------------------------------------------
  bool ParseFloat(InputBuffer& buf, float* res, bool* success)
  {
    buf.SaveState();

    char ch;
    CHECKED_OP(buf.OneOf("-+", 2, &ch));
    bool neg = ch == '-';

    int numLeadingDigits = 0;
    int numTrailingDigits = 0;

    int whole = 0;
    // find whole part if one exists
    while (buf.Satifies(InputBuffer::IsDigit, &ch))
    {
      numLeadingDigits++;
      whole = whole * 10 + (ch - '0');
    }

    float tmp = (neg ? -1.f : 1.f) * whole;

    // look for a '.'
    if (buf.ConsumeIf('.'))
    {
      // fractional
      int frac = 0;
      int len = 0;
      while (buf.Satifies(InputBuffer::IsDigit, &ch))
      {
        ++len;
        frac = frac * 10 + (ch - '0');
        numTrailingDigits++;
      }

      if (len)
      {
        tmp += frac / powf(10.f, (float)len);
      }
    }

    *res = tmp;

    // If success is set, then we only want to advance if the parse was succesful
    bool validParse = numLeadingDigits > 0 || numTrailingDigits > 0;
    if (success)
    {
      *success = validParse;
    }
    buf.RestoreState(!success || validParse);
    return true;
  }

  //-----------------------------------------------------------------------------
  bool ParseInt(InputBuffer& buf, int* res)
  {
    char ch;
    CHECKED_OP(buf.OneOf("-+", 2, &ch));
    bool neg = ch == '-';

    // read the first char, and make sure it's a digit
    CHECKED_OP(buf.Satifies(InputBuffer::IsDigit, &ch));

    int val = ch - '0';
    while (buf.Satifies(InputBuffer::IsDigit, &ch))
    {
      val = val * 10 + (ch - '0');
    }

    *res = (neg ? -1 : 1) * val;
    return true;
  }

  //-----------------------------------------------------------------------------
  template <int N>
  bool ParseVec(InputBuffer& buf, float* res)
  {
    // { x, y, z, w }
    CHECKED_OP(buf.Expect('{'));

    for (int i = 0; i < N; ++i)
    {
      buf.SkipWhitespace();
      CHECKED_OP(ParseFloat(buf, &res[i]));

      if (i != N - 1)
      {
        buf.SkipWhitespace();
        CHECKED_OP(buf.Expect(','));
      }
    }

    buf.SkipWhitespace();
    CHECKED_OP(buf.Expect('}'));
    return true;
  }

#if PARSER_WITH_VECTOR_TYPES
  //-----------------------------------------------------------------------------
  bool ParseColor(InputBuffer& buf, color* res)
  {
    float tmp[4];
    CHECKED_OP(ParseVec<4>(buf, tmp));
    *res = color(tmp[0], tmp[1], tmp[2], tmp[3]);
    return true;
  }

  //-----------------------------------------------------------------------------
  bool ParseVec2(InputBuffer& buf, vec2* res)
  {
    float tmp[2];
    CHECKED_OP(ParseVec<2>(buf, tmp));
    *res = vec2(tmp[0], tmp[1]);
    return true;
  }

  //-----------------------------------------------------------------------------
  bool ParseVec3(InputBuffer& buf, vec3* res)
  {
    float tmp[3];
    CHECKED_OP(ParseVec<3>(buf, tmp));
    *res = vec3(tmp[0], tmp[1], tmp[2]);
    return true;
  }

  //-----------------------------------------------------------------------------
  bool ParseVec4(InputBuffer& buf, vec4* res)
  {
    float tmp[4];
    CHECKED_OP(ParseVec<4>(buf, tmp));
    *res = vec4(tmp[0], tmp[1], tmp[2], tmp[3]);
    return true;
  }
#endif

  //-----------------------------------------------------------------------------
  bool ParseString(InputBuffer& buf, string* res)
  {
    char ch;
    CHECKED_OP(buf.SkipUntilOneOf("'\"", 2, &ch, true));
    size_t start = buf._idx;
    CHECKED_OP(buf.SkipUntil(ch, true));
    size_t end = buf._idx;
    return buf.SubStr(start, end - start - 1, res);
  }

  //-----------------------------------------------------------------------------
  bool ParseIdentifier(InputBuffer& buf, string* res, bool colon)
  {
    // an identifier consists of 'id:', so we parse the id, and then find the trailing ':'
    size_t start = buf._idx;
<<<<<<< Updated upstream
    size_t end;
<<<<<<< HEAD
    buf.SkipWhile(InputBuffer::IsAlphaNum, &end);
=======
    CHECKED_OP(buf.SkipWhile(InputBuffer::IsAlphaNum, &end));
=======
    buf.SkipWhile(InputBuffer::IsAlphaNum);
    size_t end = buf._idx;
>>>>>>> Stashed changes
>>>>>>> updates

    if (colon)
    {
      // find the trailing ':'
      CHECKED_OP(buf.SkipUntil(':', true));
    }

    return buf.SubStr(start, end - start, res);
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, bool value)
  {
    buf.EnsureCapacity(16);
    buf.Advance(sprintf(buf.Cur(), "%s", value ? "true" : "false"));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int value)
  {
    buf.EnsureCapacity(32);
    buf.Advance(sprintf(buf.Cur(), "%d", value));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, float value)
  {
    buf.EnsureCapacity(32);
    buf.Advance(sprintf(buf.Cur(), "%f", value));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, const string& value)
  {
    buf.EnsureCapacity(value.size() + 8);
    buf.Advance(sprintf(buf.Cur(), "'%s'", value.c_str()));
  }

#if PARSER_WITH_VECTOR_TYPES
  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, const color& value)
  {
    buf.EnsureCapacity(4 * 32);
    buf.Advance(sprintf(buf.Cur(), "{ %f, %f, %f, %f }", value.x, value.y, value.z, value.w));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, const vec2& value)
  {
    buf.EnsureCapacity(2 * 32);
    buf.Advance(sprintf(buf.Cur(), "{ %f, %f }", value.x, value.y));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, const vec3& value)
  {
    buf.EnsureCapacity(3 * 32);
    buf.Advance(sprintf(buf.Cur(), "{ %f, %f, %f }", value.x, value.y, value.z));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, const vec4& value)
  {
    buf.EnsureCapacity(4 * 32);
    buf.Advance(sprintf(buf.Cur(), "{ %f, %f, %f, %f }", value.x, value.y, value.z, value.w));
  }
#endif
  //-----------------------------------------------------------------------------
  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, bool value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 16);
    buf.Advance(sprintf(buf.Cur(), "%*s: %s;\n", len + indent, member, value ? "true" : "false"));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, int value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 32);
    buf.Advance(sprintf(buf.Cur(), "%*s: %d;\n", len + indent, member, value));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, float value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 32);
    buf.Advance(sprintf(buf.Cur(), "%*s: %f;\n", len + indent, member, value));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, const string& value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + value.size() + 32);
    buf.Advance(sprintf(buf.Cur(), "%*s: '%s';\n", len + indent, member, value.c_str()));
  }

#if PARSER_WITH_VECTOR_TYPES
  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, const color& value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 4 * 32);
    buf.Advance(sprintf(
        buf.Cur(), "%*s: { %f, %f, %f, %f };\n", len + indent, member, value.x, value.y, value.z, value.w));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, const vec2& value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 2 * 32);
    buf.Advance(sprintf(buf.Cur(), "%*s: { %f, %f };\n", len + indent, member, value.x, value.y));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, const vec3& value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 3 * 32);
    buf.Advance(
        sprintf(buf.Cur(), "%*s: { %f, %f, %f };\n", len + indent, member, value.x, value.y, value.z));
  }

  //-----------------------------------------------------------------------------
  void Serialize(OutputBuffer& buf, int indent, const char* member, const vec4& value)
  {
    int len = (int)strlen(member);
    buf.EnsureCapacity(len + indent + 4 * 32);
    buf.Advance(sprintf(
        buf.Cur(), "%*s: { %f, %f, %f, %f };\n", len + indent, member, value.x, value.y, value.z, value.w));
  }
#endif
  //-----------------------------------------------------------------------------
}
