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
    arduino.sendStopMsg();
}

void AppManager::update( float dt )
{
    recorder.update();
    arduino.update();

    if( mAppState == AppStates::PROCESSING && recorder.getIsDoneProcessing() ) {
        setAppState( AppStates::ANIMATING );
    }
    else if( mAppState == AppStates::ANIMATING ) {
        float elapsedTime = ofGetElapsedTimef() - startAnimationTime;

        if(elapsedTime > animationTime)
            setAppState( AppStates::STOPPING );
    }
}

void AppManager::draw()
{

    arduino.drawDebug();
    recorder.drawDebug();
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
        float pos = 0.0f;
        float neg = 0.0f;

        // open json file
        if( ofFile::doesFileExist( recorder.getSentimentPath() ) ) {
            ofLogNotice() << "Found  sentiment analysis " << recorder.getSentimentPath() << ", loading";

            try {
                ofJson json = ofLoadJson( recorder.getSentimentPath() );
                // ofLogNotice() << "JSON DUMP: " << json.dump();

                if( !json["neg"].is_null() )
                    neg = json["neg"];

                if( !json["pos"].is_null() )
                    pos = json["pos"];
            }
            catch( exception &exc ) {
                ofLogError() << "Unable to load json file";
            }
        }
        else {
            ofLogError() << "Sentiment file " << recorder.getSentimentPath() << " does not exists!";
        }
        arduino.sendSerialMsg( pos, neg );

        startAnimationTime = ofGetElapsedTimef();
        break;
    }
    case AppStates::STOPPING: {
        arduino.sendStopMsg();
        setAppState( AppStates::IDLE );
        break;
    }
    default:
        break;
    }
}