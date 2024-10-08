#include <mavlink_connection.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace mavsdk;

MavlinkConnection* MavlinkConnection::_singleton = nullptr;
char* MavlinkConnection::_status_text = nullptr;


MavlinkConnection::MavlinkConnection() {}

MavlinkConnection::~MavlinkConnection() {}


MavlinkConnection* MavlinkConnection::get_singleton() 
{
    if (_singleton == nullptr) {
        _singleton = new MavlinkConnection();
    }
    return _singleton;
}

bool MavlinkConnection::init(char* connection_url) 
{
    _mavsdk = new Mavsdk(Mavsdk::Configuration(SYSTEM_ID, COMP_ID, true));
    ConnectionResult connection_result;
    connection_result = _mavsdk->add_any_connection(connection_url);

    if (connection_result != ConnectionResult::Success) {
        return false;
    }

    while (_mavsdk->systems().size() == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (auto system : _mavsdk->systems()) {
        // create MavlinkPassthrough instances for vehicle and ground station
        if (system->has_autopilot()) {
            if (system->is_connected()) {
                _vehicle = system;
            }
        }
    }

    start_telemetry();
    return true;
}

void MavlinkConnection::start_telemetry()
{
    if (_vehicle == nullptr) {
        return;
    }
    
    _telemetry = new Telemetry(_vehicle);
    _telemetry->subscribe_status_text(update_status_text);
}

void MavlinkConnection::update_status_text(Telemetry::StatusText status_text)
{
    // Free the previous memory if _status_text already points to something
    if (_status_text != nullptr) {
        delete[] _status_text;
    }

    std::string str = status_text.text;

    // Allocate memory for the new status text
    _status_text = new char[str.size() + 1]; // +1 for the null terminator

    // Copy the content of status_text.text to _status_text
    std::strcpy(_status_text, str.c_str());
}