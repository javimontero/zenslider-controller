void drawTimeLapsePro() {
  	uint8_t h,posY;
  	u8g_uint_t d;                   
	//Serial.print ("entrando drawTimeLapsePro "); Serial.print ("\n");                              
	// Título con botón Back. Solo se puede selecionar en estado SCN04S01
	if (STATE==SCN04S01 && menuItem==menuTimeLapsePro.strLen)
		renderTitle ("TimeLapse Pro", 1, 1);
	else
		renderTitle ("TimeLapse Pro", 1, 0);
	
	u8g.setFontRefHeightText();
	
	u8g.setFont(fontH3); 
	h = u8g.getFontAscent()-u8g.getFontDescent();
		
	// Intervalo
	posY=25;
	d = u8g.getStrWidth(menuTimeLapsePro.str[config.language][0])/2;
  	u8g.setDefaultForegroundColor();
	//u8g.drawBox(32-d-1, 20-h+1, d*2+2, h);        // draw cursor bar
	//u8g.setDefaultBackgroundColor();
	u8g.drawStr(32-d, posY, menuTimeLapsePro.str[config.language][0]); 
	u8g.setDefaultForegroundColor();
 

	// Distancia
	d = u8g.getStrWidth(menuTimeLapsePro.str[config.language][1])/2;
  	u8g.setDefaultForegroundColor();
	//u8g.drawBox(96-d-1, 20-h+1, d*2+2, h);        // draw cursor bar
    //u8g.setDefaultBackgroundColor();
  	u8g.drawStr(96-d, posY, menuTimeLapsePro.str[config.language][1]);  
                                                                         
	// Empezar            
	posY=63;
	d = u8g.getStrWidth(menuTimeLapsePro.str[config.language][2]);  
	u8g.setDefaultForegroundColor();
	if (STATE == SCN04S01) {
 		if (menuItem==2) {
			//u8g.drawBox(64-(d/2)-2, posY+1-h, d+2, h);  // draw cursor bar
			renderBoxAroundText (64,posY,menuTimeLapsePro.str[config.language][2]);
			//u8g.drawLine(64-d, 63, d+43, 63);  // draw cursor bar
  			u8g.setDefaultBackgroundColor();
		}                   
	}
  	u8g.drawStr(64-(d/2), posY, menuTimeLapsePro.str[config.language][2]);  
	
	// Calculos
	u8g.setDefaultForegroundColor();
	u8g.drawStr (1,64, sSequenceDuration);
	
	// Segundos de intervalo y literales de Distancia
	
	u8g.setFont(fontH1);           
	h = u8g.getFontAscent()-u8g.getFontDescent();
	
	// Segundos de intervalo
	snprintf(buffer, sizeof(buffer), "%ds", currentTLInterval);
	posY=45;
	u8g.setDefaultForegroundColor();
	d = u8g.getStrWidth(buffer)/2; 	    
    if (STATE == SCN04S01 || STATE == SCN04S03) {
 		if (STATE == SCN04S01 && menuItem==0) {  
			// Segundos de intervalo                
			//u8g.drawBox(32-d-1, 40-h+1, d*2+2, h);        // draw cursor bar             
			//u8g.drawLine (31-d,41,d+33,41);
		   	//u8g.setDefaultBackgroundColor();
			renderSelectCursor (32-d,posY+1,h,d*2);
		}
	 	u8g.drawStr(32-d, posY, buffer);  
 	}                                                                   
    
	
	if (STATE == SCN04S02) {
		d = u8g.getStrWidth(buffer)/2; 	                                                          
		u8g.drawBox(32-d-1, posY-h+1, d*2+2, h);        // draw cursor bar             
		u8g.setDefaultBackgroundColor();                                       
		u8g.drawStr(32-d, posY, buffer);   		
	}
	    
	// literales de Distancia  
	u8g.setDefaultForegroundColor();
	d = u8g.getStrWidth(menuTimeLapsePro.strDistance[config.language][currentTLDistance])/2;
	if (STATE == SCN04S01 || STATE == SCN04S02) {
 		if (STATE == SCN04S01 && menuItem==1) {  
			// Segundos de intervalo                
			//u8g.drawBox(96-d-1, 40-h+1, d*2+2, h);        // draw cursor bar             
			//u8g.drawLine(95-d, 41, d+97, 41);        // draw cursor bar             
			//u8g.setDefaultBackgroundColor();
			renderSelectCursor (96-d,posY+1,h,d*2);
		}
		u8g.drawStr(96-d, posY, menuTimeLapsePro.strDistance[config.language][currentTLDistance]); 
   	}
    
   	if (STATE == SCN04S03) {
		d = u8g.getStrWidth(menuTimeLapsePro.strDistance[config.language][menuItem])/2; 	                                                          
		u8g.drawBox(96-d-1, posY-h+1, d*2+2, h);        	// draw cursor bar             
		u8g.setDefaultBackgroundColor();                                       
		u8g.drawStr(96-d, posY, menuTimeLapsePro.strDistance[config.language][menuItem]);   		
	}
} 