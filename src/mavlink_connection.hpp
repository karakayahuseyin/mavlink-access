#pragma once

#include <cstring>

#include <mavsdk/mavsdk.h>
#include "mavsdk/system.h"
#include "mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h"
#include <mavsdk/plugins/telemetry/telemetry.h>

#define SYSTEM_ID           2
#define COMP_ID             2

using namespace mavsdk;

struct TelemetryData {
    double latitude;
    double longitude;
    double altitude;
    double velocity_x;
    double velocity_y;
    double velocity_z;
    double heading;
    double battery_voltage;
    double battery_percentage;
};

class MavlinkConnection
{
public:

    static MavlinkConnection* get_singleton();
    bool init(char* connection_url);
    
    char* status_text() { return _status_text; }
    

private:

    MavlinkConnection();
    ~MavlinkConnection();

    static void update_status_text(Telemetry::StatusText status_text);
    void start_telemetry();

    static MavlinkConnection* _singleton;

    Mavsdk* _mavsdk;
    std::shared_ptr<System> _vehicle;
    Telemetry* _telemetry;

    static char* _status_text;
};