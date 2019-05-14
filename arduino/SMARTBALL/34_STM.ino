//-----------------------------------------------------------------------------------
// STM MANAGEMENT
//-----------------------------------------------------------------------------------

#define STM_HEADER 4

//-----------------------------------------------------------------------------------
// BASIC FUNCTIONS
//-----------------------------------------------------------------------------------

void setSTM(uint16_t len) {
  uint16_t channels = 0;
  uint16_t streamFlag   = _DIN[0] << 8 | _DIN[1];
  uint16_t streamOffset = _DIN[2] << 8 | _DIN[3];

  if (streamFlag) {
    byte colorOffset, mstOffset, stbOffset, irlOffset, motOffset;

    byte color1 = (streamFlag >> COL1_STREAM_ADDR) & COL_STREAM_MASK;
    if (color1 == 7 || color1 == 5 || color1 == 4) return;
    else channels += 3 * color1;

    byte color2 = (streamFlag >> COL2_STREAM_ADDR) & COL_STREAM_MASK;
    if (color2 == 7 || color2 == 5 || color2 == 4) return;
    else {
      colorOffset = channels;
      channels += 3 * color2;
    }

    bool mst = (streamFlag >> MST_STREAM_BIT) & 1; mstOffset = channels; channels += 2 * mst;
    bool stb = (streamFlag >> STB_STREAM_BIT) & 1; stbOffset = channels; channels += 2 * stb;
    bool irl = (streamFlag >> IRL_STREAM_BIT) & 1; irlOffset = channels; channels += 2 * irl;
    bool mot = (streamFlag >> MOT_STREAM_BIT) & 1; motOffset = channels; channels += 2 * mot;

    if ((len - STM_HEADER) % channels == 0) {
      uint16_t ballNumber = (len - STM_HEADER) / channels;
      uint16_t idOffset = channels * ((gset.idNumber + ballNumber - (streamOffset % ballNumber)) % ballNumber);

      bool loopable = (streamFlag >> LOOP_STREAM_BIT) & 1;
      bool inOffset = (gset.idNumber >= streamOffset) && (gset.idNumber < (ballNumber + streamOffset));

      if (loopable || inOffset) {
        if (color1) setRGB(0, color1, STM_HEADER + idOffset);
        if (color2) setRGB(1, color2, STM_HEADER + idOffset + colorOffset);
        if (mst) setMST(STM_HEADER + idOffset + mstOffset);
        if (stb) setSTB(STM_HEADER + idOffset + stbOffset);
        if (irl) setIRL(STM_HEADER + idOffset + irlOffset);
        if (mot) setMOT(STM_HEADER + idOffset + motOffset);
      }
    }
  }
}
