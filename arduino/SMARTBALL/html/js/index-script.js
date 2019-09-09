function togglePassword() {
	pwd = document.getElementById("pwd");
	view = document.getElementById("view");
	
	if (pwd.getAttribute("type") == "password") {
		pwd.setAttribute("type","text");
		view.classList.remove("icon-eye-closed");
		view.classList.add("icon-eye");
	}
	else {
		pwd.setAttribute("type","password");
		view.classList.remove("icon-eye");
		view.classList.add("icon-eye-closed");
	}
}

function testBall() {
	var xhttp = new XMLHttpRequest();
	xhttp.open("GET", "./testBall", true);
	xhttp.send();
}

function getParameters() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
   if (this.readyState == 4 && this.status == 200) {
			parameters = this.response.split(",");
			document.getElementById("NT").innerHTML = parameters[0];
			document.getElementById("ID").innerHTML = parameters[1];
			document.getElementById("SN").innerHTML = parameters[2];
			document.getElementById("FW").innerHTML = parameters[3];
			document.getElementById("EP").innerHTML = parameters[4];
			document.getElementById("PR").innerHTML = parameters[5];
    }
  };
  xhttp.open("GET", "./getParameters", true);
  xhttp.send();
}

function getNetworks(refresh) {
  var i, html, options, xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			if (this.response === "false") {
				html += "<option disabled selected>"+translation[lang]['wifi-form-network-nowifi']+"</option>";
			}
			else {
				html += "<option disabled selected>"+translation[lang]['wifi-form-network-selector']+"</option>";
				options = this.response.split(",");
				for (i=0; i < options.length; i++) {
					html += "<option>"+options[i]+"</option>";
				}
			}
			document.getElementById("ssid").innerHTML = html;
		}
  };
  xhttp.open("GET", refresh ? "./refreshNetworks" : "./getNetworks", true);
  xhttp.send();
}

getNetworks(false);
getParameters();