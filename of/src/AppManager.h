#pragma once

#include "Arduino.h"
#include "Recorder.h"
#include "ofMain.h"


class AppManager {

  public:
    AppManager();
    ~AppManager();

    void setup();
    void update( float dt );
    void draw();

    // App states
    enum class AppStates { IDLE, RECORDING, PROCESSING, ANIMATING, STOPPING, NUM_STATES };
    void setAppState( AppStates state );

  private:
    // App states
    AppStates mAppState{ AppStates::IDLE };

    // Recorder
    Recorder recorder;

    // Arduino + Serial Comms
    Arduino arduino;
};