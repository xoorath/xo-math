@ECHO OFF
REM TODO: check that nodejs is installed, and offer a simple install process if it isn't.
REM No, we aren't going to just do everything in batch like a god damn caveman.
REM I don't care if it's javascript, but we will script in a real language that runs on Win/Linux/MacOS

SET OD=%CD%
cd %~dp0
cd scripts
call node.exe distribute.js
CD %OD%