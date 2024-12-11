//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
#ifndef __GDFX_CONTENT_CONTENTMANAGER_HPP__
#define __GDFX_CONTENT_CONTENTMANAGER_HPP__

#include <string>
#include <memory>
#include <unordered_map>

#include <gdfx/content/Content.hpp>
#include <gdfx/content/ContentLoadException.hpp>

namespace gdfx {

class Graphics;

/**
* ContentManager
*/
class ContentManager {
public:
    ContentManager() : resources() {}
    ~ContentManager() {}

    // loads a new or existing content item
    template <typename T, typename... Args>
    std::shared_ptr<T> load(const std::string& path)
    {
        // check to see if the content item is already loaded by converting weak ptr into shared ptr (may return null)
        auto resource = std::dynamic_pointer_cast<T>(resources[path].lock());
        
        if (!resource) {
            T *p = T::create();
            if (!p)
                return nullptr;

            p->setContentMgr(this);
            p->setPath(path);
            
            if (!p->load()) {
                delete p; // load failed, so delete the pointer
                throw ContentLoadException(path);
            }

            resource = std::shared_ptr<T>(p);
            resources[path] = resource;
        }

        return resource;
    }

    void setGraphics(Graphics *g) { graphics = g; }
    Graphics *getGraphics() { return graphics; }

private:
    std::unordered_map<std::string, std::weak_ptr<Content>> resources;
    Graphics *graphics;
};

} // gdfx

#endif // __GDFX_CONTENT_CONTENTMANAGER_HPP__
