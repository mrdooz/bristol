#include "input_buffer.hpp"

using namespace std;

namespace parser
{


  //-----------------------------------------------------------------------------
  InputBuffer::InputBuffer() : _buf(nullptr), _idx(0), _len(0) {}

  //-----------------------------------------------------------------------------
  InputBuffer::InputBuffer(const char* buf, size_t len) : _buf(buf), _idx(0), _len(len) {}

  //-----------------------------------------------------------------------------
  InputBuffer::InputBuffer(const vector<char>& buf) : _buf(buf.data()), _idx(0), _len(buf.size()) {}

  //-----------------------------------------------------------------------------
  char InputBuffer::Peek()
  {
    if (Eof())
      throw exception("Peek at EOF");

    return _buf[_idx];
  }

  //-----------------------------------------------------------------------------
  void InputBuffer::Rewind(size_t len) { _idx = len < _idx ? _idx - len : 0; }

  //-----------------------------------------------------------------------------
  char InputBuffer::Get()
  {
    if (Eof())
      throw ParseExpcetion("Get at EOF");

    return _buf[_idx++];
  }

  //-----------------------------------------------------------------------------
  void InputBuffer::Consume()
  {
    if (Eof())
      throw ParseExpcetion("Get at EOF");

    ++_idx;
  }

  //-----------------------------------------------------------------------------
<<<<<<< Updated upstream
  bool InputBuffer::ConsumeIf(char ch, bool* res)
  {    
    // Testing at EOF is ok, and will always succeed
    if (Eof())
      return true;

    char tmp = Peek();

    bool localRes = ch == tmp;
    if (res)
      *res = localRes;
=======
  bool InputBuffer::ConsumeIf(char ch)
  {    
    if (Eof())
      return false;
>>>>>>> Stashed changes

    bool consume = Peek() == ch;
    if (consume)
      Consume();
    return consume;
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::Eof() { return _idx == _len; }

  //-----------------------------------------------------------------------------
  bool InputBuffer::OneOf(const char* str, size_t len, char* res)
  {
    *res = 0;
    int idx;
    if (!OneOfIdx(str, len, &idx))
      return false;

    if (idx == -1)
      return true;

    *res = str[idx];
    return true;
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::OneOfIdx(const char* str, size_t len, int* res)
  {
    *res = -1;
    char ch = Peek();

    for (size_t i = 0; i < len; ++i)
    {
      if (ch == str[i])
      {
        Consume();
        *res = (int)i;
        break;
      }
    }
    return true;
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::Satifies(const function<bool(char)>& fn, char* out)
  {
<<<<<<< HEAD
    // TODO: handle eof
    char ch = Peek();
=======
<<<<<<< Updated upstream
    char ch;
    CHECKED_OP(Peek(&ch));
>>>>>>> updates

    if (res)
      *res = ch;

    bool tmp = fn(ch);
    if (!tmp)
=======
    if (Eof())
>>>>>>> Stashed changes
      return false;

    char ch = Get();
    *out = ch;
    return fn(ch);
  }

  //-----------------------------------------------------------------------------
  void InputBuffer::SkipWhitespace()
  {
    while (true)
    {
      char ch = _buf[_idx];
      if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
      {
        _idx++;
        if (_idx == _len)
          return;
      }
      else
      {
        return;
      }
    }
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::Expect(char ch)
  {
    return Get() == ch;
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::IsDigit(char ch) { return !!isdigit(ch); }

  //-----------------------------------------------------------------------------
  bool InputBuffer::IsAlphaNum(char ch) { return !!isalnum(ch) || ch == '_'; }

  //-----------------------------------------------------------------------------
  bool InputBuffer::SkipUntil(char ch, bool consume)
  {
    while (!Eof())
    {
      char tmp = Peek();
      if (tmp == ch)
      {
        if (consume)
          Consume();
        return true;
      }
    }

    return false;
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::SkipUntilOneOf(const char* str, size_t len, char* res, bool consume)
  {
    while (!Eof())
    {
      char tmp = Peek();
      for (size_t i = 0; i < len; ++i)
      {
        if (tmp == str[i])
        {
          if (res)
            *res = tmp;
          if (consume)
            Consume();
          return true;
        }
      }
    }
    return false;
  }

  //-----------------------------------------------------------------------------
<<<<<<< HEAD
  void InputBuffer::SkipWhile(const function<bool(char)>& fn, size_t* end)
=======
<<<<<<< Updated upstream
  bool InputBuffer::SkipWhile(const function<bool(char)>& fn, size_t* end)
=======
  void InputBuffer::SkipWhile(const function<bool(char)>& fn)
>>>>>>> Stashed changes
>>>>>>> updates
  {
    while (!Eof())
    {
      char ch = Peek();
      if (!fn(ch))
        return;
      Consume();
    }
<<<<<<< Updated upstream
    *end = _idx;
<<<<<<< HEAD
=======
    return true;
=======
>>>>>>> Stashed changes
>>>>>>> updates
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::SubStr(size_t start, size_t len, string* res)
  {
    if (start >= _len || start + len > _len)
      return false;

    res->assign(&_buf[start], len);
    return true;
  }

  //-----------------------------------------------------------------------------
  bool InputBuffer::InnerScope(const char* delim, InputBuffer* scope)
  {
    // Note, the scope contains both the delimiters
    char open = delim[0];
    char close = delim[1];

    const char* start = &_buf[_idx];
    const char* end = &_buf[_idx + _len];
    const char* cur = start;

    // find opening delimiter
    while (cur < end)
    {
      if (*cur++ == open)
        break;
    }

    if (cur == end)
      return false;

    scope->_buf = cur - 1;
    scope->_idx = 0;

    // find closing
    int cnt = 0;
    while (cur < end)
    {
      char ch = *cur;
      if (ch == close)
      {
        if (cnt == 0)
        {
          // found the scope
          scope->_len = cur - scope->_buf + 1;
          return true;
        }
        else
        {
          --cnt;
        }
      }
      else if (ch == open)
      {
        ++cnt;
      }
      ++cur;
    }
    return false;
  }

  //-----------------------------------------------------------------------------
  void InputBuffer::SaveState()
  {
    _saveStack.push_back(_idx);
  }

  //-----------------------------------------------------------------------------
  void InputBuffer::RestoreState(bool onlyPop)
  {
    if (!onlyPop)
    {
      _idx = _saveStack.back();
    }
    _saveStack.pop_back();
  }
}
