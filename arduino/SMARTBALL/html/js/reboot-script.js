function rebootBall() {
	document.addEventListener("DOMContentLoaded", function(event) { 
			var xhttp = new XMLHttpRequest();
			xhttp.open("GET", "./rebootBall", true);
			xhttp.send();
		});
}

rebootBall();