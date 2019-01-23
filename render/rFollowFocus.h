
void drawFollowFocus() {
  	uint8_t h,posY,posX;
  	u8g_uint_t d;         
  	u8g.setFontRefHeightText();

	if (menuItem==numFocusPointsDefined() + 2 || menuItem==7) 
		renderTitle ("FollowFocus", 1, 1);
	else
		renderTitle ("FollowFocus", 1, 0);
		
	u8g.setFont(fontH2); 
	h = u8g.getFontAscent()-u8g.getFontDescent();
		
	// Literal Punto A
	posX=2;
	posY=26;
	u8g.drawStr(posX, posY, "A"); 

		
	// Literal Punto B
	posY=39;
	u8g.drawStr(posX, posY, "B"); 
	
	// Literal Punto C
	posY=51;
	u8g.drawStr(posX, posY, "C"); 

	// Segunda columna
	// Literal Punto D
	posY=26;
	posX=66;
	u8g.drawStr(posX, posY, "D"); 

		
	// Literal Punto E
	posY=39;
	u8g.drawStr(posX, posY, "E"); 
	
	// Literal Punto F
	posY=51;
	u8g.drawStr(posX, posY, "F"); 


	// Option button punto A
	posX=16;
	posY=26;
	u8g.drawCircle (posX,posY-5,3);
	if (focusPoints[0].isDefined) u8g.drawDisc (posX,posY-5,1);

	// Option button punto B
	posY=39;
	u8g.drawCircle (posX,posY-5,3);
	if (focusPoints[1].isDefined) u8g.drawDisc (posX,posY-5,1);

	// Option button punto C
	posY=51;
	u8g.drawCircle (posX,posY-5,3);
	if (focusPoints[2].isDefined) u8g.drawDisc (posX,posY-5,1);

	// Option button punto D
	posX=80;
	posY=26;
	u8g.drawCircle (posX,posY-5,3);
	if (focusPoints[3].isDefined) u8g.drawDisc (posX,posY-5,1);

	// Option button punto E
	posY=39;
	u8g.drawCircle (posX,posY-5,3);
	if (focusPoints[4].isDefined) u8g.drawDisc (posX,posY-5,1);

	// Option button punto F
	posY=51;
	u8g.drawCircle (posX,posY-5,3);
	if (focusPoints[5].isDefined) u8g.drawDisc (posX,posY-5,1);

	// SET punto A
	posX=26;
	posY=26;
	snprintf (buffer,sizeof(buffer), "set");

	u8g.drawStr(posX, posY, buffer); 
		if (menuItem==0) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	} 

	// SET punto B
	posY=39;
	u8g.drawStr(posX, posY, buffer); 
		if (menuItem==1 && numFocusPointsDefined() >= 1) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	} 
	// SET punto C
	posY=51;
	u8g.drawStr(posX, posY, buffer); 
		if (menuItem==2 && numFocusPointsDefined() >= 2) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	} 
	
	// SET punto D
	posX=90;
	posY=26;
	u8g.drawStr(posX, posY, buffer); 
		if (menuItem==3 && numFocusPointsDefined() >= 3) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	} 

	// SET punto E
	posY=39;
	u8g.drawStr(posX, posY, buffer); 
		if (menuItem==4 && numFocusPointsDefined() >= 4) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	} 

	// SET punto F
	posY=51;
	u8g.drawStr(posX, posY, buffer); 
		if (menuItem==5 && numFocusPointsDefined() >= 5) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	} 

  	// definir            
	posY=63;
	snprintf (buffer, sizeof(buffer), "Run");
	d= u8g.getStrWidth (buffer);
	//u8g.drawStr(64-(d/2), posY, buffer);

	u8g.setDefaultForegroundColor();

	//if ((menuItem == numFocusPointsDefined()+1) || (numFocusPointsDefined() == 6 && menuItem== 6)) {
	if (numFocusPointsDefined()<6) {
		if (numFocusPointsDefined()== menuItem-1) {
			u8g.drawBox (64-(d/2)-1, posY-h+1, d+1, h);
			u8g.setDefaultBackgroundColor();
		}
	} else {
		if (menuItem==6) {
			u8g.drawBox (64-(d/2)-1, posY-h+1, d+1, h);
			u8g.setDefaultBackgroundColor();	
		}
	}

	u8g.drawStr(64-(d/2), posY, buffer);
	u8g.setDefaultForegroundColor();

	

} 

