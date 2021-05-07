#include <AWS_IOT.h>
#include <WiFi.h>
#include "Nextion.h"

AWS_IOT Home_auto;

//ENTER YOUR SSID, PASSWORD, ENDPONT, CLIENT ID AND TOPIC.

char WIFI_SSID[]="";
char WIFI_PASSWORD[]="";
char HOST_ADDRESS[]="";
char CLIENT_ID[]= "";
char TOPIC_NAME[]= "";

//ASSIGNING PIN NUMBER
int BC_1 =  25;
int BC_2 =  32;
int BC_3 =  33;

//PIN VALUE TO SWITCH HIGH AND LOW
int comp1 = 0;
int comp2 = 0;
int comp3 = 0;

//DECLARE A BUTTON OBJECT [page id:0,component id:1, component name: "b0"].

NexButton  b0 = NexButton(0, 2,"b0");
NexButton  b1 = NexButton(0, 3,"b1");
NexButton  b3 = NexButton(0, 4,"b2");

//REGISTER A BUTTON OBJECCT TO THE TOUCH EVENT LIST

NexTouch *nex_listen_list[] = 
{
    &b0,
    &b1,
    &b3,
    NULL
};

// ALL LED1 CALLBACK FUNCTIONS.
void b0PopCallback(void *ptr)
{
 
 comp1 = (!comp1);
 digitalWrite(BC_1, comp1);
 

}

// ALL LED2 CALLBACK FUNCTIONS.
void b3PopCallback(void *ptr)
{
 
 comp3 = (!comp3);
 digitalWrite(BC_3, comp3);
 
  
}
// ALL LED3 CALLBACK FUNCTIONS.
void b1PopCallback(void *ptr)
{
 
 comp2 = (!comp2);
 digitalWrite(BC_2, comp2);
 
    
}



int status = WL_IDLE_STATUS; // STATUS OF WIFI
int  msgReceived = 0; // VARIABLE USED TO CHECK THE CONDITIONAL STATEMENT IN IF MSG IS RECEIVED 
char payload[512]; // VARIABLE USED TO STORE RECEIVED VALUE FROM THE SUBSCRIBED BROKER
char rcvdPayload[512];


// CALL BACK FUNCTION OF SUBSCRIBE
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{

    
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}



void setup() {
    Serial.begin(115200); // BAUD RATE OF SERIAL COMM
    delay(2000);

    while (status != WL_CONNECTED)                       // CHECK THE STATUS OF WIFI CONNECTION
    {
        Serial.print("Attempting to connect to SSID ");
        Serial.println(WIFI_SSID);
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Connect to WPA/WPA2 network. 
        delay(5000);                                   // WAIT FOR 5 SECOND FOR THE CONNECTION    
        Serial.println("Connected to wifi");


       // CHECK THE AWS CONNECTION AND SUBSCRIBE


    if(Home_auto.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==Home_auto.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfull");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(2000);



 //ASSIGN THE PIN AS OUTPUT
    pinMode( BC_1, OUTPUT);
    pinMode( BC_2, OUTPUT);
    pinMode( BC_3, OUTPUT);
    
    nexInit();
    
 // REGISTER THE POP EVENT CALLBACK FUNCTION OF THE CURRENT BUTTON. 
    b0.attachPop(b0PopCallback, &b0);
    b1.attachPop(b1PopCallback, &b1);
    b3.attachPop(b3PopCallback, &b3);


}

void loop() {


   // CHECK THE RECEIVED VALUE

    if(msgReceived == 1)
    {
        msgReceived = 0;
        if((int)rcvdPayload[0] == 49){   // CHECK ASCII VALUE OF 1 IS 49 
           b0PopCallback(&b0);
        }
        else if((int)rcvdPayload[0] == 50){   // CHECK ASCII VALUE OF 2 IS 50 
           b1PopCallback(&b1);
        }
         else if((int)rcvdPayload[0] == 51){    // CHECK ASCII VALUE OF 3 IS 51 
           b3PopCallback(&b3);
        }

        
    }
    vTaskDelay(1000 / portTICK_RATE_MS); 
   
}
