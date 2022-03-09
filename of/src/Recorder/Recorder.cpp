#include "Recorder.h"

Recorder::Recorder()
{
}

Recorder::~Recorder()
{
    soundStream.close();
}

void Recorder::onExit( ofEventArgs &e )
{
    soundStream.close();
}

void Recorder::setup( string recordingPath )
{
    setupSoundBuffer();

    ofAddListener( ofEvents().exit, this, &Recorder::onExit );

    //! Setup audio recorder
    mRecorderAud.setup( false, true, glm::vec2( 0., 0. ) );
    mRecorderAud.setAudioConfig( 1024, 44100 );
    mRecorderAud.setOverWrite( true );

    // you don't need to set this if FFMPEG is in your system path //
    // mRecorderVideo.setFFmpegPathToAddonsPath();

    //! setup recording path
    string path = recordingPath;
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    path += "\\" + ofToString( ofGetYear() );
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    path += "\\" + ofToString( ofGetMonth() );
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    path += "\\" + ofToString( ofGetDay() );
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    mRecordingPath = path + "\\";
}


void Recorder::setupSoundBuffer()
{
    ofSoundStreamSettings settings;

    /*
    auto devices = soundStream.getMatchingDevices( "Focusrite USB ASIO" );
    if( !devices.empty() )
        settings.setInDevice( devices[0] );
    */

    auto devices = soundStream.getDeviceList( ofSoundDevice::Api::MS_DS );
    if( !devices.empty() ) {
        settings.setInDevice( devices[3] );
    }

    settings.setApi( ofSoundDevice::Api::MS_DS );

    settings.setInListener( this );
    settings.sampleRate = 44100;
    settings.numInputChannels = 2;
    settings.bufferSize = 1024;
    soundStream.setup( settings );
}

void Recorder::update()
{
    if( mAudState == AudioRecordingStates::STOP ) {
        setAudioState( AudioRecordingStates::SPEECH_TO_TEXT );
    }
}

void Recorder::draw( int x, int y, float width, float height )
{
}

void Recorder::drawDebug()
{
    ofSetColor( 255, 255, 220 );
    waveform.draw();

    if( mRecorderAud.isRecording() ) {
        ofDrawBitmapStringHighlight( "audio duration: " + std::to_string( mRecorderAud.getRecordedAudioDuration( 30.0f ) ), 40, 70 );
    }

    ofPushStyle();
    {
        ofDrawBitmapStringHighlight( "FPS: " + std::to_string( ofGetFrameRate() ), 10, 16 );

        if( mRecorderAud.isRecording() ) {
            ofSetColor( ofColor::red );
        }
        else {
            ofSetColor( ofColor::green );
        }
        ofDrawCircle( ofPoint( 10, 70 ), 10 );


        // Draw the information
        ofDrawBitmapStringHighlight( "Press 'r' to start recording and 's' to stop recording.", 10, ofGetHeight() - 200 );
    }
    ofPopStyle();
}

void Recorder::start()
{

    mVisitorPath = mRecordingPath + ofGetTimestampString();
    ofDirectory::createDirectory( mVisitorPath, false );
    string vid = mVisitorPath;

    //! record audio
    setAudioState( AudioRecordingStates::RECORDING );
}

void Recorder::stop()
{
    if( !mRecorderAud.isRecording() ) {
        ofLogError() << "Cant stop recording b/c it has not started!";
        return;
    }

    //! stop audio, we need to call this in the same update loop that the thread is spawned hence
    //! the state 'prep_stop'
    setAudioState( AudioRecordingStates::PREP_STOP );
}

//--------------------------------------------------------------
void Recorder::audioIn( ofSoundBuffer &input )
{

    switch( mAudState ) {

    case AudioRecordingStates::RECORDING: {
        if( mRecorderAud.isRecording() ) {
            mRecorderAud.addBuffer( input, 30.0f );
        }
        break;
    }
    case AudioRecordingStates::PREP_STOP: {
        setAudioState( AudioRecordingStates::STOP );
        break;
    }
    default:
        break;
    }


    waveform.clear();
    for( size_t i = 0; i < input.getNumFrames(); i++ ) {
        float sample = input.getSample( i, 0 );
        float x = ofMap( i, 0, input.getNumFrames(), 0, ofGetWidth() );
        float y = ofMap( sample, -1, 1, 0, ofGetHeight() );
        waveform.addVertex( x, y );
    }
}

void Recorder::setAudioState( AudioRecordingStates state )
{
    mAudState = state;

    switch( mAudState ) {
    case AudioRecordingStates::IDLE: {
        resetVisitorPath();
        break;
    }
    case AudioRecordingStates::RECORDING: {
        // mVisitorAudioPath = mVisitorPath + "\\audio.mp3";
        mVisitorAudioPath = mVisitorPath + "\\audio.wav";
        mRecorderAud.setAudOutputPath( mVisitorAudioPath );
        mRecorderAud.startCustomAudioRecord();
        break;
    }
    case AudioRecordingStates::PREP_STOP: {
        break;
    }
    case AudioRecordingStates::STOP: {
        if( mRecorderAud.isRecording() )
            mRecorderAud.stop();

        break;
    }
    case AudioRecordingStates::SPEECH_TO_TEXT: {
        translateSpeechToText();
        break;
    }
    case AudioRecordingStates::SENTIMENT_ANALYSIS: {
        performSentimentAnalysis();
        break;
    }
    default:
        break;
    }
}

void Recorder::translateSpeechToText()
{
    ofLogNotice() << "TRANSCRIBE AUDIO FILE";
    string batPath = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\AncientFutures\\scripts\\transcribe.bat";
    string keyPath = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\ancient-futures-343119-c8149bde1b51.json";
    // string audioPath = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\ManualTest.wav";

    string audioPath = mRootPath + mVisitorAudioPath;

    ofLogNotice() << "Audio path: " << audioPath;

    translation = ofSystem( batPath + " " + keyPath + " " + audioPath );

    translation.erase( remove( translation.begin(), translation.end(), '\n' ), translation.end() ); 


    ofLogNotice() << "Speech-to-text Result: " << translation;

    // NEED TO DO: save transcription to file
    ofJson text, json;
    text["text"] = translation; 
    json.push_back( text );

    string textFile = mRootPath + mVisitorPath + "\\translation.json";
    ofSaveJson( textFile, json );
    setAudioState( AudioRecordingStates::SENTIMENT_ANALYSIS );
}

void Recorder::performSentimentAnalysis()
{
    ofLogNotice() << "PERFORM SENTIMENT ANALYSIS";
    string pythonFile = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\AncientFutures\\nlk\\nlk_sentiment.py";
    string textFile = mRootPath + mVisitorPath + "\\sentiment.json";
    string path = ofSystem( "echo %path%" );
    string cmd = "set PATH=" + path;
    // ofLogNotice() << cmd;
    ofSystem( cmd );
    cmd = "python " + pythonFile + " " + "\"" + translation + "\"" + " " + "\"" + textFile + "\"";
    ofLogNotice() << cmd;
    string result = ofSystem( cmd );
    ofLogNotice() << "Sentiment Analysis Result: " << result;

    setAudioState( AudioRecordingStates::IDLE );
}

bool Recorder::getIsDoneProcessing()
{

    if( mAudState == AudioRecordingStates::IDLE )
        return true;
    else
        return false;
}