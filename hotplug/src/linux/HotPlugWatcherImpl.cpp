//! \cond INTERNAL

/*!
 * \file HotPlugWatcherImpl.cpp
 * \warning This file is not a part of watcher library public interface.
 *
 * Contains implementation of device hot plug events monitoring using
 * [udev](https://www.freedesktop.org/software/systemd/man/libudev.html) library.
 */

#include "HotPlugWatcherImpl.h"
#include <string>
#include <time.h>
#include <unistd.h>

HotPlugWatcher::Implementation::Implementation(const HotPlugWatcher& watcher, HotPlugWatcher::DeviceType deviceType)
    : _watcher(watcher)
    , _udev(udev_new())
    , _monitor(udev_monitor_new_from_netlink(_udev, "udev"))
    , _pipe{-1, -1}
    , _stop(false)
{
	if (_monitor != nullptr)
	{
		if (0 == pipe(_pipe))
		{
			switch (deviceType)
			{
			case HotPlugWatcher::USB:
				udev_monitor_filter_add_match_subsystem_devtype(_monitor, "usb", "usb_device");
				break;
			default: // no sepcific filter, every device event will be reported
				break;
			}

			udev_monitor_enable_receiving(_monitor);
			_thread = std::thread(&HotPlugWatcher::Implementation::run, this, _monitor);
		}
		else
		{
			perror("Couldn't create pipe for watcher");
		}
	}
}

HotPlugWatcher::Implementation::~Implementation()
{
	// set the stop flag for thread and then wake it up via pipe
	_stop = true;
	unsigned char stop = 1;
	::write(_pipe[1], &stop, sizeof(stop));
	_thread.join();

	udev_monitor_unref(_monitor);
	udev_unref(_udev);
}

void HotPlugWatcher::Implementation::run(udev_monitor* monitor) const
{
	const int monitorDescriptor = udev_monitor_get_fd(monitor);
	const int pipeReadDescriptor = _pipe[0];

	fd_set descriptorSet = {};
	FD_SET(monitorDescriptor, &descriptorSet);
	FD_SET(pipeReadDescriptor, &descriptorSet);

	const int highestDescriptor = std::max(pipeReadDescriptor, monitorDescriptor);

	while (true)
	{
		if (_stop.load())
		{
			break;
		}

		int numberOfDescriptors = select(highestDescriptor + 1, &descriptorSet, NULL, NULL, NULL);
		if (numberOfDescriptors < 0)
		{
			break;
		}
		else
		{
			if (FD_ISSET(pipeReadDescriptor, &descriptorSet))
			{
				unsigned char data = 0;
				::read(pipeReadDescriptor, &data, sizeof(data));
			}
			else if (FD_ISSET(monitorDescriptor, &descriptorSet))
			{
				udev_device* device = udev_monitor_receive_device(monitor);
				if (device)
				{
					HotPlugNotificationType notificationType;
					HotPlugNotificationData notificationData;
					notificationType = HotPlugNotificationType::UNKNOWN;
					const std::string action = udev_device_get_action(device);
					if (action == "add")
					{
						notificationType = HotPlugNotificationType::DEVICE_ADDED;
					}
					else if (action == "remove")
					{
						notificationType = HotPlugNotificationType::DEVICE_REMOVED;
					}
					else if (action == "change")
					{
						notificationType = HotPlugNotificationType::DEVICE_CHANGED;
					}
					else if (action == "move")
					{
						notificationType = HotPlugNotificationType::DEVICE_MOVED;
					}

					if (notificationType != HotPlugNotificationType::UNKNOWN)
					{
						udev_list_entry* properties = udev_device_get_properties_list_entry(device);
						if (properties)
						{
							udev_list_entry* entry = nullptr;
							udev_list_entry_foreach(entry, properties)
							{
								const char* propertyName  = udev_list_entry_get_name(entry);
								const char* propertyValue = udev_list_entry_get_value(entry);
								if (propertyName)
								{
									notificationData.properties.emplace(propertyName, propertyValue);
								}
							}
						}
						_watcher.notifyObservers(notificationType, notificationData);
					}
					udev_device_unref(device);
					device = nullptr;
				}
			}
		}
	}
}

//! \endcond
