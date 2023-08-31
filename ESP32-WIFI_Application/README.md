# ESP32 WiFi Application
This repository contains an example program for setting up an ESP32-based device as a WiFi Access Point (AP) using the ESP-IDF (Espressif IoT Development Framework).

## Overview
The program demonstrates how to configure and manage a WiFi Access Point on an ESP32 device. It sets up an AP with specific configuration settings and handles WiFi-related events. 
The program uses the ESP-IDF framework and FreeRTOS to manage tasks and events.

## Features
- Configures the WiFi Access Point (AP) with custom settings.
- Handles WiFi and IP events using event handlers.
- Starts an HTTP server (not implemented in this example).
- Illustrates queue-based communication between tasks.

## Getting Started
Follow these steps to run the program on your ESP32 device:

1. Install the ESP-IDF development environment as per Espressif's documentation.
   https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html
2. Connect your ESP32 device to your computer.
3. Open a terminal in the project directory and run the following commands:
4. Clone the repository to your local machine:
   > git clone https://github.com/Abhishek-Tamboli9/Achyutas_Code_Base.git
   > idf.py build
   > idf.py -p PORT flash --> Replace PORT with the appropriate serial port of your ESP32 device.
5. After flashing, the program will run on the device. You can monitor the output using
   idf.py -p PORT monitor

## Explanation of Macros and Functions
### Macros
1. **WIFI_AP_SSID**: This macro defines the Service Set Identifier (SSID) for the WiFi Access Point (AP).
2. **WIFI_AP_PASS**: This macro specifies the password required to connect to the WiFi Access Point (AP). It provides security for the network.
3. **WIFI_AP_CHANNEL**: This macro sets the WiFi channel on which the Access Point operates. WiFi operates on different channels within the 2.4 GHz frequency range. Different channels help reduce interference between nearby networks.
4. **WIFI_AP_SSID_HIDDEN**: This macro determines whether the AP's SSID is hidden or visible. If set to 1, the SSID won't be broadcasted in the list of available networks, enhancing network security.
5. **WIFI_AP_MAX_CONNECTIONS**: This macro defines the maximum number of clients that can be connected to the WiFi Access Point simultaneously.
6. **WIFI_AP_BEACON_INTERVAL**: This macro sets the interval at which the AP broadcasts beacon frames. Beacon frames are used for synchronization and network discovery by WiFi devices.
7. **WIFI_AP_IP**: This macro specifies the default IP address assigned to the WiFi Access Point.
8. **WIFI_AP_GATEWAY**: This macro defines the default gateway IP address for the WiFi Access Point. The gateway is used to route traffic between the AP's network and external networks.
9. **WIFI_AP_NETMASK**: This macro sets the subnet mask for the WiFi Access Point. The subnet mask defines the range of IP addresses that belong to the same network.
10. **WIFI_AP_BANDWIDTH**: This macro sets the bandwidth for the Access Point's WiFi channel. It specifies whether the channel uses a 20 MHz or 40 MHz bandwidth. Smaller bandwidth might provide better stability in crowded WiFi environments.
11. **WIFI_STA_POWER_SAVE**: This macro configures the power-saving mode for the WiFi station (client) mode. 
12. **MAX_SSID_LENGTH**: This macro defines the maximum length for the SSID (network name). It adheres to IEEE standards.
13. **MAX_PASSWORD_LENGTH**: This macro defines the maximum length for the WiFi password. It adheres to IEEE standards.
14. **MAX_CONNECTION_RETRIES**: This macro specifies the maximum number of retry attempts the WiFi station should make when it gets disconnected from the network before taking alternative actions.
15. **WIFI_APP_TASK_STACK_SIZE**: This macro defines the stack size in bytes for the WiFi application task.
16. **WIFI_APP_TASK_PRIORITY**: This macro sets the priority of the WiFi application task in relation to other tasks running on the system.
17. **WIFI_APP_TASK_CORE_ID**: The ESP32 has multiple cores, and this macro specifies which core the WiFi application task should run on.

### Functions
1. **wifi_app_event_handler**: This function handles various WiFi and IP events. It prints log messages based on the event type, providing insights into the WiFi state changes.
2. **wifi_app_event_handler_init**: Initializes the event loop for WiFi events and registers event handler instances for both WiFi and IP events.
3. **wifi_app_default_wifi_init**: Initializes the TCP/IP stack and WiFi configuration with default values. It also creates the default station and access point network interface (esp_netif_t objects).
4. **wifi_app_soft_ap_config**: Configures the WiFi access point settings, such as SSID, password, channel, authentication mode, and DHCP settings. It also sets the static IP information for the AP.
5. **wifi_app_task**: The main WiFi application task. It initializes the event handler, TCP/IP stack, WiFi configuration, and the AP. It waits for WiFi application messages from the queue and responds accordingly. For instance, it starts an HTTP server or handles WiFi connection events.
6. **wifi_app_send_message**: Sends a WiFi application message to the queue. It packages the message into a structure and sends it to the queue.
7. **wifi_app_start**: The function to start the WiFi application. It initializes NVS, sets up the WiFi application task, and prepares the ESP32 to start the WiFi functionality.
8. **app_main**: The entry point of the program. It initializes the Non-Volatile Storage (NVS) and starts the WiFi application by calling the wifi_app_start function.

**These functions and macros are fundamental in setting up and managing the WiFi Access Point on the ESP32 device. They handle event registration, configuration, task management, and 
communication through message queues.**

### Contributing
Contributions to this repository are welcome! If you would like to contribute, follow these steps:

1. Fork the repository to your GitHub account.
2. Create a new branch with a descriptive name for your contribution.
3. Make your desired changes or additions to the repository.
4. Commit and push your changes to your forked repository.
5. Open a pull request from your branch to the original repository's main branch.
6. Once your pull request is reviewed and accepted, your changes will be merged into the main repository.
