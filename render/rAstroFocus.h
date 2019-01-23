
void drawAstroFocus() {
  	uint8_t h,posY,posX;
  	u8g_uint_t d;         
  	u8g.setFontRefHeightText();

	if (STATE==SCN07S01 && menuItem==9)
		renderTitle ("AstroFocus", 1, 1);
	else
		renderTitle ("AstroFocus", 1, 0);
		
	u8g.setFont(fontH2); 
	h = u8g.getFontAscent()-u8g.getFontDescent();
		
	// Literal Punto de entrada / Comienzo
	posY=26;
	posX=2;
	d = u8g.getStrWidth(menuAstroFocus.str[config.language][0]);
	u8g.drawStr(posX, posY, menuAstroFocus.str[config.language][0]); 

		
	// Literal Punto de salida / Final
	posY=39;
	d = u8g.getStrWidth(menuAstroFocus.str[config.language][1]);
  	u8g.drawStr(posX, posY, menuAstroFocus.str[config.language][1]);
	// Literal Duracion
	posY=51;
	d = u8g.getStrWidth(menuAstroFocus.str[config.language][2]);
  	u8g.drawStr(posX, posY, menuAstroFocus.str[config.language][2]); 

  	// Empezar            
	posY=63;
	// Si ya se han definido los puntos i/o y hay que ir al inicio
	if (inPointIsSet && outPointIsSet && currentPosition != IN) // Ir al inicio
		snprintf (buffer, sizeof(buffer), "%s", menuAstroFocus.str[config.language][4]);
	else // Empezar
		snprintf (buffer, sizeof(buffer), "%s", menuAstroFocus.str[config.language][3]);	

	d = u8g.getStrWidth(buffer);  
	u8g.setDefaultForegroundColor();
	if (STATE == SCN07S01) {
 		if (menuItem==8) {
			u8g.drawBox (64-(d/2)-1, posY-h+1, d+1, h);
			u8g.setDefaultBackgroundColor();
		}                   
	}
	u8g.drawStr(64-(d/2), posY, buffer);
	u8g.setDefaultForegroundColor();

	
	// Option button punto entrada
	posX = 48;
	posY=26;

	u8g.drawCircle (posX,posY-5,3);
	if (inPointIsSet) u8g.drawDisc (posX,posY-5,1);

	// Option button punto salida
	posY=39;
	u8g.drawCircle (posX,posY-5,3);
	if (outPointIsSet) u8g.drawDisc (posX,posY-5,1);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// In / Out points

	posY=26;
	posX =60;
	u8g.drawStr(posX, posY, "set"); 
	if ((STATE==SCN07S01) && (menuItem == 0)) 
		renderSelectCursor (posX-1,posY+1,h,u8g.getStrWidth("set")+1);
	// Out Point
	posY = 39;
	//posX = 90;
	u8g.drawStr(posX, posY, "set");	
	if ((STATE==SCN07S01) && (menuItem == 1)) 
		renderSelectCursor (posX-1,posY+1,h,u8g.getStrWidth("set")+1);
	

	if (STATE == SCN07S02) { // In Point
		posY=26;
		//posX =90;
		
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, "set");	

		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, 19, h);
		u8g.setDefaultBackgroundColor();
		if (menuItem == -3) u8g.drawStr(posX, posY, "<<<"); 
		if (menuItem == -2) u8g.drawStr(posX, posY, "-<<");
		if (menuItem == -1) u8g.drawStr(posX, posY, "--<");
		if (menuItem == 0) u8g.drawStr(posX, posY, "---");
		if (menuItem == 1) u8g.drawStr(posX, posY, ">--");
		if (menuItem == 2) u8g.drawStr(posX, posY, ">>-");
		if (menuItem == 3) u8g.drawStr(posX, posY, ">>>");
		u8g.setDefaultForegroundColor();
		//break;
	}

	if (STATE == SCN07S03) { // Out Point
		// Out Point
		posY=39;
		//posX =90;

		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, "set"); 

		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, 19, h);
		u8g.setDefaultBackgroundColor();
		if (menuItem == -3) u8g.drawStr(posX, posY, "<<<"); 
		if (menuItem == -2) u8g.drawStr(posX, posY, "-<<");
		if (menuItem == -1) u8g.drawStr(posX, posY, "--<");
		if (menuItem == 0) u8g.drawStr(posX, posY, "---");
		if (menuItem == 1) u8g.drawStr(posX, posY, ">--");
		if (menuItem == 2) u8g.drawStr(posX, posY, ">>-");
		if (menuItem == 3) u8g.drawStr(posX, posY, ">>>");
		u8g.setDefaultForegroundColor();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Round Trip
	posX=100;
	posY=33;
	if (astroRoundTrip==0)
		//u8g.drawStr(posX, posY, ">>");
		snprintf (buffer,sizeof(buffer), "%s", ">>");
	else
		//u8g.drawStr(posX, posY, "><");
		snprintf (buffer,sizeof(buffer), "%s", "><");

	u8g.drawStr (posX,posY, buffer);
	
	if (STATE == SCN07S01 && menuItem==2) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	//Duracion
	posY=51;
	posX=70;
	unsigned char espacio=8;

	// Horas
	snprintf (buffer,sizeof(buffer), "%d", selectedDurationHours);
	d = u8g.getStrWidth(buffer);  
	u8g.setDefaultForegroundColor();
	u8g.drawStr(posX, posY, buffer); 
	if ((STATE == SCN07S01) && (menuItem==3)) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}                   
	if (STATE==SCN07S08) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	
	// :
	posX=posX+espacio;
	snprintf (buffer,sizeof(buffer), ":");
	u8g.drawStr(posX, posY, buffer); 
	
	// Decenas de minuto
	posX=posX+espacio;
	snprintf (buffer,sizeof(buffer), "%d", selectedDurationMinutesTens);
	u8g.drawStr(posX, posY, buffer); 
		if ((STATE == SCN07S01) && (menuItem==4)) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}   
	if (STATE==SCN07S07) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
                
	// Unidades de minuto
	posX=posX+espacio;
	snprintf (buffer,sizeof(buffer), "%d", selectedDurationMinutesUnits);
	u8g.drawStr(posX, posY, buffer); 
	if ((STATE == SCN07S01) && (menuItem==5)) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}                   
	if (STATE==SCN07S06) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
    
    // : 
	posX=posX+espacio;
	snprintf (buffer,sizeof(buffer), ":");
	u8g.drawStr(posX, posY, buffer); 
	
	// Decenas de segundo
	posX=posX+espacio;
	snprintf (buffer,sizeof(buffer), "%d", selectedDurationSecondsTens);
	u8g.drawStr(posX, posY, buffer); 
	if ((STATE == SCN07S01) && (menuItem==6)) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}                   
	if (STATE==SCN07S05) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	
	// Unidades de segundo
	posX=posX+espacio;
	snprintf (buffer,sizeof(buffer), "%d", selectedDurationSecondsUnits);
	u8g.drawStr(posX, posY, buffer); 
	if ((STATE == SCN07S01) && (menuItem==7)) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}                   
	if (STATE==SCN07S04) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

} 
