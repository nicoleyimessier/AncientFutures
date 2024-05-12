
## Set up

1. Add `sox` to your environment variables paths: `C:\ancient_futures\AncientFutures\stand_alone_version\sox-14.4.1`
2. Open Windows Power Shell in Admin Mode
3. Run `Install-Module -Name AudioDeviceCmdlets`
4. Run `Get-AudioDevice -List` to find the microphone device (i.e. `Microphone (Realtek High Definition Audio)`)
5. Add your Google Key to `C:\ancient_futures\AncientFutures\stand_alone_version` with name `speech-to-text-key.json`
6. cd `C:\ancient_futures\AncientFutures\stand_alone_version`
7. Run `npm i`
8. Run `node index.js`


## Links & Notes

https://sourceforge.net/projects/sox/files/sox/14.4.1/sox-14.4.1-win32.zip/download

https://github.com/frgnca/AudioDeviceCmdlets

### OPTIONAL
VIRTUAL CABLE

https://vb-audio.com/Cable/

npm i @google-cloud/speech
npm i node-record-lpcm16