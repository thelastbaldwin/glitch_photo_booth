var converter = require('./converter');

converter.setMoviePath('../data');

converter.convert('test_video.mov', function(){
	//remove the original file
	// fs.unlink(movFile);
	console.log('done saving');
});