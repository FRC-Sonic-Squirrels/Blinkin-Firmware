void EndtoEndBlend_1() {
  uint8_t speed = beatsin8(6,0,255);
  endclr = blend(BLACK, colorList[COLOR1], speed);
  midclr = blend(colorList[COLOR1], BLACK, speed);

  fill_gradient_RGB(leds, 0, endclr, stripLength/2, midclr);
  fill_gradient_RGB(leds, stripLength/2+1, midclr, stripLength-1, endclr);
}

void EndtoEndStaticBlend_1() {
  fill_gradient_RGB(leds, 0, BLACK, stripLength/2, colorList[COLOR1]);
  fill_gradient_RGB(leds, stripLength/2+1, colorList[COLOR1], stripLength-1, BLACK);

}
