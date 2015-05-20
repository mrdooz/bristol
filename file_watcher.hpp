#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "time_utils.hpp"

namespace bristol
{
  class FileWatcher
  {
  public:
    typedef std::function<bool(const std::string&, void *)> cbFileChanged;
    typedef uint32_t WatchId;

    FileWatcher();
    void Tick();

    WatchId AddFileWatch(
        const std::string& filename,
        void* token,
        bool initialCallback,
        bool* initialResult,
        const cbFileChanged &cb);

    void RemoveFileWatch(WatchId id);

  private:

    struct CallbackContext
    {
      void* token;
      cbFileChanged cb;
      WatchId id;
    };

    struct WatchedFile
    {
      std::string filename;
      time_t lastModification;
      WatchId id;
      std::vector<CallbackContext> callbacks;
    };

    TimeStamp _lastTickTime;
    std::vector<std::string> _paths;
    //std::unordered_map<std::string, std::shared_ptr<WatchedFile>> _watchedFiles;
    std::vector<std::shared_ptr<WatchedFile>> _watchedFiles;
    std::unordered_map<WatchId, std::shared_ptr<WatchedFile>> _idToFile;
    uint32_t _nextId;

    uint32_t _watchFileOfs = 0;
  };
}
