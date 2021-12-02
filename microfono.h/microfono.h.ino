// Librerias necesarias
#include <PDM.h>
#include <arduinoFFT.h>

// Parámetros para el procesamiento
#define Muestras 256 // Número de muestras para calcular la FFT
#define Muestreo 16000 // Frecuencia de muestreo

// Declaración de estructuras
void onPDMdata(void);
arduinoFFT FFT = arduinoFFT();

// Variables del programa
double vReal[Muestras];
double vImag[Muestras];
short sampleBuffer[Muestras];
volatile int samplesRead;
 
void setup() {
  // Configuración del puerto serial
  Serial.begin(115200);
  while (!Serial);
  // Configuración del micrófono
  PDM.onReceive(onPDMdata);
  PDM.setBufferSize(Muestras);
  if (!PDM.begin(1, Muestreo)) 
  {
    Serial.println("NO se puedo inicializar PDM!");
    while (1);
  }
}

void loop() {
      // Almacenamiento de los datos en vReal[]
      if (samplesRead) 
      {
       for (int i = 0; i < Muestras; i++) 
       {
        vReal[i]=(double)sampleBuffer[i];
        vImag[i]=0;
        }
       samplesRead = 0;
       // Cálculo de la transformada de Fourier
       FFT.Windowing(vReal, Muestras, FFT_WIN_TYP_HAMMING, FFT_FORWARD); //  Ventaneo
       FFT.Compute(vReal, vImag, Muestras, FFT_FORWARD); // Cálculo de la FFT
       FFT.ComplexToMagnitude(vReal, vImag, Muestras); // Cálculo de la magnitud para cada frecuencia
       double peak = FFT.MajorPeak(vReal, Muestras, Muestreo); // Detección del mayor pico
       
       Serial.println(peak);  // Imprime el pico encontrado
       if(peak>774 && peak < 794){digitalWrite(LEDR,LOW);}
       else {digitalWrite(LEDR,HIGH);}
       if(peak>870 && peak < 890){digitalWrite(LEDG,LOW);}
       else {digitalWrite(LEDG,HIGH);}
       if(peak>977 && peak < 997){digitalWrite(LEDB,LOW);}
       else {digitalWrite(LEDB,HIGH);}
    }
}
void onPDMdata() {
  int bytesAvailable = PDM.available();
  int bytesRead = PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesRead / 2;
}
