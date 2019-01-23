void calculateMinimumTimeLapseDuration (void) {
	// Regeneramos el menu Duracion del Timelapse
	
	// Distancia maxima del intervalo (Velocidad * Tiempo Movimiento Máximo)
	unsigned long dmi = sliderSpeed * (newMenuTimeLapsePro.interval[menuItem] - tfd);

	// Número de intervalos mínimos
	int nim = railLenght / dmi;

	// Duración mínima del timelapse
	minimumTimeLapseDuration= nim * newMenuTimeLapsePro.interval[menuItem];

}

// void calculateTimeLapseParameters(void) {
// 	float intervalDistance;
// 	int velocidad=15; //mm/s

// 	// Calculamos el numero de disparos necesarios para hacer una secuencia de la duración definida
// 	// Tomamos una secuencia de 25 fps	
// //	numShots= 25*sequenceDuration;
// //	intervalDistance = (float)railLenght/numShots;
// //	programParameters.intervalDuration = (float)(timelapseDuration * 60)/numShots*1000;
// //	programParameters.motorMoveDuration = (float)intervalDistance/velocidad*1000;
// //	maxExposure = programParameters.intervalDuration - programParameters.motorMoveDuration;

// 	_debug ("intervalDistance");   _debugln (intervalDistance);
// 	_debug ("intervalDuration ");  _debugln (programParameters.intervalDuration);
// 	_debug ("motormoveDuration "); _debugln (programParameters.motorMoveDuration);


// }