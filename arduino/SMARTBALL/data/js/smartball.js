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

scanNetwork();