#pragma once
#include <vector>
#include <deque>
namespace parser
{

  struct ParseExpcetion : std::exception
  {
    ParseExpcetion(const char* e) : std::exception(e) {}
  };

#define CHECKED_OP(x)                                                                                        \
  do                                                                                                         \
  {                                                                                                          \
    if (!(x))                                                                                                \
      return false;                                                                                          \
  } while (false);

  struct InputBuffer
  {
    InputBuffer();
    InputBuffer(const char* buf, size_t len);
    InputBuffer(const std::vector<char>& buf);
    char Peek();
    void Rewind(size_t len);
    char Get();
    bool OneOf(const char* str, size_t len, char* res);
    bool OneOfIdx(const char* str, size_t len, int* res);
    bool Expect(char ch);
    bool SkipUntil(char ch, bool consume);
    bool SkipUntilOneOf(const char* str, size_t len, char* res, bool consume);
<<<<<<< HEAD
    void SkipWhile(const std::function<bool(char)>& fn, size_t* end);
=======
<<<<<<< Updated upstream
    bool SkipWhile(const std::function<bool(char)>& fn, size_t* end);
>>>>>>> updates
    bool Satifies(const std::function<bool(char)>& fn, char* ch);
=======
    void SkipWhile(const std::function<bool(char)>& fn);
    bool Satifies(const std::function<bool(char)>& fn, char* out);
>>>>>>> Stashed changes
    void SkipWhitespace();
    bool SubStr(size_t start, size_t len, std::string* res);

    static bool IsDigit(char ch);
    static bool IsAlphaNum(char ch);

    void Consume();
    bool ConsumeIf(char ch);
    bool Eof();

    bool InnerScope(const char* delim, InputBuffer* scope);

    void SaveState();
    void RestoreState(bool onlyPop=false);

    std::deque<size_t> _saveStack;

    const char* _buf;
    size_t _idx;
    size_t _len;
  };
}
