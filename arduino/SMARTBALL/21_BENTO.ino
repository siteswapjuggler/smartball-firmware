//-----------------------------------------------------------------------------------
// UDP SOCKET
//-----------------------------------------------------------------------------------

WiFiUDP BenTo;                          // BenTo UDP socket
#define BENTO_IN 8888                   // BenTo UDP in port

boolean connectBenTo() {
  return BenTo.begin(BENTO_IN) == 1;
}

//-----------------------------------------------------------------------------------
// PACKET MANAGEMENT
//-----------------------------------------------------------------------------------

void receiveBenTo() {
  uint16_t packetSize = BenTo.parsePacket();
  if (packetSize) {
    BenTo.read(_DIN, packetSize);
    setRGB(0, (packetSize - 1) / 3, 0);
  }
}
