//-----------------------------------------------------------------------------------
// ARTNET PROTOCOL (EXPERIMENTAL)
//-----------------------------------------------------------------------------------

#define ARTNET_UNIVERSE 1
ArtnetReceiver artnet;

boolean connectART() {
  artnet.begin();
  artnet.subscribe(ARTNET_UNIVERSE, artnetCallback);
  return true;
}

void receiveART() {
  artnet.parse();
}
