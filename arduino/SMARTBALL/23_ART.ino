//-----------------------------------------------------------------------------------
// ARTNET PROTOCOL (EXPERIMENTAL)
//-----------------------------------------------------------------------------------

ArtnetWiFiReceiver artnet;

boolean connectART() {
  //Serial.println("- begin artnet");
  artnet.begin();
  artnet.subscribe(aset.universe, artnetCallback);
  return true;
}

void receiveART() {
  artnet.parse();
}
