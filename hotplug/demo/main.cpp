#include <hotplug/HotPlugWatcher.h>
#include <hotplug/HotPlugNotification.h>
#include <iostream>
#include <algorithm>
#include <unistd.h>

void printDeviceNotification(const HotPlugNotificationType& type, const HotPlugNotificationData& notification);

int main()
{
	std::cout << "Listening to USB devices 'added'/'removed' events (use Ctrl+C to stop)...\n\n";

	HotPlugWatcher usbDeviceWatcher(HotPlugWatcher::USB);
	usbDeviceWatcher.registerObserver(printDeviceNotification);

	// ctrl+c me
	while (true)
	{
		sleep(1);
	}
	return 0;
}

void printDeviceNotification(const HotPlugNotificationType& type, const HotPlugNotificationData& notification)
{
	switch (type)
	{
	case HotPlugNotificationType::DEVICE_ADDED:
		std::cout << "ADDED";
		break;
	case HotPlugNotificationType::DEVICE_REMOVED:
		std::cout << "REMOVED";
		break;
	case HotPlugNotificationType::DEVICE_CHANGED:
		std::cout << "CHANGED";
		break;
	case HotPlugNotificationType::DEVICE_MOVED:
		std::cout << "MOVED";
		break;
	default:
		std::cout << "UNKNOWN";
		break;
	}

	std::cout << " Device with properties:\n";
	std::for_each(std::begin(notification.properties), std::end(notification.properties), [](auto&& i){ std::cout << "\t" << i.first << ": " << i.second << std::endl; });
	std::cout << std::endl;
}