void drawSelectPoint () {
	uint8_t h,posY,posX;
  	u8g_uint_t d;         
  	u8g.setFontRefHeightText();
	
	//renderTitle ("Definici\xF3n punto", 0, 0);
	renderTitle ("Point definition", 0, 0);
	u8g.setFont(fontH2); 
	h = u8g.getFontAscent()-u8g.getFontDescent();

	posX=2;
	posY=30; 

	int sp = 7; 
	// <<<
	snprintf (buffer, sizeof(buffer), "<<<");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S02 && menuItem == -3) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	} 


	posX += u8g.getStrWidth (buffer) + sp;
	snprintf (buffer, sizeof(buffer), "<<");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S02 && menuItem == -2) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}	

	posX += u8g.getStrWidth (buffer) + sp;
	snprintf (buffer, sizeof(buffer), "<");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S02 && menuItem == -1) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

	posX += u8g.getStrWidth (buffer) + sp;
	snprintf (buffer, sizeof(buffer), "OK");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S01 && menuItem == 0) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}
	if (STATE ==SCN09S02 && menuItem==0) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	
	posX += u8g.getStrWidth (buffer) + sp;
	snprintf (buffer, sizeof(buffer), ">");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S02 && menuItem == 1) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	posX += u8g.getStrWidth (buffer) + sp;
	snprintf (buffer, sizeof(buffer), ">>");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S02 && menuItem == 2) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	posX += u8g.getStrWidth (buffer) + sp;
	snprintf (buffer, sizeof(buffer), ">>>");
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S02 && menuItem == 3) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}


	/*Duracion*/ 

	posX=2;
	posY=50; 
	sp=7;

	//snprintf (buffer, sizeof(buffer), "Duraci\xF3n:");
	snprintf (buffer, sizeof(buffer), "Duration:");
	u8g.drawStr(posX, posY, buffer); 

	// Decenas de segundo
	posX = 70;
	snprintf (buffer, sizeof(buffer), "%d", selectedDurationSecondsTens);
	d = u8g.getStrWidth(buffer);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S01 && menuItem == 1) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}
	if (STATE==SCN09S04) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}
	
	// Unidades de segundo
	posX += sp;
	snprintf (buffer, sizeof(buffer), "%d", selectedDurationSecondsUnits);
	u8g.drawStr(posX, posY, buffer); 
		if (STATE == SCN09S01 && menuItem == 2) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}
	if (STATE==SCN09S05) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

	posX += sp;
	snprintf (buffer, sizeof(buffer), ",");
	u8g.drawStr(posX, posY, buffer); 
	
	// Decimas de segundo
	posX += sp;
	snprintf (buffer, sizeof(buffer), "%d", selectedDurationSecondsTenths);
	u8g.drawStr(posX, posY, buffer); 
	if (STATE == SCN09S01 && menuItem == 3) {
		renderSelectCursor (posX-2,posY+1,h,u8g.getStrWidth(buffer)+2);
	}
	if (STATE==SCN09S06) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

	posX += sp;
	snprintf (buffer, sizeof(buffer), "s");
	u8g.drawStr(posX, posY, buffer); 

	// OK

	posY=63;
	snprintf (buffer, sizeof(buffer), "Define");
	d= u8g.getStrWidth (buffer);
	//u8g.drawStr(64-(d/2), posY, buffer);

	u8g.setDefaultForegroundColor();
	if (STATE == SCN09S01 && menuItem == 4) {
		u8g.drawBox (64-(d/2)-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
	}
	u8g.drawStr(64-(d/2), posY, buffer);
	u8g.setDefaultForegroundColor();

	

}



void drawFollowFocusRun(void){
	uint8_t h,posY,posX;
  	u8g_uint_t d;         
  	u8g.setFontRefHeightText();

	if (STATE==SCN10S01 && menuItem==2)
		renderTitle ("Run FollowFocus", 1, 1);
	else
		renderTitle ("Run FollowFocus", 1, 0);
		
	u8g.setFont(fontH2); 
	h = u8g.getFontAscent()-u8g.getFontDescent();

	// Ir a Inicio
	//posX=50;
	posY=26;
	
	snprintf (buffer, sizeof(buffer), "Goto Start");
	d = u8g.getStrWidth(buffer);
	posX = 64-(d/2);
	u8g.drawStr(posX, posY, buffer); 
	if (menuItem == 0) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}

	// Run 
	posX=50;
	posY=46;
	
	if (currentPosition==numFocusPointsDefined()-1) {	// Estamos en el ultimo, tenemos que ir al inicio 
		snprintf (buffer, sizeof(buffer), "Move");	
	}
	else {
		switch (currentPosition) {
			case 0:
			snprintf (buffer, sizeof(buffer), "Move A->B");
			break;
			case 1:
			snprintf (buffer, sizeof(buffer), "Move B->C");
			break;
			case 2:
			snprintf (buffer, sizeof(buffer), "Move C->D");
			break;
			case 3:
			snprintf (buffer, sizeof(buffer), "Move D->E");
			break;
			case 4:
			snprintf (buffer, sizeof(buffer), "Move E->F");
			break;
			default:
			snprintf (buffer, sizeof(buffer), "Move");
			break;
		}
	}
	//snprintf (buffer, sizeof(buffer), "Move point2point");
	d = u8g.getStrWidth(buffer);
	posX = 64-(d/2);
	u8g.drawStr(posX, posY, buffer); 
	if (menuItem == 1) {
		u8g.setDefaultForegroundColor();
		u8g.drawBox(posX-1, posY-h+1, d+1, h);
		u8g.setDefaultBackgroundColor();
		u8g.drawStr(posX, posY, buffer); 
		u8g.setDefaultForegroundColor();
	}


}
