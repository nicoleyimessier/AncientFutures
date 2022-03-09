#include "AppManager.h"

AppManager::AppManager()
{
}

AppManager::~AppManager()
{
}

void AppManager::setup()
{

    //! setup recorder
    // recorder.setup( "D:\\recordings\\" );
    recorder.setup( ofToDataPath( "recordings" ) );

    //! setup Arduino
    arduino.setup();
}

void AppManager::update( float dt )
{
    recorder.update();
    arduino.update();

    if( mAppState == AppStates::PROCESSING && recorder.getIsDoneProcessing() ) {
        setAppState( AppStates::ANIMATING );
    }
}

void AppManager::draw()
{
    recorder.drawDebug();
    arduino.drawDebug();
}

// ---- APP STAETS ---- //
void AppManager::setAppState( AppStates state )
{
    mAppState = state;
    switch( mAppState ) {
    case AppStates::IDLE: {
        break;
    }
    case AppStates::RECORDING: {
        recorder.start();
        break;
    }
    case AppStates::PROCESSING: {
        recorder.stop();
        break;
    }
    case AppStates::ANIMATING: {
        arduino.sendSerialMsg();
        break;
    }
    case AppStates::STOPPING: {
        break;
    }
    default:
        break;
    }
}