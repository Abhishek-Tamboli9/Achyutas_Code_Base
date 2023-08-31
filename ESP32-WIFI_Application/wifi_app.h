/*
 * wifi_app.h
 *
 *  Created on: 29 Aug 2023
 *      Author: Abhishek
 */

#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_

#include "esp_netif.h"

/*							WIFI settings						*/
#define WIFI_AP_SSID					"ESP32_AP"				//AP NAME
#define WIFI_AP_PASS					"pass"					//AP PASSWORD
#define	WIFI_AP_CHANNEL					1						//AP CHANNEL
#define WIFI_AP_SSID_HIDDEN				0						//AP VISIBILITY
#define WIFI_AP_MAX_CONNECTIONS			5						//AP MAXIMUM CLIENTS
#define WIFI_AP_BEACON_INTERVAL			100						//AP BEACON: 100 ms DEFAULT
#define WIFI_AP_IP						"192.168.0.1"			//AP DEFAULT IP
#define	WIFI_AP_GATEWAY					"192.168.0.1"			//AP DEFAULT GATEWAY
#define WIFI_AP_NETMASK					"255.255.255.0"			//AP NETMASK
#define WIFI_AP_BANDWIDTH				WIFI_BW_HT20			//AP BANDWITH 20MHz (YOU CAN CHOOSE 40MHz ALSO)
#define WIFI_STA_POWER_SAVE				WIFI_PS_NONE			//POER SAVE NT USED
#define MAX_SSID_LENGTH					32						//IEEE SANDARD MAXIMUM
#define MAX_PASSWORD_LENGTH				64						//IEEE SANDARD MAXIMUM
#define MAX_CNNECTION_RETRIES			5						//RETRY NUMBER ON DISCONNECT

/**
 * netif object for station and Access point
 */
extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;

/**
 * Message id's for the Wifi application task
 */
typedef enum wifi_app_message
{
	WIFI_APP_MSG_START_HTTP_SERVER = 0,
	WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
	WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
	WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT,
	WIFI_APP_MSG_LOAD_SAVED_CREDENTIALS,
	WIFI_APP_MSG_STA_DISCONNECTED,
}wifi_app_message_e;

/**
 * Structure for the Message queue
 */
typedef struct wifi_app_queue_msg
{
	wifi_app_message_e msgID;
}wifi_app_queue_message_t;

/**
 * Sends the message to the queue
 * @param msgID message ID from the wifi_app_message_e enum.
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
 */
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

/**
 * Start the WIFI RTOS task
 */
void wifi_app_start(void);

#endif /* MAIN_WIFI_APP_H_ */
