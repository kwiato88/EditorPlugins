#include "Navigator.hpp"

namespace CTagsPlugin
{

bool operator==(const Location& p_lhs, const Location& p_rhs)
{
    return p_lhs.filePath == p_rhs.filePath && p_lhs.lineNumber == p_rhs.lineNumber;
}

Navigator::Navigator(std::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
	                 std::shared_ptr<Plugin::ILocationGetter> p_locationGetter)
 : m_locations(),
   m_currentLocationIndex(-1),
   m_locationSetter(p_locationSetter),
   m_locationGetter(p_locationGetter)
{
}

void Navigator::goToNextLocation()
{
    if (canGoToNextLocation())
        goTo(++m_currentLocationIndex);
}

bool Navigator::canGoToNextLocation() const
{
    return !m_locations.empty()
           && m_currentLocationIndex < m_locations.size() - 1;
}

void Navigator::goToPreviousLocaton()
{
    if (canGoToPreviousLocation())
        goTo(--m_currentLocationIndex);
}

bool Navigator::canGoToPreviousLocation() const
{
    return !m_locations.empty() && m_currentLocationIndex > 0;
}

void Navigator::goToCurrentLocation()
{
	if(isCurrentLocationIndexValid())
		goTo(m_currentLocationIndex);
}

bool Navigator::isCurrentLocationIndexValid() const
{
	return m_currentLocationIndex < m_locations.size()
		&& m_currentLocationIndex >= 0
		&& !m_locations.empty();
}

void Navigator::goTo(unsigned int p_locationIndex)
{
    Location l_location = m_locations[p_locationIndex];
    m_locationSetter->setFile(l_location.filePath);
    m_locationSetter->setLine(l_location.lineNumber);
    m_locationSetter->setColumn(l_location.columNumber);
}

void Navigator::clear()
{
    m_locations.clear();
    m_currentLocationIndex = -1;
}

void Navigator::addLocation(const Location& p_location)
{
    deleteNextLocations();
    if(!isLoctionOnQueueTop(p_location))
    {
        addLocationOnTop(p_location);
        cutLocationsQueue();
    }
}

void Navigator::addLocationOnTop(const Location& p_location)
{
    m_locations.push_back(p_location);
    ++m_currentLocationIndex;
}

void Navigator::deleteNextLocations()
{
    if(!m_locations.empty())
        m_locations.erase(m_locations.begin() + m_currentLocationIndex + 1, m_locations.end());
}

bool Navigator::isLoctionOnQueueTop(const Location& p_location) const
{
    return !m_locations.empty() && p_location == m_locations.back();
}

void Navigator::cutLocationsQueue()
{
    if(m_locations.size() > MAX_LOCATIONS_QUEUE_LENGTH)
    {
        m_currentLocationIndex -= LOCATIONS_AMOUND_TO_CUT;
        m_locations.erase(m_locations.begin(), m_locations.begin() + LOCATIONS_AMOUND_TO_CUT);
    }
}

} // namespace CTagsPlugin
