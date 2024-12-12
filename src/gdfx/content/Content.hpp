//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_CONTENT_CONTENT_HPP__
#define __GDFX_CONTENT_CONTENT_HPP__

#include <string>
#include <cstdint>

namespace gdfx {

class ContentManager;

/**
* Content base class.
*
*/
class Content {
public:
    Content() : contentMgr(nullptr), path(), fullPath() {}
    virtual ~Content() {}

    virtual bool load() = 0;
    virtual void unload() = 0;

    bool reload() { unload(); return load(); }

    void setContentMgr(ContentManager *content) { contentMgr = content; }
    ContentManager *getContentMgr() { return contentMgr; }

    void setPath(const std::string& path);
    const std::string& getPath() const { return path; }
    const std::string& getFullPath() const { return fullPath; }

private:
    ContentManager *contentMgr;
    std::string path;
    std::string fullPath;
};

} // gdfx

#endif // __GDFX_CONTENT_CONTENT_HPP__
