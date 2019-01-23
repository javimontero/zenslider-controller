#include "render/rVideoPro.h"
#include "render/rTimeLapsePro.h"
#include "render/rAstroFocus.h"
#include "render/rFollowFocus.h"
#include "render/rRailLen.h"
   

void u8gsetup(void) {
	if ( u8g.getMode() == U8G_MODE_R3G3B2 ) 
    	u8g.setColorIndex(255);     // white
  	else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    	u8g.setColorIndex(3);         // max intensity
  	else if ( u8g.getMode() == U8G_MODE_BW )
    	u8g.setColorIndex(1);         // pixel on
}

int u8gnextpage (void) {return u8g.nextPage(); }

void u8gfirstpage (void) {u8g.firstPage(); }

/***********************************************************************************/
/* Renderizado de menus en la pantalla */
void drawMenu(char *menu[], int numElements) {
  	uint8_t i, h;
  	u8g_uint_t w, d;
  
  	u8g.setFont(fontH2);
  	u8g.setFontRefHeightAll();
  	u8g.setFontPosTop();

	h = u8g.getFontAscent()-u8g.getFontDescent();
	w = u8g.getWidth();

  	for (i=0; i<maxElements; i++) {
	  	d = (w-u8g.getStrWidth(menu[vpIndex+i]))/2;
	    u8g.setDefaultForegroundColor();
	    if ( i == vpOffset ) {         			// currently selected menu item
	    	u8g.drawBox(0, i*h+1, w, h);        // draw cursor bar
	    	u8g.setDefaultBackgroundColor();
	    }
	    u8g.drawStr(d, i*h, menu[vpIndex+i]);	
  	}
}              
  
void drawConfiguration (void) {
 	uint8_t h, posY, posX;
 	u8g_uint_t d;
 	//snprintf (buffer, sizeof(buffer),"%s","");	// buffer=""


	if (menuItem==menuConfiguration.numElements)
		renderTitle (menuConfiguration.strTitle[config.language],1,1);
		
	else
	 	renderTitle (menuConfiguration.strTitle[config.language],1,0);
		//renderTitle ("cambio idioma",1,0);      
	                                                                                
  	u8g.setFont(fontH2);
	h = u8g.getFontAscent()-u8g.getFontDescent();
	//u8g.setFontPosTop();
	u8g.setFontRefHeightText();
	 
  	// Render de los títulos de los menus
  	// Idioma
  	posY= 27;
  	d = u8g.getStrWidth(menuConfiguration.strElement[config.language][0]);
  	u8g.setDefaultForegroundColor();
  	u8g.drawStr(1, posY, menuConfiguration.strElement[config.language][0]);
  	// Invertir giro encoder
  	posY+=h+1; 
	d = u8g.getStrWidth(menuConfiguration.strElement[config.language][1]);
  	u8g.setDefaultForegroundColor();
  	u8g.drawStr(1, posY, menuConfiguration.strElement[config.language][1]);     
  	// Longitud rail
  	posY+=h+1; 
	d = u8g.getStrWidth(menuConfiguration.strElement[config.language][2]);
  	u8g.setDefaultForegroundColor();
  	u8g.drawStr(1, posY, menuConfiguration.strElement[config.language][2]); 


 
  	posY= 27;
  	posX= u8g.getStrWidth(menuConfiguration.strElement[config.language][0])+10;
  	//d = u8g.getStrWidth(menuConfiguration.strLanguages[config.language]);
  	//d = u8g.getStrWidth(buffer)+2;

  	//u8g.setDefaultForegroundColor();
  	if (STATE==SCN02S01) {
  		d = u8g.getStrWidth(menuConfiguration.strLanguages[config.language]);
  		posX=128-5-d;
  		if (menuItem==0)
  			renderSelectCursor (posX-2,posY+1,h+1,d+2);
		
		u8g.drawStr (posX, posY, menuConfiguration.strLanguages[config.language]);
	}
  	
  	if (STATE == SCN02S02)  {	// Seleccion del idioma
  		d = u8g.getStrWidth(menuConfiguration.strLanguages[menuItem]);
  		posX=128-5-d;
  		u8g.drawBox(posX-1, posY-h+1, d+2, h);
		u8g.setDefaultBackgroundColor();	
		u8g.drawStr (posX, posY, menuConfiguration.strLanguages[menuItem]);
	}


  	// Invertir giro encoder
  	u8g.setDefaultForegroundColor();
	posY+=h+1; 
  	//posX= u8g.getStrWidth(menuConfiguration.strElement[config.language][1])+10;
	d = u8g.getStrWidth(menuConfiguration.strInvertSelect[config.encoderSpin]);
	posX = 128-5-d;
	
  	//u8g.setDefaultForegroundColor();
  	if (STATE==SCN02S01) {	
  		if (menuItem==1) {
	 		// u8g.drawBox(posX-1, posY+2, d+2, h); 	// draw cursor bar
	  	// 	u8g.setDefaultBackgroundColor();
  			renderSelectCursor (posX-2,posY+1,h+1,d+2);
		}
	}
	u8g.drawStr(posX, posY, menuConfiguration.strInvertSelect[config.encoderSpin]);

	// Longitud rail
	posY+=h+1;
	snprintf (buffer, sizeof (buffer), "SET");
  	//posX= u8g.getStrWidth(menuConfiguration.strElement[config.language][2])+10;
  	posX =128-5-u8g.getStrWidth(buffer); 
  	//d = u8g.getStrWidth(menuConfiguration.strLanguages[config.language]);
  	d = u8g.getStrWidth(buffer)+2;
  	//u8g.setDefaultForegroundColor();
  	if (STATE==SCN02S01 && menuItem == 2) 
  		renderSelectCursor (posX-2,posY+1,h+1,d);
	u8g.drawStr (posX, posY, buffer);
	
}

