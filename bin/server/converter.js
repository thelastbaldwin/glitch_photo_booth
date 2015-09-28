var ffmpeg = require('fluent-ffmpeg');

module.exports = {
	MOVIE_PATH: ".",
	setMoviePath: function(path){
		this.MOVIE_PATH = path;
	},
	convert: function(movFile, callback){
		var fileName = movFile.substr(0, movFile.lastIndexOf('.'));

		//resize the orginal video and output as an mp4 at the same time
		ffmpeg(movFile)
		.outputOptions('-pix_fmt yuv420p')
		.noAudio()
		.fps('29.97')

		.output(this.MOVIE_PATH + '/' + fileName + '_converted.mov')
		.size('640x480')
		.autopad()

		.output(this.MOVIE_PATH + '/' + fileName + '_converted.mp4')
		.size('640x480')
		.autopad()

		.on('end', function(){
			if(typeof callback === 'function'){
				callback();
			}
		})
		.run();
	}
};