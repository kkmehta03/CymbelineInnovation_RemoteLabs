# Cymbeline Innovation RemoteLabs
Remote Labs Kit developed by Cymbeline Innovation, using Wemos D1 Mini ESP8266 Web Server &amp; arduino Web Sockets.

## Procedure:
* Rig up the circuit as per the schematic below.
* Copy the code in [this link](https://github.com/KhyatiMehta3/CymbelineInnovation_RemoteLabs/blob/master/LabVIEW_RemoteLabs_Cymbeline.ino)
* Modify these lines of the code 
```javascript
static const char ssid[] = "......";
static const char password[] = "........";
```
as per your Wifi SSID and its password, for the D1 Mini to successfully connect to a hotspot.
* Fire up Arduino IDE and verify the code after modifications.
* On successful compilation, upload the sketch onto the Wemos D1 Mini board.
* Connect your device to the same Wifi network, open up your browser and type the URL http://192.168.1.207.
* Use the remote lab through the web page.

## Schematic
![alt text](https://github.com/KhyatiMehta3/CymbelineInnovation_RemoteLabs/blob/master/RemoteLab_Fritzing.png "Schematic")


