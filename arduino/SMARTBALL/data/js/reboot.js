function reboot() {
	document.addEventListener("DOMContentLoaded", function(event) { 
		var xhttp = new XMLHttpRequest();
		xhttp.open("GET", "./ballReboot", true);
		xhttp.send();
	});
}

reboot();