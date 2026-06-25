//==============================================================
void touch() {
  bool touched = tft.getTouch( &tX, &tY);     // Touch X,Y
  if (touched) { //Serial.printf("%d,%d\n", tX, tY);
    // Vol bar -------------------------------------------------
    if ( tX>150 && tX<312 && tY>182 && tY<198){ 
      vol = int((tX-150)/((312-150)/20));    // bar -> vol
      if (vol > 20) {vol = 20;}              // max 20
      bar(); audio.setVolume(vol); return;   // Volume and Button
    }
    // Vol- ----------------------------------------------------
    if(Tb(0)){vol=vol-1; if (vol<0){vol=0;} // Vol -
      Bn="vol-"; Btn(Bn,"on"); bar();       // Button, bar
      audio.setVolume(vol);delay(250);return;// set Volume
    }
    // <<ch ----------------------------------------------------
    if(Tb(1)){sn=sn-10; if (sn<0 ){sn=0;}   // station down -10
      Bn="<<"; Btn(Bn,"on");pl=false;return;// << Button
    }  
    // <ch -----------------------------------------------------
    if(Tb(2)){sn=sn-1; if (sn<0 ){sn=0;}    // station down -1
      Bn="<ch";Btn(Bn,"on");pl=false;return;// <ch Button
    }
    // ch> -----------------------------------------------------
    if(Tb(3)){sn=sn+1;  if (sn>un){sn=0;}   // station UP +1
      Bn="ch>";Btn(Bn,"on");pl=false;return;// ch> Button
    }
    // ch>> ----------------------------------------------------
    if(Tb(4)){sn=sn+10; if (sn>un){sn=0;}   // station UP +10
      Bn=">>"; Btn(Bn,"on");pl=false;return;// ch>> Button
    } 
    // Vol+ ----------------------------------------------------
    if(Tb(5)){vol=vol+1; if(vol>20){vol=20;}// Vol +
      Bn="vol+"; Btn(Bn,"on"); bar();       // Button, bar
      audio.setVolume(vol);delay(250);return;// set Volume
    }
    // BL- -----------------------------------------------------
    if ( tX>  0 && tX< 60 && tY>120 && tY<170){
      bl--; if (bl < 0){ bl = 0;}           // BackLight minus
      tft.setBrightness(backlight[bl]);delay(250);return; 
    }
    // BL+ -----------------------------------------------------
    if ( tX>259 && tX<319 && tY>120 && tY<170 ){
      bl++; if (bl > 4){ bl = 4;}           // BackLight plus
      tft.setBrightness(backlight[bl]);delay(250);return; 
    }
    // inv -----------------------------------------------------
    if ( tX>100 && tX<226 && tY>120 && tY<170 ){ inv=!inv;
      if ( inv){tft.invertDisplay(1);delay(250);return;}
      if (!inv){tft.invertDisplay(0);delay(250);return;} 
    }
    // bgcolor -------------------------------------------------
    if ( tX>0 && tX<480 && tY<30 ){
      bc = tX * 205; if( bc > 65535){ bc = 65535;}
      tft.fillScreen(bc);MP(25,1,"Web Radio of ESP32");
      Btn("all","off"); pl=false;return;
    }
    // bgcolor black -------------------------------------------
    if ( tX>0 && tX<480 && tY>30 && tY<60 ){ bc = 0; // black
      tft.fillScreen(bc);MP(25,1,"Web Radio of ESP32");
      tft.pushImage(0,0,18,20,(uint16_t *)apple20);//apple fan
      Btn("all","off"); pl=false;return;
    }
    
  } // End of Touched
}
//=======//==============================================================
bool Tb( int p ){                            // Touch button
  if ( tX > b[p][0] && tX < (b[p][0] + b[p][2]) && 
       tY > b[p][1] && tY < (b[p][1] + b[p][3]) ){ return true;}
  else{ return false;}
}
//==============================================================
