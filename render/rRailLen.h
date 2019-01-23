void drawRailLen(void) {

	uint8_t h,posY,posX;
  	u8g_uint_t d;         
  	u8g.setFontRefHeightText();

	if (STATE == SCN11S01 && menuItem == 4) 
		renderTitle (strRailLenght[config.language], 1, 1);
	else
		renderTitle (strRailLenght[config.language], 1, 0);
		
	// Literal fijo longitud rail
	u8g.setFont(fontH2); 
	h = u8g.getFontAscent()-u8g.getFontDescent();

	posY = 40;
	posX = 75;
	snprintf (buffer, sizeof(buffer), "cm");
	d = u8g.getStrWidth(buffer); 
	u8g.drawStr(posX, posY, buffer);

	// OK, salvar
	posY = 63;
	snprintf (buffer, sizeof(buffer), "OK");
	posX = 64-(d/2);
	d = u8g.getStrWidth(buffer); 
	if (STATE == SCN11S01 && menuItem == 3) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox (64-(d/2)-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
	}
	u8g.drawStr(posX, posY, buffer);	
 		

	//////////////////// Longitud rail

	posY = 40;
	posX = 45;
	unsigned char espacio=8;
	
	// Centenas
	snprintf (buffer, sizeof (buffer),"%d",selectedRailLenghtHundreds);
	d = u8g.getStrWidth(buffer);
	u8g.setDefaultForegroundColor();
	u8g.drawStr (posX, posY, buffer);
	if (STATE == SCN11S01 && menuItem == 0 ){
 		renderSelectCursor (posX-2,posY+1,h,d+2);
 		//u8g.setDefaultBackgroundColor();
 	}
	
	if (STATE==SCN11S02) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

	// Decenas
	posX += espacio;
	snprintf (buffer, sizeof (buffer),"%d",selectedRailLenghtTenths);
	d = u8g.getStrWidth(buffer);
	//u8g.setDefaultForegroundColor();
	u8g.drawStr (posX, posY, buffer);
	if (STATE == SCN11S01 && menuItem == 1 ){
 		renderSelectCursor (posX-2,posY+1,h,d+2);
 		//u8g.setDefaultBackgroundColor();
 	}
	
	if (STATE==SCN11S03) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	
	// Unidades
	posX += espacio;
	snprintf (buffer, sizeof (buffer),"%d",selectedRailLenghtUnits);
	d = u8g.getStrWidth(buffer);
	//u8g.setDefaultForegroundColor();
	u8g.drawStr (posX, posY, buffer);
	if (STATE == SCN11S01 && menuItem == 2 ){
 		renderSelectCursor (posX-2,posY+1,h,d+2);
 		//u8g.setDefaultBackgroundColor();
 	}
	
	if (STATE==SCN11S04) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

}