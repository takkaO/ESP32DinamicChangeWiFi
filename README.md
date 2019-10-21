# ESP32 dinamic change WiFi setting

### Description
ESP32が接続するWiFiアクセスポイントのSSIDとパスワードを動的に変更するサンプルコード．  
起動時にサーバモードとクライアントモードを選択し，サーバーモードでWiFiアクセスポイントのSSIDとパスワードをブラウザ経由で変更可能．(Tested ESP32-DevkitC)  
  
  
Sample code that dynamically changes the SSID and password of the WiFi access point to which ESP32 connects.  
The server mode and client mode can be selected at startup, and the SSID and password of the WiFi access point can be changed via the browser in server mode.(Tested ESP32-DevkitC)

### Environment
- ESP32-DevkitC
- Push button
	- Connect to IO22 pin and pull up

### How to enter server mode
1. Turn off the devide.
2. Turn on your device while holding down the ```Mode change button``` (IO22 pin).
3. Connect to the access point ```ESP32-AP``` using a smartphone or PC.
    Password is **```12345678```**.
<img src="https://i.imgur.com/EDKnExo.png" width="300px">
4. Open web browser, and access ```192.168.4.1``` and enter SSID and password and press submit button.
<img src="https://i.imgur.com/6WORb1w.png" width="300px">
5. If successful submitting, turn off the device and reboot while **NOT** holding down the ```Mode change button```.
