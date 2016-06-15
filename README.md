# hotplug library
## General information
Library provides tools to watch for devices hot plug notifications (e.g. plugging in/out USB device).

## Pre-requisites
To build hotplug I use [cmake](https://cmake.org)<br>

## Documenation
The code has doxygen-compatible comments and there's configuration file for doxygen available as well. To generate goxygen documentation one can do following:
~~~bash
# assuming you are in a build folder located next to hotplug repository:
cmake ../hotplug/hotplug/ && cmake --build . --target hotplug_docs
~~~

And in case you want to look at internal documentation (i.e. the one for library private parts) just add `-DDOXYGEN_SECTIONS=INTERNAL`:<br>

~~~bash
# assuming you are in a build folder located next to hotplug repository:
cmake -DDOXYGEN_SECTIONS=INTERNAL ../hotplug/hotplug/ && cmake --build . --target hotplug_docs
~~~

## Platform support
- Linux - supported
- OS X - not supported yet
- Windows - not supported yet

## Usage examples
Following code will print all device information provided by the system when device will be hot plugged in.
~~~c++
HotPlugWatcher watcher;
watcher.registerObserver(HotPlugNotificationType::DEVICE_ADDED, [](const HotPlugNotificationData& data)
{
	std::cout << "Plugged in device with following properties:\n";
	std::for_each(std::begin(data.properties),
	              std::end(data.properties),
	              [](const std::pair<std::string, std::string>& i)
	              {
	                  std::cout << "\t" << i.first << ": " << i.second << std::endl;
	              });
	std::cout << std::endl;
});
~~~
Check hotplug [demo](https://github.com/dissabte/hotplug/blob/master/hotplug/tests/demo/main.cpp) app for more info.

