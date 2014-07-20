#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "time_utils.hpp"

namespace bristol
{

  class FileWatcher
  {
  public:

    typedef std::function<bool(const std::string&, void *)> cbFileChanged;

    void Tick();

    void AddFileWatch(
        const std::string& filename,
        void* token,
        bool initialCallback,
        bool* initialResult,
        const cbFileChanged &cb);

    void RemoveFileWatch(const cbFileChanged &cb);

  private:

    struct WatchedFile
    {
      std::string filename;
      time_t lastModification;
      std::vector<std::pair<void*, cbFileChanged>> callbacks;
    };

    TimeStamp _lastTickTime;
    std::vector<std::string> _paths;
    std::unordered_map<std::string, WatchedFile> _watchedFiles;

  };
}