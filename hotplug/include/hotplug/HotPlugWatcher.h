#pragma once

/*!
 * \file HotPlugWatcher.h
 * Contains HotPlugWatcher implementation - Observable that informs about device hot plug related notifications.
 */

#include "Observable.h"
#include "HotPlugNotification.h"
#include <memory>

/*!
 * \class HotPlugWatcher HotPlugWatcher.h <hotplug/HotPlugWatcher.h>
 * \brief The HotPlugWatcher class is Observable that watches for device related notifications
 *
 * HotPlugWatcher will inform registered observers about device hot plug events.
 *
 * \sa HotPlugNotificationType, HotPlugNotificationData, Observable
 */

class HotPlugWatcher : public Observable<HotPlugNotificationType, HotPlugNotificationData>
{
public:

	//! Enumeration of device types to watch for plug/unplug.
	enum DeviceType
	{
		ALL, //!< All device events will be reported.
		USB, //!< Only USB device events will be reported.
	};

public:
	/*!
	 * \brief HotPlugWatcher constructor
	 * \param deviceType specifies the type of the devices to watch for.
	 */
	explicit HotPlugWatcher(DeviceType deviceType = DeviceType::USB);
	/*!
	 * \brief ~HotPlugWatcher destructor
	 */
	virtual ~HotPlugWatcher();

private:
	class Implementation;
	std::shared_ptr<Implementation> _impl;
};
