var ffmpeg = require('fluent-ffmpeg'),
	fs = require('fs');

ffmpeg.setFfmpegPath(process.env.FFMPEG_PATH);
ffmpeg.setFfprobePath(process.env.FFPROBE_PATH);

module.exports = {
	TEST_VID: "test_video.mov",
	MOVIE_PATH: ".",
	movToMp4: function(movFile){
		var fileName = movFile.substr(0, movFile.lastIndexOf('.'));

		//resize the orginal video and output as an mp4 at the same time
		ffmpeg(movFile)
		// .inputFormat('mov')
		.outputOptions('-pix_fmt yuv420p')
		.noAudio()
		// .videoCodec('libx264')
		.fps('29.97')
		.size('640x480')
		.autopad()
		.output(fileName + '_converted.mov')
		.on('end', function(){
			console.log('done saving');
		}).
		on('error', function(err){
			console.log(err);
		})
		.run();
	}
};