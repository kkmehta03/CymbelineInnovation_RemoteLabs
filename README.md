# Cymbeline Innovation RemoteLabs
Remote Labs Kit developed by Cymbeline Innovation, using Wemos D1 Mini ESP8266 Web Server &amp; arduino Web Sockets.
The kit is designed for 3 inputs and has 5 outputs.

#### Before trying any uploads or any changes to the code, please download and go through the [step-by-step PPT](https://github.com/KhyatiMehta3/CymbelineInnovation_RemoteLabs/blob/master/Step%20by%20Step%20Instructions.pptx) and ensure all libraries and device drivers are perfectly installed. Most possible errors occur because your libraries aren't included. 

### Inputs
|Inputs|Pins|
|------|----|
| a    | D3 |
| b    | D2 |
| c    | D1 |

### Outputs
| Gates | Pins |
|-------|------|
| NAND  | D0   |
| OR    | D5   |
| XOR   | D6   |
| AND   | D7   |
| FaS   | D8   |

### Procedure:
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

### Schematic
![alt text](https://github.com/KhyatiMehta3/CymbelineInnovation_RemoteLabs/blob/master/RemoteLab_Fritzing1.png "Schematic")

### The Remote Lab Kit
![alt text](https://github.com/KhyatiMehta3/CymbelineInnovation_RemoteLabs/blob/master/RemoteLab_Kit.jpeg "RemoteLab Kit")
---
### Remote Access from anywhere in the world
* To be able to access this kit from anywhere in the world, what has to be done is port forwarding.
* Go [here](https://www.ipchicken.com) to get your global ip address. Copy it.
* Access that ip address on your browser.
* When prompted for user name and password, type admin for both.
* Every router web page is different, so to configure this, go to something in the lines of "NAT" or "Virtual server".
* Paste the global IP in field that might have "Remote IP" and your local IP at the field called "Local IP".
* Enter your port number that you have configured in the code. It can be anything apart from default port numbers. For example, 5004.
* Apply changes.
* Type in your global ip followed by port number (Ex- 117.215.112.102:5004) on any browser connected to any network and control your remote lab completely remotely.
### In case you find issues or need help, please go to the Issues tab, and click "New Issue" button and write in your query & issue. Please write your query in detail along with screenshots to make it easy for us to understand the problem. We will get back to you with the solution.
