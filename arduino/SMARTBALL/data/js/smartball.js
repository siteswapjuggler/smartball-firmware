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

function drawConfig() {
	$( "#navbarNav li" ).removeClass( "active" )
	$( "#config").addClass("active");
	$( "main" ).load( "inc-config.html" );
}

function drawFiles(element) {
	$( "#navbarNav li" ).removeClass( "active" )
	$( "#files").addClass("active");
	$( "main" ).load( "inc-file.html" );
}

function drawDocs() {
	$( "#navbarNav li" ).removeClass( "active" )
	$( "#docs").addClass("active");
	$( "main" ).load( "inc-doc.html" );
}

function drawAbout() {
	$( "#navbarNav li" ).removeClass( "active" )
	$( "#about").addClass("active");
	$( "main" ).load( "inc-about.html" );
}


$(document).ready(function(){
	drawConfig();
	scanNetwork();    
});