void drawTimeLapse() {
  	uint8_t h, posY;
  	u8g_uint_t d;
     
	u8g.setFontRefHeightText();

	// Título con botón Back          
	if (STATE==SCN03S01 && menuItem==menuTimeLapse.strLen)
		renderTitle (menuTimeLapse.strTitle[config.language], 1, 1);
	else
		renderTitle (menuTimeLapse.strTitle[config.language], 1, 0);		
                                                            
	// Intervalo
	posY=25;
  	u8g.setFont(fontH3); 
	d = u8g.getStrWidth(menuTimeLapse.str[config.language][0])/2;
  	u8g.setDefaultForegroundColor();
  	u8g.drawStr(64-d, posY, menuTimeLapse.str[config.language][0]);  

    // Empezar            
	posY=63;
	d = u8g.getStrWidth(menuTimeLapse.str[config.language][1]);  
	u8g.setDefaultForegroundColor();
	if (STATE == SCN03S01 && menuItem==1) {
		//u8g.drawBox(64-(d/2)-2, posY+1-h, d+2, h);  // draw cursor bar
		renderBoxAroundText (64,posY,menuTimeLapse.str[config.language][1]);
		//u8g.drawLine(64-d, 63, d+43, 63);  // draw cursor bar
		u8g.setDefaultBackgroundColor();
	}
	u8g.drawStr(64-(d/2), posY, menuTimeLapse.str[config.language][1]);

    
	// Segundos de intervalo
	posY=45;
 	u8g.setFont(fontH1);
	h = u8g.getFontAscent()-u8g.getFontDescent(); 
  	u8g.setDefaultForegroundColor();
	//d = u8g.getStrWidth(menuTimeLapse.strInterval[currentTLInterval])/2; 	    

	snprintf(buffer, sizeof(buffer), "%ds", currentTLInterval);

	d = u8g.getStrWidth(buffer)/2;

    if (STATE == SCN03S01 && menuItem==0) 
		renderSelectCursor (64-d,posY+1,h,d*2);
	
	if (STATE==SCN03S01)
		u8g.drawStr(64-d, posY, buffer);  

	if (STATE == SCN03S02) { // Estamos en la seleccion de los segundos de intervalo
    	d = u8g.getStrWidth(buffer)/2; 	                                                          
		u8g.drawBox(64-d-1, posY-h+1, d*2+2, h);        // draw cursor bar             
		u8g.setDefaultBackgroundColor();                                       
		u8g.drawStr(64-d, posY, buffer);   		
	}
}       



void drawRVM (){
  	u8g_uint_t d;

	renderTitle (strRunning[config.language],0,0);
              
  	u8g.setFont(fontH3); 
	d = u8g.getStrWidth(strSpeed[config.language])/2;
  	u8g.setDefaultForegroundColor();
  	u8g.drawStr(64-d, 25, strSpeed[config.language]);  
	
  	u8g.setFont(fontH1);
  	d = u8g.getStrWidth(menuVideo.str[menuItem])/2;
  	u8g.drawStr(64-d, 45, menuVideo.str[menuItem]);    

	// // Vamos actualizando los parametros del programa
	// programParameters.speed=menuVideo.speed[menuItem];
	// programParameters.intervalDuration=0;


}

void drawRTM (){                    
	renderTitle (strRunning[config.language],0,0);
	renderAnimationRuler();
	renderRunningFooter(); 

	u8g.setFont(fontH3); 
	//d = u8g.getStrWidth(strSpeed[config.language])/2;
  	u8g.setDefaultForegroundColor();
  	//u8g.drawStr(4, 45, "SEQ: 1:35:43"); // Test render
}          


