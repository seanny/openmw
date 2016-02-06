#ifndef OPENMW_COMPONENTS_RESOURCE_MANAGER_H
#define OPENMW_COMPONENTS_RESOURCE_MANAGER_H

#include <osg/ref_ptr>

namespace VFS
{
    class Manager;
}

namespace osgDB
{
    class ObjectCache;
}

namespace Resource
{

    /// @brief Base class for managers that require a virtual file system and object cache.
    /// @par This base class implements clearing of the cache, but populating it and what it's used for is up to the individual sub classes.
    class ResourceManager
    {
    public:
        /// @param expiryDelay how long to keep objects in cache after no longer being referenced.
        ResourceManager(const VFS::Manager* vfs, const double expiryDelay = 300.0);

        /// Clear cache entries that have not been referenced for longer than expiryDelay.
        virtual void updateCache(double referenceTime);

        /// Clear all cache entries regardless of having external references.
        virtual void clearCache();

        const VFS::Manager* getVFS() const;

    protected:
        const VFS::Manager* mVFS;
        osg::ref_ptr<osgDB::ObjectCache> mCache;
        double mExpiryDelay;
    };

}

#endif
