//-----------------------------------------------------------------------------------
// ARTNET PROTOCOL (EXPERIMENTAL)
//-----------------------------------------------------------------------------------

uint32_t ARTNET_UNIVERSE = 1;
ArtnetWiFiReceiver artnet;

boolean connectART() {
  //Serial.println("- begin artnet");
  artnet.begin();
  artnet.subscribe(ARTNET_UNIVERSE, artnetCallback);
  return true;
}

void receiveART() {
  artnet.parse();
}
