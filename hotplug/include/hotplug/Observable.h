#pragma once

/*!
 * \file Observable.h
 * Contains template class Observable implementation.
 */

#include <map>
#include <vector>
#include <algorithm>
#include <functional>

/*!
 * \class Observable Observable.h <hotplug/Observable.h>
 * \brief Observer design pattern implementation
 *
 * Observable represents a Subject class in terms of Observer design pattern
 * (see https://en.wikipedia.org/wiki/Observer_pattern)
 *
 * Template parameters are actually notification type and the notification data type.
 *
 * Observers (std::function objects) are of two types:
 * - Specific, which are registered for certain notification and they receive only notification data;
 * - Generic, which are registered for all notifications and they receive both notification and notification data.
 *
 * \warning Class is not thread safe. registerObserver(), unregisterObserver() and notifyObservers() should be guarded externally.
 */

template <typename Notification, typename NotificationData>
class Observable
{
public:
	//! Type alias for specific observer
	using SpecificObserver = std::function<void(const NotificationData&)>;

	//! Type alias for generic observer
	using GenericObserver  = std::function<void(const Notification&, const NotificationData&)>;

public:
	explicit Observable() : _currentId(0) {}
	virtual ~Observable() {}

	/*!
	 * \brief Registers observer for specified notification and returns observer id
	 * \param [in] notification specific notification to register observer for.
	 * \param [in] observer observer object that will be called when notification is fired.
	 * \return Observer id. This id can be used to unregister observer when it's not needed anymore.
	 * \sa unregisterObserver(), notifyObservers()
	 *
	 * Observer object is copied.
	 */
	unsigned long long registerObserver(const Notification& notification, const SpecificObserver& observer);

	/*!
	 * \brief Registers observer for specified notification and returns observer id
	 * \param [in] notification specific notification to register observer for.
	 * \param [in] observer observer object that will be called when notification is fired.
	 * \return Observer id. This id can be used to unregister observer when it's not needed anymore.
	 * \sa unregisterObserver(), notifyObservers()
	 *
	 * Observer object is moved.
	 */
	unsigned long long registerObserver(const Notification& notification, SpecificObserver&& observer);

	/*!
	 * \brief Registers observer and returns observer id
	 * \param [in] observer observer object that will be called when notification is fired.
	 * \return Observer id. This id can be used to unregister observer when it's not needed anymore.
	 * \sa unregisterObserver(), notifyObservers()
	 *
	 * Observer will be called for all notifications. Notification is passed as the parameter as well as
	 * notification data. Observer object is copied.
	 */
	unsigned long long registerObserver(const GenericObserver& observer);

	/*!
	 * \brief Registers observer and returns observer id
	 * \param [in] observer observer object that will be called when notification is fired.
	 * \return Observer id. This id can be used to unregister observer when it's not needed anymore.
	 * \sa unregisterObserver(), notifyObservers()
	 *
	 * Observer will be called for all notifications. Notification is passed as the parameter as well as
	 * notification data. Observer object is moved.
	 */
	unsigned long long registerObserver(GenericObserver&& observer);

	/*!
	 * \brief Unregisters observer
	 * \param [in] observerId id of observer that will be unregistered
	 * \sa registerObserver(), notifyObservers()
	 */
	void unregisterObserver(const unsigned long long observerId);

	/*!
	 * \brief Notifies registered observer about fired notification
	 * \param [in] notification notification that registered observers will be informed of
	 * \param [in] data notification data that will be passed to the observers
	 * \sa registerObserver(), unregisterObserver()
	 *
	 * Specific observers (i.e. observers registered for certain notification) will be notified before generic ones.
	 */
	void notifyObservers(const Notification& notification, const NotificationData& data) const;

private:
	/*!
	 * \internal Observers could be placed inside union to save some space, but that requires more code
	 * and I think will decrease general readability.
	 */
	struct ObserverWithId
	{
		unsigned long long id;
		SpecificObserver specificObserver;
		GenericObserver  genericObserver;
	};

private:
	/*!
	 * \internal These two containers could be combined in one, but I don't want to add some requirements on Notification.
	 */
	std::map<Notification, std::vector<ObserverWithId>> _specificObservers; //! associative container of specific observers
	std::vector<ObserverWithId>                         _genericObservers;  //! container of generic observers
	unsigned long long                                  _currentId;         //! helper integer field to generate observer id
};


template <typename Notification, typename NotificationData>
unsigned long long Observable<Notification, NotificationData>::registerObserver(const Notification& notification, const Observable::SpecificObserver& observer)
{
	_specificObservers[notification].push_back({++_currentId, observer, GenericObserver()});
	return _currentId;
}

template <typename Notification, typename NotificationData>
unsigned long long Observable<Notification, NotificationData>::registerObserver(const Notification& notification, Observable::SpecificObserver&& observer)
{
	_specificObservers[notification].push_back({++_currentId, std::move(observer), GenericObserver()});
	return _currentId;
}

template <typename Notification, typename NotificationData>
unsigned long long Observable<Notification, NotificationData>::registerObserver(const Observable::GenericObserver& observer)
{
	_genericObservers.push_back({++_currentId, SpecificObserver(), observer});
	return _currentId;
}

template <typename Notification, typename NotificationData>
unsigned long long Observable<Notification, NotificationData>::registerObserver(Observable::GenericObserver&& observer)
{
	_genericObservers.push_back({++_currentId, SpecificObserver(), std::move(observer)});
	return _currentId;
}

template <typename Notification, typename NotificationData>
void Observable<Notification, NotificationData>::unregisterObserver(const unsigned long long observerId)
{
	const auto observerIdMatchFunctor = [&observerId](const auto& observer) -> bool {return observer.id == observerId;};

	const auto specificObserver = std::find_if(std::begin(_specificObservers), std::end(_specificObservers), [&observerId, &observerIdMatchFunctor](const auto& pair)
	{
		const auto& observers = pair.second;
		const auto observer = std::find_if(std::begin(observers), std::end(observers), observerIdMatchFunctor);
		return observer != std::end(observers);
	});

	if (specificObserver != std::end(_specificObservers))
	{
		_specificObservers.erase(specificObserver);
	}
	else
	{
		const auto observer = std::find_if(std::begin(_genericObservers), std::end(_genericObservers), observerIdMatchFunctor);
		if (observer != std::end(_genericObservers))
		{
			_genericObservers.erase(observer);
		}
	}
}

template <typename Notification, typename NotificationData>
void Observable<Notification, NotificationData>::notifyObservers(const Notification& notification, const NotificationData& data) const
{
	if (_specificObservers.count(notification))
	{
		const auto& specificObservers = _specificObservers.at(notification);
		for (const ObserverWithId& observer : specificObservers)
		{
			observer.specificObserver(data);
		}
	}

	for (const ObserverWithId& observer : _genericObservers)
	{
		observer.genericObserver(notification, data);
	}
}
