# Thermostat
Simple temperature reporting sketch for a Feather Huzzah using a dht22 sensor and published the values to an mqtt server. Also has wifi config built in and status page. 
This project can be used as a template for a lot of other projects that require wifi and mqtt connectivity. The main setup and loop handle calls to the other io files to keep everything organized.

Setup handles pin config, wifimanager setup, mqtt initial connection, and loading the web.

Loop handles resetting wifi on button press, verifying mqtt connection occasionally, handle web get/posts, and any other function that would be needed in the DeviceFunctions

DeviceFunctions - home for anything you would want to happen. This project handles checking the dht22 temp and publishes the message. You could add other functions to check here and call them in the loop

Mqtt - contains functions that handle connection to mqtt server

Web - contains functions to do different things for get/posts, and display of the main status page
