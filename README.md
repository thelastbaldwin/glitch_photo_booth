# Glitch Photo Booth #

An OpenFrameworks application that runs concurrently with a web server. 
The application records a short video, the server uploads the video to a specified endpoint
then updates the application upon success.

Movies are saved in mp4 format without audio.

### Dependencies: ###

[node](http://nodejs.org)

[Node fluent-mmpeg](https://github.com/fluent-ffmpeg/node-fluent-ffmpeg)

[OpenFrameworks](http://openframeworks.cc/)

ofxOFC (standard add-on)

[ffmpeg](https://www.ffmpeg.org/)
`FFMPEG_PATH` and `FFPROBE_PATH` environment variables must be set if ffmpeg is not in your PATH
