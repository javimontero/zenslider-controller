
void drawVideoPro() {
  	uint8_t h,posY,posX;
  	u8g_uint_t d;         
  	u8g.setFontRefHeightText();

	if (STATE==SCN06S01 && menuItem==menuVideoPro.numElements)
		renderTitle ("Video Pro", 1, 1);
	else
		renderTitle ("Video Pro", 1, 0);
		
	u8g.setFont(fontH2); 
	h = u8g.getFontAscent()-u8g.getFontDescent();
		
	// Literal Punto de entrada / Comienzo
	posY=26;
	posX=2;
	d = u8g.getStrWidth(menuVideoPro.str[config.language][0]);
	u8g.drawStr(posX, posY, menuVideoPro.str[config.language][0]); 

		
	// Literal Punto de salida / Final
	posY=39;
	d = u8g.getStrWidth(menuVideoPro.str[config.language][1]);
  	u8g.drawStr(posX, posY, menuVideoPro.str[config.language][1]);
	// Literal Duracion
	posY=51;
	d = u8g.getStrWidth(menuVideoPro.str[config.language][2]);
  	u8g.drawStr(posX, posY, menuVideoPro.str[config.language][2]); 

  	// Empezar            
	posY=63;
	// Si ya se han definido los puntos i/o y hay que ir al inicio
	if (inPointIsSet && outPointIsSet && currentPosition != IN) // Ir al inicio
		snprintf (buffer, sizeof(buffer), "%s", menuVideoPro.str[config.language][4]);
	else // Empezar
		snprintf (buffer, sizeof(buffer), "%s", menuVideoPro.str[config.language][3]);	

	d = u8g.getStrWidth(buffer);  
	u8g.setDefaultForegroundColor();
	if (STATE == SCN06S01) {
 		if (menuItem==3) {
			u8g.drawBox (32-(d/2)-1, posY-h+1, d+1, h);
			u8g.setDefaultBackgroundColor();
		}                   
	}
	u8g.drawStr(32-(d/2), posY, buffer);
	u8g.setDefaultForegroundColor();


	// Ping Pong
	posY=63;
	snprintf (buffer, sizeof(buffer), "%s", menuVideoPro.str[config.language][5]);
	d = u8g.getStrWidth(buffer);  
	u8g.setDefaultForegroundColor();
	if (STATE == SCN06S01) {
 		if (menuItem==4) {
			u8g.drawBox (94-(d/2)-1, posY-h+1, d+1, h);
			u8g.setDefaultBackgroundColor();
		}                   
	}
	u8g.drawStr(94-(d/2), posY, buffer);
	u8g.setDefaultForegroundColor();
	
	// Option button punto entrada
	posY=26;
	u8g.drawCircle (65,posY-5,3);
	if (inPointIsSet) u8g.drawDisc (65,posY-5,1);

	// Option button punto salida
	posY=39;
	u8g.drawCircle (65,posY-5,3);
	if (outPointIsSet) u8g.drawDisc (65,posY-5,1);

	switch (STATE) {
		case SCN06S01:
		// In point
		posY=26;
		posX =90;
		u8g.drawStr(posX, posY, "set"); 
		if (menuItem == 0) renderSelectCursor (posX-1,posY+1,h,u8g.getStrWidth("set")+1);
		// Out Point
		posY = 39;
		posX = 90;
		u8g.drawStr(posX, posY, "set");	
		if (menuItem == 1) renderSelectCursor (posX-1,posY+1,h,u8g.getStrWidth("set")+1);
		break;
		
		case SCN06S04:
		// In point
		posY=26;
		posX =90;
		u8g.drawStr(posX, posY, "set"); 
		//if (menuItem == 0) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		// Out Point
		posY = 39;
		posX = 90;
		u8g.drawStr(posX, posY, "set");	
		//if (menuItem == 1) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		break;
		
		case SCN06S05:
		// In point
		posY=26;
		posX =90;
		u8g.drawStr(posX, posY, "set"); 
		//if (menuItem == 0) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		// Out Point
		posY = 39;
		posX = 90;
		u8g.drawStr(posX, posY, "set");	
		//if (menuItem == 1) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		break;
		
		case RVMP:
		// In point
		posY=26;
		posX =90;
		u8g.drawStr(posX, posY, "set"); 
		//if (menuItem == 0) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		// Out Point
		posY = 39;
		posX = 90;
		u8g.drawStr(posX, posY, "set");	
		//if (menuItem == 1) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		break;

		case SCN06S06:
		// In point
		posY=26;
		posX =90;
		u8g.drawStr(posX, posY, "set"); 
		//if (menuItem == 0) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		// Out Point
		posY = 39;
		posX = 90;
		u8g.drawStr(posX, posY, "set");	
		//if (menuItem == 1) renderSelectCursor (posX,posY,h,u8g.getStrWidth("set"));
		break;

		case SCN06S02:
		// Out Point
		posY=39;
		posX =90;
		u8g.drawStr(posX, posY, "set");	
		// In point
		posY=26;
		posX =90;
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
		break;

		case SCN06S03:
		// In point
		posY=26;
		posX =90;
		u8g.drawStr(posX, posY, "set"); 
		// Out Point
		posY=39;
		posX =90;
		if (menuItem == -3) u8g.drawStr(posX, posY, "<<<"); 
		if (menuItem == -2) u8g.drawStr(posX, posY, "-<<");
		if (menuItem == -1) u8g.drawStr(posX, posY, "--<");
		if (menuItem == 0) u8g.drawStr(posX, posY, "---");
		if (menuItem == 1) u8g.drawStr(posX, posY, ">--");
		if (menuItem == 2) u8g.drawStr(posX, posY, ">>-");
		if (menuItem == 3) u8g.drawStr(posX, posY, ">>>");
		break;
	}

	// Duracion
	posY=51;
	posX=90;

	//snprintf(buffer,sizeof(buffer),"%s: %u",strShots[config.language], currentNumShots);    
	snprintf (buffer,sizeof(buffer), "%d s", selectedDuration);
	d=u8g.getStrWidth (buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (menuItem == 2 && STATE == SCN06S01) renderSelectCursor (posX-2,posY+1,h,d+2);
	if (STATE==SCN06S04) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer);
	}
	
} 
