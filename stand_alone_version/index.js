const speech = require("@google-cloud/speech");
const recorder = require('node-record-lpcm16');
const osc = require("node-osc");

process.env.GOOGLE_APPLICATION_CREDENTIALS = "./speech-to-text-key.json";


// ----------------------------------------------------------------------------------------------
// OSC
// ----------------------------------------------------------------------------------------------
const oscclient = new osc.Client('127.0.0.1', 5150);
const oscServer = new osc.Server(5000, '127.0.0.1', () => {
  console.log('OSC Server is listening');
});

oscServer.on('message', function (msg) {
  console.log(`Message: `, msg);

  if(msg.length >= 2)
  {
    if(msg[0] == "app")
    {
      if(msg[1] == "record")
      {
        //console.log(`record`);
        startRecording();
      }
      else if(msg[1] == "stopRecording")
      {
        //console.log(`stop recording`);
        stopRecording();
      }
    }
  }
  
});

// =========================== GOOGLE CLOUD SETTINGS ================================ //
// The encoding of the audio file, e.g. 'LINEAR16'
// The sample rate of the audio file in hertz, e.g. 16000
// The BCP-47 language code to use, e.g. 'en-US'
const encoding = "LINEAR16";
const sampleRateHertz = 16000;
const speech_config = {
  config: {
    encoding: encoding,
    sampleRateHertz: sampleRateHertz,
    languageCode: "en-US",
    enableWordTimeOffsets: true,
    enableAutomaticPunctuation: true,
    enableWordConfidence: true,
    enableSpeakerDiarization: true,
    diarizationSpeakerCount: 2,
    model: "latest_long",
    useEnhanced: true,
  },
  interimResults: true,
};

// Creates a speech to text client
let client = new speech.SpeechClient();

//Create the recorder
let recording = null;

//Create the stream
let recognizeStream = null;

// create string to hold final transcript
let transcript = "";


function startRecording() {
  recognizeStream = client
    .streamingRecognize(speech_config)
    .on("data", data => {
      const result = data.results[0];
      const isFinal = result.isFinal;
      transcript = result.alternatives[0].transcript;
      console.log(data.results[0].alternatives[0].transcript);
      
      if(data.results[0].alternatives[0].confidence > 0)
      {
        oscclient.send('/ancient_futures', result.alternatives[0].transcript, isFinal, () => {
          console.log("send over osc done: ", transcript);
        });
      }
    });
  
  recording = recorder.record({
      sampleRateHertz: sampleRateHertz,
      threshold: 0,
      verbose: false,
      recordProgram: 'sox',
      silence: '10.0',
      device: "Microphone (Realtek High Definition Audio)"
    })
  recording
    .stream({verbose: true})
    .on('error', console.error)
    .pipe(recognizeStream);
}

function stopRecording() {

  if(recording)
    recording.stop();

  if(recognizeStream)
    recognizeStream.end();

  recording = null;
  recognizeStream = null;
}

function reset() {
 stopRecording();
}

//startRecording();

//setInterval(reset, 300000);

console.log('Listening, press Ctrl+C to stop.');