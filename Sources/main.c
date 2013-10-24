#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"

int main(void)
{
   uint32_t t,i=0, ana=0;
   int guardar,dato=0;
   float roberto;
   //int32_t guardar;
   
   TFC_Init();
   
   for(;;)
   {      
      //TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
         TFC_Task();

         //This Demo program will look at the middle 2 switch to select one of 4 demo modes.
         //Let's look at the middle 2 switches
         switch(dato)
         {
         default:
         case 0 :
        	 TFC_SetMotorPWM(0,0);
        	 TFC_SetServo(0,0);
            //Demo mode 0 just tests the switches and LED's
            if(TFC_PUSH_BUTTON_1_PRESSED)
               dato=3;
            
            break;
               
         case 1:
            
            //Demo mode 1 will just move the servos with the on-board potentiometers
            if(TFC_Ticker[0]>=20)
            {
               TFC_Ticker[0] = 0; //reset the Ticker
               //Every 20 mSeconds, update the Servos
               TFC_SetServo(0,TFC_ReadPot(0));
               TFC_SetServo(1,TFC_ReadPot(1));
            }
            //Let's put a pattern on the LEDs
            if(TFC_Ticker[1] >= 125)
            {
               TFC_Ticker[1] = 0;
               t++;
               if(t>4)
               {
                  t=0;
               }         
               TFC_SetBatteryLED_Level(t);
            }
            
            TFC_SetMotorPWM(0,0); //Make sure motors are off
            TFC_HBRIDGE_DISABLE;
         

            break;
            
         case 2 :
            
            //Demo Mode 2 will use the Pots to make the motors move
            TFC_HBRIDGE_ENABLE;
            TFC_SetMotorPWM(TFC_ReadPot(0),TFC_ReadPot(0));
                  
            //Let's put a pattern on the LEDs
            if(TFC_Ticker[1] >= 125)
               {
                  TFC_Ticker[1] = 0;
                     t++;
                     if(t>4)
                     {
                        t=0;
                     }         
                  TFC_SetBatteryLED_Level(t);
               }
            break;
         
         case 3 :
        	 
        	 TFC_HBRIDGE_ENABLE;
        	 
        	 if(TFC_PUSH_BUTTON_0_PRESSED){
        	 dato=0;
        	 }
        	 
         ana = 4096;
    
         if(TFC_Ticker[0]>100 && LineScanImageReady==1)
                        {
                         TFC_Ticker[0] = 0;
                         LineScanImageReady=0;
                         TERMINAL_PRINTF("\r\n");
                         TERMINAL_PRINTF("L:");
                         
                            if(t==0)
                               t=3;
                            else
                               t--;
                            
                            TFC_SetBatteryLED_Level(t);
                           
                            for(i=0;i<128;i++){
                                      	   TERMINAL_PRINTF("%x,",LineScanImage0[i]);
                                      	   //x hexa, i integer, f float......
                                      	   if(LineScanImage0[i]<ana){
                                      		   ana = LineScanImage0[i];
                                      	   	   guardar = i;
                                      	  }
                            }
                                         guardar = guardar - 64;
                                         //TERMINAL_PRINTF("\r\n");
                                         //TERMINAL_PRINTF("%i",guardar);
                                         TFC_SetServo(0,(float)guardar/64.0f); //Rescale to -1.0 to 1.0
                                        
                 if(guardar < -3 || guardar > 3){
                  TFC_SetMotorPWM(30/100.0f,30/100.0f); //al parecer 30 es lo ideal en la curva
                 }
                 else{
                 TFC_SetMotorPWM(50/100.0f,50/100.0f); //el ELSE era necesario, lo habiamos omitido
                 }

         /*
         	 if(guardar < 0){
         		 guardar = guardar*(-1);
         	 }
         	 roberto = (float)guardar;
         	roberto = roberto*(-0.02923) + 32;
         		TFC_SetMotorPWM(roberto/64.0f,roberto/64.0f); // 0 a 1
         		TERMINAL_PRINTF("\r\n");
         		TERMINAL_PRINTF("\r\n");
         		TERMINAL_PRINTF("%i",roberto/64);
         		*/
         }
                   
            break;
         }
   }
   
   return 0;
}
