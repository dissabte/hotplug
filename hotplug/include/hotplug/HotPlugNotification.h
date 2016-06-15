#pragma once

/*!
 * \file HotPlugNotification.h
 */

#include <map>
#include <string>

/*!
 * \enum HotPlugNotificationType
 * All notifications that HotPlugWatcher can inform of.
 */
enum class HotPlugNotificationType
{
	UNKNOWN,         //!< Default notification type as a placeholder for "all other notifications"
	DEVICE_ADDED,    //!< Device was added, e.g. USB flash drive was plugged in
	DEVICE_REMOVED,  //!< Device was removed
	DEVICE_CHANGED,  //!< Device property was changed
	DEVICE_MOVED     //!< Device is moved. e.g. udev rules on Linux
};

/*!
 * \class HotPlugNotificationData HotPlugNotification.h <hotplug/HotPlugNotification.h>
 * \brief The HotPlugNotificationData struct consists of a associative container of device properties
 */
struct HotPlugNotificationData
{
	std::map<std::string, std::string> properties;
};
