#include "file_watcher.hpp"
#include "file_utils.hpp"
#include <boost/date_time/posix_time/posix_time_types.hpp>

using namespace std;
using namespace bristol;
using namespace boost::posix_time;

//------------------------------------------------------------------------------
void FileWatcher::AddFileWatch(
    const string& filename,
    void* token,
    bool initialCallback,
    bool* initialResult,
    const cbFileChanged &cb)
{
  auto it = _watchedFiles.find(filename);
  if (it == _watchedFiles.end())
  {
    // the watch isn't already present, so add it
    WatchedFile w;
    w.filename = filename;
    w.lastModification = LastModification(filename.c_str());
    it = _watchedFiles.insert(make_pair(filename, w)).first;
  }

  it->second.callbacks.push_back(make_pair(token, cb));

  if (initialCallback)
  {
    bool res = cb(filename, token);
    if (initialResult)
      *initialResult = res;
  }

}

//------------------------------------------------------------------------------
void FileWatcher::RemoveFileWatch(const cbFileChanged &cb)
{
  //   for (auto i = begin(_watchedFiles); i != end(_watchedFiles); ++i)
  //   {
  //     auto &v = i->second;
  //     v.erase(remove_if(begin(v), end(v), [&](const pair<cbFileChanged, void*> &x)
  //       { return cb == x.first; }), end(v));
  //   }
}


//------------------------------------------------------------------------------
void FileWatcher::Tick()
{
  ptime now = boost::posix_time::microsec_clock::local_time();
  if (_lastTickTime.is_not_a_date_time() || (now - _lastTickTime) > seconds(1))
  {
    for (auto& kv : _watchedFiles)
    {
      const string& filename = kv.first;
      WatchedFile& f = kv.second;
      time_t lastModification = LastModification(filename.c_str());
      if (lastModification > f.lastModification)
      {
        // file has been modified, so call all the callback
        for (const auto& p : f.callbacks)
        {
          void* token = p.first;
          const cbFileChanged& cb = p.second;
          cb(filename, token);
        }
        f.lastModification = lastModification;
      }
    }
    //
  }
}
