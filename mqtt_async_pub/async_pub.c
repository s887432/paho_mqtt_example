#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "MQTTClient.h"

//#define ADDRESS     "tcp://localhost:1883"
//#define ADDRESS     "tcp://10.160.39.61:1883"
#define ADDRESS     "tcp://"
#define PORT		":1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "mchp/dashboard"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("##Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
	char addr[64];

	if( argc != 2 )
	{
		printf("USAGE: mqtt_sub BROKER_IP\r\n");
		exit(1);
	}

	sprintf(addr, "%s%s%s", ADDRESS, argv[1], PORT);
	
	time_t t = time(NULL);
	struct tm *tm;
	char s[64];
	
    MQTTClient_create(&client, addr, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    
	conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
	while(1) {
		tm = localtime(&t);
		strftime(s, sizeof(s), "%c", tm);	
		printf("%s\r\n", s);

		pubmsg.payload = s;
		pubmsg.payloadlen = strlen(s);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		deliveredtoken = 0;
		
		

		MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
		
		while(deliveredtoken != token);    

		sleep(1);
	}
	MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    
	return rc;
}
