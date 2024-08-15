void DISP_PictWrite(){
  M5.Display.startWrite();
  M5.Display.pushImage(0, 0, imgWidth, imgHeight, img);
  M5.Display.endWrite();
}
void DISP_PictWrite_MODE_B(){
  M5.Display.startWrite();
  M5.Display.pushImage(0, 0, imgWidth, imgHeight, img2);
  M5.Display.endWrite();
}

void DISP_PictWrite_MODE_C(){
  M5.Display.startWrite();
  M5.Display.pushImage(0, 0, imgWidth, imgHeight, img3);
  M5.Display.endWrite();
}