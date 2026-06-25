#ifndef utf8RusGFX_h
#define utf8RusGFX_h

char* DspCore::utf8Rus(const char* str, bool uppercase) {
  int index = 0;
  static char strn[BUFLEN];
  strlcpy(strn, str, BUFLEN);  
  
  	//if (uppercase) {	// prepínať veľké a malé písmená	
	//for (char *iter = strn; *iter != '\0'; ++iter)
	//*iter = toupper(*iter);  
	//}	

//if(L10N_LANGUAGE==EN)  return strn;
  while (strn[index])
  {
    if (strn[index] >= 0xBF)
    {
      switch (strn[index]) {
	  
        case 0xC3: {
            if (strn[index + 1] == 0xA1) {strn[index] = 0x61;break;} // á > a
            if (strn[index + 1] == 0xA4) {strn[index] = 0x61;break;} // ä > a
            if (strn[index + 1] == 0x81) {strn[index] = 0x41;break;} // Á > A
            if (strn[index + 1] == 0x84) {strn[index] = 0x41;break;} // Ä > A
            if (strn[index + 1] == 0x89) {strn[index] = 0x45;break;} // É > E
            if (strn[index + 1] == 0xA9) {strn[index] = 0x65;break;} // é > e
            if (strn[index + 1] == 0x8D) {strn[index] = 0x49;break;} // Í > I
            if (strn[index + 1] == 0xAD) {strn[index] = 0x69;break;} // í > i
            if (strn[index + 1] == 0x9D) {strn[index] = 0x59;break;} // Ý > Y
            if (strn[index + 1] == 0xBD) {strn[index] = 0x79;break;} // ý > y
            if (strn[index + 1] == 0x93) {strn[index] = 0x4F;break;} // Ó > O
            if (strn[index + 1] == 0x94) {strn[index] = 0x4F;break;} // Ô > O
            if (strn[index + 1] == 0x96) {strn[index] = 0x4F;break;} // Ö > O
            if (strn[index + 1] == 0xB3) {strn[index] = 0x6F;break;} // ó > o
            if (strn[index + 1] == 0xB4) {strn[index] = 0x6F;break;} // ô > o
            if (strn[index + 1] == 0xB6) {strn[index] = 0x6F;break;} // ö > o
            if (strn[index + 1] == 0x9A) {strn[index] = 0x55;break;} // Ú > U
            if (strn[index + 1] == 0x9C) {strn[index] = 0x55;break;} // Ü > U
            if (strn[index + 1] == 0xBA) {strn[index] = 0x75;break;} // ú > u
            if (strn[index + 1] == 0xBC) {strn[index] = 0x75;break;} // ü > u
            if (strn[index + 1] >= 0x90 && strn[index + 1] <= 0xBF) strn[index] = strn[index + 1] + 0x30;break;}
        
        case 0xC4: {
            if (strn[index + 1] == 0x9A) {strn[index] = 0x45;break;} // Ě > E
            if (strn[index + 1] == 0x9B) {strn[index] = 0x65;break;} // ě > e
            if (strn[index + 1] == 0x8C) {strn[index] = 0x43;break;} // Č > C
            if (strn[index + 1] == 0x8D) {strn[index] = 0x63;break;} // č > c
            if (strn[index + 1] == 0x8E) {strn[index] = 0x44;break;} // Ď > D
            if (strn[index + 1] == 0x8F) {strn[index] = 0x64;break;} // ď > d
            if (strn[index + 1] == 0xB9) {strn[index] = 0x4C;break;} // Ĺ > L
            if (strn[index + 1] == 0xBD) {strn[index] = 0x4C;break;} // Ľ > L
            if (strn[index + 1] == 0xBA) {strn[index] = 0x6C;break;} // ĺ > l
            if (strn[index + 1] == 0xBE) {strn[index] = 0x6C;break;} // ľ > l
            if (strn[index + 1] >= 0x90 && strn[index + 1] <= 0xBF) strn[index] = strn[index + 1] + 0x30;break;}

        case 0xC5: {
            if (strn[index + 1] == 0x87) {strn[index] = 0x4E;break;} // Ń > N
            if (strn[index + 1] == 0x88) {strn[index] = 0x6E;break;} // ň > n
            if (strn[index + 1] == 0x98) {strn[index] = 0x52;break;} // Ř > R
            if (strn[index + 1] == 0x99) {strn[index] = 0x72;break;} // ř > r
            if (strn[index + 1] == 0xA0) {strn[index] = 0x53;break;} // Š > S
            if (strn[index + 1] == 0xA1) {strn[index] = 0x73;break;} // š > s
            if (strn[index + 1] == 0xA4) {strn[index] = 0x54;break;} // Ť > T
            if (strn[index + 1] == 0xA5) {strn[index] = 0x74;break;} // ť > t
            if (strn[index + 1] == 0xAE) {strn[index] = 0x55;break;} // Ů > U
            if (strn[index + 1] == 0xAF) {strn[index] = 0x75;break;} // ů > u
            if (strn[index + 1] == 0xBD) {strn[index] = 0x5A;break;} // Ž > Z
            if (strn[index + 1] == 0xBE) {strn[index] = 0x7A;break;} // ž > z
            if (strn[index + 1] == 0x95) {strn[index] = 0x72;break;} // ŕ > r
            if (strn[index + 1] >= 0x80 && strn[index + 1] <= 0x8F) strn[index] = strn[index + 1] + 0x70;break;}
      }  
		 
       int sind = index + 2;
      while (strn[sind]) {
        strn[sind - 1] = strn[sind];
        sind++;
      }
      strn[sind - 1] = 0;
    }
	
	if (strn[index] == 0xC2)
    {
      switch (strn[index + 1]) {
        case 0xB4: {
			strn[index] = 0x60; // '
              break;
            }
		  }  
		 
       int sind = index + 2;
      while (strn[sind]) {
        strn[sind - 1] = strn[sind];
        sind++;
      }
      strn[sind - 1] = 0;
    }
		
    index++;
  }
 return strn;
}
#endif

