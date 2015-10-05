var express = require('express'),
	app = express(),
	http = require('http').Server(app),
	dgram = require('dgram'),
	osc = require('osc-min'),
	sendSocket = dgram.createSocket('udp4'),
	receiveSocket = dgram.createSocket('udp4'),
	SEND_PORT = 12345,
	RECEIVE_PORT = 12346,
	OUTPUT_DIR = "../data/";

sendSocket.bind(SEND_PORT);
receiveSocket.bind(RECEIVE_PORT);

function getOSCMessage(msg){
	//extract relevant data from OSC Message
	var oscMessage = osc.fromBuffer(msg);
	try{
		console.log(oscMessage);
		// //translate osc buffer into javascript object
		var element = oscMessage.elements[0],
			address = element.address,
			args = element.args,
			cleanMovie = args[0].value,
			distortedMovie = args[1].value;

		return {
			address: address,
			cleanMovie: cleanMovie,
			distortedMovie: distortedMovie
		};

	}catch(error){
		console.log("invalid OSC packet");
	}
}

receiveSocket.on('message', function(message, remote){
	var oscData = getOSCMessage(message);
	// looks like this 
	// {
	// 	address: '/video',
	// 	cleanMovie: 'filename.mp4',
	// 	distortedMovie: 'filename_distorted.mp4'
	// }
	// console.log(oscData);

	//find the files using OUTPUT_DIR and filenames
	//send a post request to S3 with resources
	//on complete, call sendOSCMessage with the code
});

function sendOSCMessage(code){
	var buffer = osc.toBuffer({
		address: '/uploaded',
		args : [{
			type: "string",
			value: code
		}]
	});
	sendSocket.send(buffer, 0, buffer.length, SEND_PORT, '127.0.0.1');
}

var sendInterval = setInterval(function(){
	sendOSCMessage('1337H4X04L0L0L0L0L');
}, 3000);

