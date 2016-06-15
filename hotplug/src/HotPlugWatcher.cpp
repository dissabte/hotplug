//! \cond INTERNAL

#include "../include/hotplug/HotPlugWatcher.h"

/*!
 * \file HotPlugWatcher.cpp
 * Contains HotPlugWatcher implementation.
 */


#ifdef __linux__
#include "linux/HotPlugWatcherImpl.h"
#else
#error This OS is not supported yet.
#endif

HotPlugWatcher::HotPlugWatcher(HotPlugWatcher::DeviceType deviceType /*= DeviceType::USB_ALL*/)
    : Observable()
    , _impl(new Implementation(*this, deviceType))
{
}

HotPlugWatcher::~HotPlugWatcher()
{
}

//! \endcond
