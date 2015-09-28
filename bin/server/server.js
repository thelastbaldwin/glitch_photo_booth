var converter = require('./converter.js'),
	express = require('express'),
	app = express(),
	http = require('http').Server(app),
	dgram = require('dgram'),
	osc = require('osc-min'),
	// sendSocket = dgram.createSocket('udp4'),
	// receiveSocket = dgram.createSocket('udp4'),
	SEND_PORT = 12345,
	RECEIVE_PORT = 12346;

// sendSocket.bind(SEND_PORT);
// receiveSocket.bind(RECEIVE_PORT);

function getOSCMessage(msg){
	//extract relevant data from OSC Message
	var oscMessage = osc.fromBuffer(msg);

	try{
		console.log(oscMessage);
		// //translate osc buffer into javascript object
		// var element = oscMessage.elements[0],
		// address = element.address,
		// args = element.args; //contains 'type' and 'value

		// return {
		// 	address: address,
		// 	filename: args[0].value,
		// 	id : args[1].value
		// }

	}catch(error){
		console.log("invalid OSC packet");
	}
}

function sendOSCMessage(photoType, socketId){
	// var buffer = osc.toBuffer({
	// 	address: '/take/picture',
	// 	args : [{
	// 		type: "string",
	// 		value: photoType
	// 	},
	// 	{
	// 		type: "string",
	// 		value: socketId
	// 	}]
	// });

	// sendSocket.send(buffer, 0, buffer.length, SEND_PORT, 'localhost');
}

