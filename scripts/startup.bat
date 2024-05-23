::netsh advfirewall set allprofiles state off

taskkill /f /im explorer.exe /fi "memusage gt 2"
::taskkill /f /im node.exe

timeout /t 3 

start cmd.exe /K "cd c:\ancient_futures\AncientFutures\stand_alone_version && node index.js"

start cmd.exe /K "cd c:\ancient_futures\of_v0.11.2_vs2017_release\apps\myApps\AncientFuturesOF\bin && AncientFuturesOF.exe"

::npm run ampm