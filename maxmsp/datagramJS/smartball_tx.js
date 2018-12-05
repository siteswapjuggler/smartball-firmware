autowatch=1;
inlets=1;
outlets=2;

function send(cmd,data) {
	var l = data.length, msg = new Array();
	for (var i = 0; i < l; i++) msg.push(data[i]);
	outlet(0,cmd,msg);
	}
send.local=1;
send.immediate=1;

//--------------------------
// SMARTBALL COMMANDS
//--------------------------

function anything() {
	var noargs = new Array();
	
	if (messagename == "/stm") {
		data = new Array();
		data.push((arguments[0]>>8)&255);
		data.push(arguments[0]&255);
		data.push((arguments[1]>>8)&255);
		data.push(arguments[1]&255);
		for (i=2;i<arguments.length;i++) {
			data.push(arguments[i] & 255);
		}
		send(0x21,data); 
	}
	else if (messagename == "/rgb") {
		for (i=0;i<arguments.length;i++) {
			arguments[i] &= 255;
		}
		send(0x20,arguments); 
	}
	else if (messagename == "/rgb-bg") {
		for (i=0;i<arguments.length;i++) {
			arguments[i] &= 255;
		}
		send(0x22,arguments); 
	}
	else if (messagename == "/ping") {
		send(0x01,noargs); 
	}
	else if (messagename == "/reboot") {
		send(0x02,noargs); 
	}
	else if (messagename == "/sleep") {
		send(0x03,noargs); 
	}
	else if (messagename == "/setFactory") {
		data = new Array();
		arguments[2] *= 10; 
		data.push(arguments[0]>>8);
		data.push(arguments[0]&255);
		data.push(arguments[1]>>8);
		data.push(arguments[1]&255);
		data.push(arguments[2]>>8);
		data.push(arguments[2]&255);
		send(0x10,data);
	}
	else if (messagename == "/saveFactory") {
		send(0x11,noargs); 
	}
	else if (messagename == "/setGeneral") {
		data = new Array();
		data.push(arguments[0]&255);
		send(0x12,data);
	}
	else if (messagename == "/saveGeneral") {
		send(0x13,noargs); 
	}
	else if (messagename == "/imu") {
		arguments[0] &= 255; 
		send(0x30,arguments);
	}
	else if (messagename == "/saveImu") {
		send(0x31,noargs);
	}
	else if (messagename == "/accRange") {
		arguments[0] &= 3; 
		send(0x32,arguments);
	}
	else if (messagename == "/gyrRange") {
		arguments[0] &= 3; 
		send(0x33,arguments);
	}
	else if (messagename == "/irl") {
		data = new Array();
		arguments[0] &= 1023; 
		data.push(arguments[0]>>8);
		data.push(arguments[0]&255);
		send(0x40,data); 
	}
	else if (messagename == "/mot") {
		data = new Array();
		arguments[0] &= 1023; 
		data.push(arguments[0]>>8);
		data.push(arguments[0]&255);
		send(0x50,data);  
	}
	else if (messagename == "/stb") {
		data = new Array();
		val  = arguments[0]*100; 
		data.push(val>>8);
		data.push(val&255);
		send(0x60,data); 
	}
	else if (messagename == "/master") {
		for (i=0;i<arguments.length;i++) {
			arguments[i] &= 255;
		}
		send(0x70,arguments); 
	}
}
anything.immediate = 1;