void drawStopMotion() {
  	uint8_t h, posY, posX;
  	u8g_uint_t d;
     
	u8g.setFontRefHeightText();

	// Título con botón Back          
	if (STATE==SCN05S01 && menuItem==menuStopMotion.strLen)
		renderTitle (menuStopMotion.strTitle[config.language], 1, 1);
	else
		renderTitle (menuStopMotion.strTitle[config.language], 1, 0);		
                                                            
	// Label Distancia
	posY=25;
  	u8g.setFont(fontH3); 
	d = u8g.getStrWidth(menuStopMotion.str[config.language][0])/2;
  	u8g.setDefaultForegroundColor();
  	u8g.drawStr(64-d, posY, menuStopMotion.str[config.language][0]);  

    // Label Disparar            
	posY=62;
	posX=94;
	d = u8g.getStrWidth(menuStopMotion.str[config.language][1]);  
	u8g.setDefaultForegroundColor();
	if (STATE == SCN05S01 && menuItem==1) {
		//u8g.drawBox(64-(d/2)-2, posY+1-h, d+2, h);  // draw cursor bar
		renderBoxAroundText (posX,posY,menuStopMotion.str[config.language][1]);
		//u8g.drawLine(64-d, 63, d+43, 63);  // draw cursor bar
		u8g.setDefaultBackgroundColor();
	}
	u8g.drawStr(posX-(d/2), posY, menuStopMotion.str[config.language][1]);

    
	// Valores distancia
	posY=45;
 	u8g.setFont(fontH1);
	h = u8g.getFontAscent()-u8g.getFontDescent(); 
  	u8g.setDefaultForegroundColor();
	d = u8g.getStrWidth(menuStopMotion.strDistance[currentSMDistance])/2; 	    
    if (STATE == SCN05S01 && menuItem==0) 
		renderSelectCursor (64-d,posY+1,h,d*2);
	
	if (STATE==SCN05S01)
		u8g.drawStr(64-d, posY, menuStopMotion.strDistance[currentSMDistance]);  

	if (STATE == SCN05S02) { // Estamos en la seleccion de la distancia
    	d = u8g.getStrWidth(menuStopMotion.strDistance[menuItem])/2; 	                                                          
		u8g.drawBox(64-d-1, posY-h+1, d*2+2, h);        // draw cursor bar             
		u8g.setDefaultBackgroundColor();                                       
		u8g.drawStr(64-d, posY, menuStopMotion.strDistance[menuItem]);   		
	}

	// Render siguiente disparo
	u8g.setFont (fontH3);
	u8g.setDefaultForegroundColor();
	snprintf(buffer,sizeof(buffer),"%s: %u",strShots[config.language], currentNumShots);    
	u8g.drawStr (2,61,buffer);	
	u8g.setDefaultForegroundColor();          
}       

void renderRunningFooter() { 
	u8g.setFont (fontH3);
	u8g.setDefaultForegroundColor();
	u8g.drawBox (0,53,128,11);
	u8g.setDefaultBackgroundColor();
	if (STATE==RTM)
		snprintf (buffer, sizeof(buffer), "%s:TL(%ds)", strMode[config.language], currentTLInterval);
	else
		snprintf (buffer, sizeof(buffer), "%s: VM", strMode[config.language]);		
	u8g.drawStr (126-u8g.getStrWidth(buffer),61, buffer); // Justificamos a la derecha
	//strcpy (buffer,"Disparos: ");
	snprintf(buffer,sizeof(buffer),"%s:%u",strShots[config.language], currentNumShots);    
	//Numero de disparos 
	u8g.drawStr (2,61,buffer);	
	u8g.setDefaultForegroundColor();                                             
}

/***********************************************************************************/
void renderAnimation (void) {
	for (int i=8; i<128; i+=8) 
		u8g.drawCircle (i,60,2);
		
	u8g.drawDisc (timerAnimationItem*8,60,2);
	
	if (millis()-timerAnimation > F_ANIMATION) {
		timerAnimationItem++;
		timerAnimation=millis();
		if (timerAnimationItem>15) timerAnimationItem=1;
	}
}  

