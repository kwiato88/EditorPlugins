#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP

#include <vector>
#include <memory>

#include "Editor.hpp"

namespace CTagsPlugin
{

struct Location
{
    std::string filePath;
    int lineNumber;
    int columNumber;
};

bool operator==(const Location& p_lhs, const Location& p_rhs);

class Navigator
{
public:
    Navigator(Plugin::Editor& p_editor);

    /*
     * @throws: LocationSetterException
     */
    void goToNextLocation();
    /*
     * @thrown: LocationSetterException
     */
    void goToPreviousLocaton();
    void goToCurrentLocation();
    void addLocation(const Location& p_location);
    void clear();
                                                                      
private:
    static const int MAX_LOCATIONS_QUEUE_LENGTH = 64;
    static const int LOCATIONS_AMOUND_TO_CUT = 10;

    bool canGoToNextLocation() const;
    bool canGoToPreviousLocation() const;
	bool isCurrentLocationIndexValid() const;
    void goTo(unsigned int p_locationIndex);
    bool isLoctionOnQueueTop(const Location& p_location) const;
    void deleteNextLocations();
    void addLocationOnTop(const Location& p_location);
    void cutLocationsQueue();

	Plugin::Editor& m_editor;

    std::vector<Location> m_locations;
    int m_currentLocationIndex;
};

} // namespace CTagsPlugin

#endif // NAVIGATOR_HPP
