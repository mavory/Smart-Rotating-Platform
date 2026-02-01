# Smart-Rotating-Platform
SRP is a smart system controlled by ESP32. It can control a platform that is illuminated by a neopixel ring, all using buttons and the web!

![image](https://blueprint.hackclub.com/user-attachments/representations/redirect/eyJfcmFpbHMiOnsiZGF0YSI6ODc3NzEsInB1ciI6ImJsb2JfaWQifX0=--a6429f615a83e3682c9f3a7bb1c8b023b79d0ebd/eyJfcmFpbHMiOnsiZGF0YSI6eyJmb3JtYXQiOiJqcGciLCJyZXNpemVfdG9fbGltaXQiOlsyMDAwLDIwMDBdLCJjb252ZXJ0Ijoid2VicCIsInNhdmVyIjp7InF1YWxpdHkiOjgwLCJzdHJpcCI6dHJ1ZX19LCJwdXIiOiJ2YXJpYXRpb24ifX0=--15ca3815f01a5683e19ea0585d2eef9af9e441d7/20260121_175356.jpg)

## Why did I do this?

I wanted to create a project where I could put a thing on a platform and plug it in, then it would spin. So I thought about it and created this!

## Features

- Web interface
- It has a buzzer for notification
- Platform rotation with speed adjustment
- Neopixel ring for lighting

## Scripts

The script has approximately 230 lines, at the top you can add WiFi and password, then the system will be connected to the internet! Just upload it to the ESP32 and everything should work.

### Home page:
![image](https://blueprint.hackclub.com/user-attachments/representations/redirect/eyJfcmFpbHMiOnsiZGF0YSI6ODc3OTEsInB1ciI6ImJsb2JfaWQifX0=--5298e85cea60655e9c43f7eb1735860cefa8ac91/eyJfcmFpbHMiOnsiZGF0YSI6eyJmb3JtYXQiOiJwbmciLCJyZXNpemVfdG9fbGltaXQiOlsyMDAwLDIwMDBdLCJjb252ZXJ0Ijoid2VicCIsInNhdmVyIjp7InF1YWxpdHkiOjgwLCJzdHJpcCI6dHJ1ZX19LCJwdXIiOiJ2YXJpYXRpb24ifX0=--0f85faa91c373105a0f317054e965c1f47e93a37/image.png)

## How it works?

- If you turn on the system and if everything is written correctly, the ESP32 will connect to the internet and you can control everything via the web!
- On the OLED display you can see the speed of the motor and whether it is spinning. 
- You can also see the IP address to which the system is connected.
- You can control various things via the buttons and you can also map them to something else in the code yourself!

### Libraries:

```
WiFi.h
ESPAsyncWebServer.h
AccelStepper.h
Adafruit_SSD1306.h
Adafruit_NeoPixel.h
```


## 3D models

Here is a view of the top and bottom of the case:

![image](https://github.com/mavory/Smart-Rotating-Platform/blob/main/Photos/Sn%C3%ADmek%20obrazovky%202026-01-25%20113950.png?raw=true)

![image](https://github.com/mavory/Smart-Rotating-Platform/blob/main/Photos/Sn%C3%ADmek%20obrazovky%202026-01-25%20113855.png?raw=true)

## Wiring Diagram

![image](https://blueprint.hackclub.com/user-attachments/representations/redirect/eyJfcmFpbHMiOnsiZGF0YSI6OTAyOTQsInB1ciI6ImJsb2JfaWQifX0=--a5f09185a44380b633d99bc31173a792134c591b/eyJfcmFpbHMiOnsiZGF0YSI6eyJmb3JtYXQiOiJwbmciLCJyZXNpemVfdG9fbGltaXQiOlsyMDAwLDIwMDBdLCJjb252ZXJ0Ijoid2VicCIsInNhdmVyIjp7InF1YWxpdHkiOjgwLCJzdHJpcCI6dHJ1ZX19LCJwdXIiOiJ2YXJpYXRpb24ifX0=--0f85faa91c373105a0f317054e965c1f47e93a37/image.png)

![image](https://blueprint.hackclub.com/user-attachments/representations/redirect/eyJfcmFpbHMiOnsiZGF0YSI6OTY2NjgsInB1ciI6ImJsb2JfaWQifX0=--9c29b90eaf6372a04a33f62b600f6c296ba7bead/eyJfcmFpbHMiOnsiZGF0YSI6eyJmb3JtYXQiOiJwbmciLCJyZXNpemVfdG9fbGltaXQiOlsyMDAwLDIwMDBdLCJjb252ZXJ0Ijoid2VicCIsInNhdmVyIjp7InF1YWxpdHkiOjgwLCJzdHJpcCI6dHJ1ZX19LCJwdXIiOiJ2YXJpYXRpb24ifX0=--0f85faa91c373105a0f317054e965c1f47e93a37/circuit_image%20(1).png)

## Bill of Materials (BOM)

| Item            | Quantity | Link |
|-----------------|----------|------|
| ESP32           | 1        | [AliExpress](https://www.aliexpress.com/item/1005009694232832.html) |
| Breadboards     | 2        | [AliExpress](https://www.aliexpress.com/item/1005007493250982.html) |
| Touch sensor    | 1        | [AliExpress](https://www.aliexpress.com/item/1005008974368078.html) |
| Buzzer          | 1        | [AliExpress](https://www.aliexpress.com/item/1005006260328559.html) |
| Jumpers         | Much     | [AliExpress](https://www.aliexpress.com/item/1005002611994443.html) |
| Button          | 1        | [AliExpress](https://www.aliexpress.com/item/1005007197228321.html) |
| Neopixel ring   | 1        | [AliExpress](https://www.aliexpress.com/item/1005008125328620.html) |
| OLED display    | 1        | [AliExpress](https://www.aliexpress.com/item/32896971385.html) |
| Stepper motor   | 1        | [AliExpress](https://www.aliexpress.com/item/1005007004416294.html) |

## There is the final video of how it works!

_Here is a youtube video where you can see how it works:_ [Link](https://youtu.be/nypfZz_uRD4)

