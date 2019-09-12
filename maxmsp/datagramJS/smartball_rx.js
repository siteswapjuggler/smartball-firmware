autowatch=1;
inlets=1;
outlets=1;

function parse(id,sn,cmd,data) {
	function xyz(addr,div) {
		var coordinates = new Array();
		for (var i=0;i<3;i++) {
			coordinates[i] = int16_complement(data[addr],data[addr+1])/div;
			addr += 2;
		}
		return coordinates;
	}
	
	function int16_complement(msb,lsb) {
		var v = (msb << 8) | lsb;	     
		if ((v >> 15) & 1) v = (v & 0x7FFF) - 0x8000;
		return v;
	}
		
	function int8_complement(v) {
		if ((v >> 7) & 1) v = (v & 0x7F) - 0x80;
		return v;
	}

	function imu() {
		var addr = 1;
		if (data[0] & 1)  {
			outlet(0,id,'imu','acc',xyz(addr,100.));
			addr += 6;
		}
		if (data[0] & 2) {
			outlet(0,id,'imu','gyr',xyz(addr,100.));
			addr += 6;
		}
		if (data[0] & 4) {
			outlet(0,id,'imu','mag',xyz(addr,100.));
			addr += 6;
		}
		if (data[0] & 8) {
			outlet(0,id,'imu','tmp',int16_complement(data[addr],data[addr+1])/100.);
			addr += 2;
		}
		if (data[0] & 16)  {
			outlet(0,id,'imu','vec',xyz(addr,100.));
			addr += 6;
		}
		if (data[0] & 32) {
			outlet(0,id,'imu','quat',xyz(addr,10000.));
			addr += 6;
		}
		if (data[0] & 64) {
			outlet(0,id,'imu','wld',xyz(addr,100.));
			addr += 6;
		}
		if (data[0] & 128) {
			outlet(0,id,'imu','sta',int16_complement(data[addr],data[addr+1]));
			addr += 2;
		}
	}

	
	if (cmd == 0x00) {
		outlet(0,id,'bat',int16_complement(data[0],data[1])/100.);
	}
	else if (cmd == 0x01) {
		outlet(0,id,'pong');
	}
	else if (cmd == 0x30) {
		imu();
	}
}
parse.local=1;
parse.immediate=1;

function list() {
	var data = arrayfromargs(arguments), 
		l = data.length, 
		id  = data[0] << 8 | data[1],
		sn  = data[2] << 8 | data[3],
		cmd = data[4];
	parse(id,sn,cmd,data.slice(5,l));
}
list.immediate=1;