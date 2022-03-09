@echo off

set GOOGLE_APPLICATION_CREDENTIALS=%1

cd C:\Users\nicol\Documents\creative\projects\ancient_futures\code\nodejs-speech

node samples\recognize.js sync %2

:done
echo.
