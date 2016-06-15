#pragma once

//! \cond INTERNAL

/*!
 * \file HotPlugWatcherImpl.h
 * Contains HotPlugWatcher Linux implementation.
 * \warning This file is not a part of hotplug library public interface.
 */

#include <hotplug/HotPlugWatcher.h>
#include <thread>
#include <atomic>
#include <libudev.h>

/*!
 * \class HotPlugWatcher::Implementation HotPlugWatcherImpl.h "HotPlugWatcherImpl.h"
 * \brief The HotPlugWatcher::Implementation class uses udev library functionality to watch for devices events
 * \warning This class is not a part of hotplug library public interface.
 */

class HotPlugWatcher::Implementation
{
public:
	/*!
	 * \brief HotPlugWatcher::Implementation constructor
	 * \param watcher reference to the HotPlugWatcher, used to notify it's observers
	 * \param deviceType specifies the type of the devices to watch for
	 *
	 * Starts separate thread which listens to udev events.
	 */
	Implementation(const HotPlugWatcher& watcher, HotPlugWatcher::DeviceType deviceType);

	/*!
	 * \brief HotPlugWatcher destructor
	 */
	~Implementation();

private:
	void run(udev_monitor* monitor) const;

private:
	const HotPlugWatcher& _watcher;

	udev*                _udev;
	udev_monitor*        _monitor;
	int                  _pipe[2];

	std::thread          _thread;
	std::atomic<bool>    _stop;
};

//! \endcond
