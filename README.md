# Glitch Photo Booth #

An OpenFrameworks application that runs concurrently with a web server. 
The application records a short video, the server uploads the video to a specified endpoint
then updates the application upon success.

Movies are saved in mp4 format without audio.

[Fix for XCode 7 projects](http://forum.openframeworks.cc/t/fix-for-xcode-7-and-of-0-8-4-opensslconf-h-not-found/20800)

### Dependencies: ###

[node](http://nodejs.org)

[OpenFrameworks](http://openframeworks.cc/)

[ofxVideoRecorder](https://github.com/timscaffidi/ofxVideoRecorder)

ofxOFC (standard add-on)

[ffmpeg](https://www.ffmpeg.org/)

ffmpeg must be added to your path
