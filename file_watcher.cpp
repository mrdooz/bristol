#include <algorithm>
#include "file_watcher.hpp"
#include "file_utils.hpp"

using namespace std;
using namespace bristol;

//------------------------------------------------------------------------------
FileWatcher::FileWatcher()
    : _nextId(1)
{
}

//------------------------------------------------------------------------------
FileWatcher::WatchId FileWatcher::AddFileWatch(
    const string& filename,
    void* token,
    bool initialCallback,
    bool* initialResult,
    const cbFileChanged &cb)
{
  auto it = _watchedFiles.find(filename);
  shared_ptr<WatchedFile> w;
  if (it == _watchedFiles.end())
  {
    // the watch isn't already present, so add it
    w = make_shared<WatchedFile>(WatchedFile{ filename, LastModification(filename.c_str()) });
    _watchedFiles.insert(make_pair(filename, w));
  }
  else
  {
    w = it->second;
  }

  WatchId id = _nextId++;
  w->callbacks.push_back({token, cb, id});
  _idToFile[id] = w;

  // Check if we should invoke the callback directly
  if (initialCallback)
  {
    bool res = cb(filename, token);
    if (initialResult)
      *initialResult = res;
  }

  return id;
}

//------------------------------------------------------------------------------
void FileWatcher::RemoveFileWatch(WatchId id)
{
  auto it = _idToFile.find(id);
  if (it == _idToFile.end())
    return;

  shared_ptr<WatchedFile>& w = it->second;
  auto& callbacks = w->callbacks;
  callbacks.erase(
    std::remove_if(callbacks.begin(), callbacks.end(), [=](const CallbackContext& ctx) { return ctx.id == id; }), 
    callbacks.end());
}

//------------------------------------------------------------------------------
void FileWatcher::Tick()
{
  TimeStamp now = TimeStamp::Now();
  if (!_lastTickTime.IsValid() || (now - _lastTickTime) > TimeDuration::Seconds(1))
  {
    for (auto& kv : _watchedFiles)
    {
      const string& filename = kv.first;
      shared_ptr<WatchedFile>& f = kv.second;
      time_t lastModification = LastModification(filename.c_str());
      if (lastModification > f->lastModification)
      {
        // file has been modified, so call all the callbacks
        for (const CallbackContext& ctx : f->callbacks)
        {
          ctx.cb(filename, ctx.token);
        }

        f->lastModification = lastModification;
      }
    }
  }
}