void renderAnimationRuler(void) {
	uint8_t posY,posX,maxWidth; 
	// Escala    
	posY= 30;
	posX=12; 
	maxWidth=100; 

	for (int i=0; i < 51;i++) {
		u8g.drawPixel (posX + (i*2),posY);
	}                    

	for (int i=0; i < (maxWidth/10)+1 ; i++) {
		u8g.drawPixel (posX+(i*10),posY-1);
		u8g.drawPixel (posX+(i*10),posY-2);
	}

	// Adaptamos el ancho de la caja al tiempo transcurrido del intervalo seleccionado
	u8g.drawBox (posX,posY-6, (maxWidth*(millis()-timerShootInterval)) / programParameters.intervalDuration,3);

	// Reseteamos el timer
	if (millis() - timerAnimation > menuTimeLapse.interval[currentTLInterval]) {
		timerAnimation=millis();  
	}
}

/***********************************************************************************/
/* Dibuja titulo en la parte superior con el nombre de la opción				   */
/* char *t: 				Texto a mostrar                                        */
/* int withBackButton:      Se muestra o no el botón de volver					   */ 
/* int backButtonSelected:  Si el botón está activo, mostrar la posición           */
/*							correspondiente, seleccionado o no seleccionado		   */
/***********************************************************************************/							
void renderTitle (char *t, int withBackButton, int backButtonSelected) {
    /* Titulo de la pantalla */ 
  	u8g.setFont(fontH2);
  	u8g.setDefaultForegroundColor();
	u8g.setFontPosBaseline();
	
	uint8_t w;
	
	if (withBackButton) {
		u8g.drawBox (16,0,128,13);       
		u8g.setDefaultBackgroundColor();
  		u8g.drawStr(64-(u8g.getStrWidth(t)/2), 10, t);
		u8g.setDefaultForegroundColor();
  
		// Dibujamos la flecha de retorno
	   	if (backButtonSelected) u8g.drawBitmapP (0,0,2,16,arrow_select_bitmap);
		else u8g.drawBitmapP (0,0,2,16,arrow_noSelect_bitmap);
    } else { // No tiene boton Back, usamos todo el ancho del LCD  
		u8g.drawBox (0,0,128,13);       
		u8g.setDefaultBackgroundColor();
		//Serial.print ("renderTitle:"); Serial.print (t); Serial.print("\n");
		w= 64-(u8g.getStrWidth(t)/2);
  		u8g.drawStr(w, 10, t);
  		//u8g.drawStr(w, 40, t);

	}    
	u8g.setDefaultForegroundColor();
	
}                    
  
/***********************************************************************************/
// Dibuja el marco que define la seleccion actual
// int x: 				Posición x de la esquina inferior izquierda 
// int y:               Posición y de la esquina inferior izquierda 
// int h:               Altura
// int w:               Anchura
/***********************************************************************************/							
void renderSelectCursor (int x, int y, int h, int w) {
	//w=w+1;
	// Esquina inferior izquierda
	u8g.drawPixel (x,y);
	u8g.drawPixel (x+1,y);
	u8g.drawPixel (x+2,y);
	u8g.drawPixel (x,y-1);
	u8g.drawPixel (x,y-2);
	
	// Esquina superior izquierda
	u8g.drawPixel (x,y-h);
	u8g.drawPixel (x+1,y-h);
	u8g.drawPixel (x+2,y-h);
	u8g.drawPixel (x,y-h+1);
	u8g.drawPixel (x,y-h+2);
		
	// Esquina superior derecha
	u8g.drawPixel (x+w,y-h);
	u8g.drawPixel (x+w-1,y-h);
	u8g.drawPixel (x+w-2,y-h);
	u8g.drawPixel (x+w,y-h+1);
 	u8g.drawPixel (x+w,y-h+2);    
   
	// Esquina inferior derecha
	u8g.drawPixel (x+w,y);
	u8g.drawPixel (x+w-1,y);
	u8g.drawPixel (x+w-2,y);
	u8g.drawPixel (x+w,y-1);
	u8g.drawPixel (x+w,y-2);
	
}                       
void renderBoxAroundText (int x, int y, char * t ){ 
	u8g.setFontRefHeightText();
	uint8_t w = u8g.getStrWidth(t);
	uint8_t h=u8g.getFontAscent()-u8g.getFontDescent();
	
	u8g.drawBox(x-(w/2)-1, y-h-1, w+1, h+2);
	
}

void drawSplashScreen() {
 // u8g.setFont(fontH2);
 //  	u8g.setFontRefHeightAll();
 //  	u8g.setFontPosTop();

 //  	u8g.drawStr(2, 25, gitversion); 
}

void renderVersionNumber (){
	u8g.setFont (fontH3);
	u8g.setDefaultForegroundColor();

	u8g.drawLine(15, 56, 113, 56);  // draw cursor bar
	snprintf (buffer, sizeof (buffer), "ZenSlider %s", version);
	
	// Centramos el literal
	u8g.drawStr (64-(u8g.getStrWidth(buffer)/2),64, buffer);
}
