var ffmpeg = require('fluent-ffmpeg'),
	fs = require('fs');

ffmpeg.setFfmpegPath(process.env.FFMPEG_PATH);
ffmpeg.setFfprobePath(process.env.FFPROBE_PATH);

module.exports = {
	TEST_VID: "9-28-2015-10-25-24.382515_oc.mov",
	MOVIE_PATH: ".",
	movToMp4: function(movFile){
		var fileName = movFile.substr(0, movFile.lastIndexOf('.'));
		var mp4File = fileName + '.mp4';

		ffmpeg(movFile)
		.inputFormat('mov')
		.noAudio()
		.videoCodec('libx264')
		.fps('29.97')
		.size('640x480')
		.autopad()
		.format('mp4')
		.on('end', function(){
			console.log(fileName + '.mp4 saved');
		}).
		on('error', function(err){
			console.log(err);
		})
		.save(mp4File);
		
	}
};

ffmpeg.getAvailableCodecs(function(err, codecs) {
  console.log('Available codecs:');
  console.dir(codecs);
});