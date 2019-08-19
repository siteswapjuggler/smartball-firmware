function scanNetwork() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
   if (this.readyState == 4 && this.status == 200) {
	  document.getElementById("ssid").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "./scanNetwork", true);
  xhttp.send();
}

function getID() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
   if (this.readyState == 4 && this.status == 200) {
	  document.getElementById("ballSN").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "./getSN", true);
  xhttp.send();
}

function getSSID() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
   if (this.readyState == 4 && this.status == 200) {
	  document.getElementById("ballSSID").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "./getSSID", true);
  xhttp.send();
}

getID();
getSSID();
scanNetwork();