@echo off

set GOOGLE_APPLICATION_CREDENTIALS=%1

::cd C:\Users\nicol\Documents\creative\projects\ancient_futures\code\nodejs-speech
::cd C:\Users\nicol\Documents\creative\projects\ancient_futures\code\AncientFutures\nodejs-speech
cd C:\ancient_futures\AncientFutures\nodejs-speech

node recognize.js sync %2

:done
echo.
