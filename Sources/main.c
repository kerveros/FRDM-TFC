#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"

int main(void)
{
   uint32_t t,i=0, ana;
   int guardar,dato=0;
   float kp=0.4, kp_motor=0.8, motor;
   int mmotor_max, motor_min;
   //int arreglo[128];
   //run, y, Variable_contador=0
   
   //int32_t guardar;
   
   TFC_Init();
   
   for(;;)
      
   {
         TFC_Task();

         switch(dato)
         {
         default:
         case 0 :
                 TFC_SetMotorPWM(0,0);
                 TFC_SetServo(0,0);
            //Demo mode 0 just tests the switches and LED's
            if(TFC_PUSH_BUTTON_1_PRESSED)
               //for(run=0; run<10000000; run++){} //2,500,000 1 seg
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
                                for(i=0;i<128;i++)
                                {
                                   TERMINAL_PRINTF("%X,",LineScanImage0[i]);
                                }
                                  
                                for(i=0;i<128;i++){
                                         if(LineScanImage0[i]<ana){
                                            ana = LineScanImage0[i];
                                           guardar = i;
                                       }
                                 }
                                motor = guardar;
                                if (motor<64){
                                   motor = (motor/64)*kp_motor;
                                }
                                else{
                                   motor = ((127-motor)/64)*kp_motor;
                                }
                                  
                                guardar = kp*(guardar - 64);  //-1, 1
                                TFC_SetServo(0,(float)guardar/64.0f);
                                                
                        TFC_SetMotorPWM(motor, motor);
                        
                         }
            break;
         }
   }  
   return 0;
}

