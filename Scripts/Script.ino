#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AccelStepper.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "Vory"; //WiFi!
const char* password = "Pazout2604"; //Password!!!

//Pinout
#define RING_LEDS 24 //leds
#define MOTOR_PIN1 19
#define MOTOR_PIN2 18
#define MOTOR_PIN3 5
#define MOTOR_PIN4 17
#define RING_PIN 27
#define SWITCH_PIN 32
#define TOUCH_PIN 4
#define BUZZ_PIN 25

AccelStepper motor(8, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Adafruit_NeoPixel ring(RING_LEDS, RING_PIN, NEO_GRB + NEO_KHZ800);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// stavy jak moje hlavy
bool globalPower = true;
bool motorPower = true;
bool ringPower = true;

int motorSpeed = 600;
int motorDir = 1;
bool isMoving = false;
bool demoMode = false;
bool auto360 = false;

int ringMode = 0;
int ringBright = 150;

// tlacitko
struct BtnLogic { int pin; int clicks; uint32_t last; bool lastS; bool lp; };
BtnLogic swB = {SWITCH_PIN, 0, 0, true, false};
BtnLogic tcB = {TOUCH_PIN, 0, 0, true, false};

void notify() {
  String j = "{";
  j += "\"gp\":" + String(globalPower) + ",\"mp\":" + String(motorPower) + ",\"rp\":" + String(ringPower) + ",";
  j += "\"s\":" + String(motorSpeed) + ",\"d\":" + String(motorDir) + ",\"rm\":" + String(ringMode) + ",";
  j += "\"mv\":" + String(isMoving) + ",\"dm\":" + String(demoMode) + ",\"rb\":" + String(ringBright);
  j += "}";
  ws.textAll(j);
}

// logika
void handlePCB(int c, bool lp) {
  if (lp) { demoMode = !demoMode; if(demoMode){globalPower=true; motorPower=true; isMoving=true;} }
  else {
    if(c==1) { isMoving = !isMoving; motorPower = true; }
    if(c==2) { motorSpeed = (motorSpeed >= 1000)?200:motorSpeed+200; }
    if(c==3) { isMoving=false; motorPower=false; demoMode=false; auto360=false; }
    if(c==4) { auto360=true; isMoving=true; motorPower=true; motor.setCurrentPosition(0); }
  }
  notify(); tone(BUZZ_PIN, 1000, 50);
}

void handleTouch(int c, bool lp) {
  if (lp) { globalPower = !globalPower; }
  else {
    if(c==1) { ringMode = (ringMode+1)%15; ringPower=true; }
    if(c==2) { /* Volné po odebrání pásku */ }
    if(c==3) { ringBright = (ringBright>=250)?50:ringBright+50; }
  }
  notify(); tone(BUZZ_PIN, 1200, 30);
}

// web (asi bily)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>
<title>PLATFORM PRO</title>
<style>
  :root { --bg: #F5F5F7; --card: #FFFFFF; --acc: #007AFF; --err: #FF3B30; --txt: #1D1D1F; }
  body { background: var(--bg); color: var(--txt); font-family: -apple-system, sans-serif; margin: 0; padding: 20px; display: flex; justify-content: center; }
  .container { width: 100%; max-width: 480px; }
  .card { background: var(--card); border-radius: 18px; padding: 25px; margin-bottom: 20px; box-shadow: 0 8px 30px rgba(0,0,0,0.04); border: 1px solid rgba(0,0,0,0.02); }
  h2 { font-size: 12px; color: #86868B; text-transform: uppercase; margin-bottom: 15px; letter-spacing: 0.5px; }
  
  /* Slicer-style Joystick */
  .joy-wrapper { position: relative; width: 240px; height: 240px; margin: 0 auto 25px; background: #F2F2F7; border-radius: 50%; display: flex; align-items: center; justify-content: center; }
  .joy-btn { position: absolute; width: 75px; height: 75px; border: none; background: white; color: var(--txt); border-radius: 50%; cursor: pointer; font-weight: 700; box-shadow: 0 4px 12px rgba(0,0,0,0.08); transition: 0.2s; }
  .joy-btn:active { transform: scale(0.92); background: var(--acc); color: white; }
  .joy-left { left: 15px; }
  .joy-right { right: 15px; }
  .joy-stop { position: relative; width: 85px; height: 85px; background: var(--err); color: white; z-index: 5; font-size: 15px; box-shadow: 0 6px 20px rgba(255,59,48,0.25); }
  
  .btn { width: 100%; padding: 14px; border-radius: 12px; border: none; background: #E5E5EA; color: var(--txt); font-weight: 600; cursor: pointer; transition: 0.3s; margin-bottom: 10px; }
  .btn.active { background: var(--acc); color: white; }
  .btn-power { background: #34C759; color: white; }
  .btn-power.off { background: var(--err); }
  
  select { width: 100%; padding: 12px; border-radius: 10px; border: 1px solid #D1D1D6; background: white; font-size: 15px; outline: none; }
  input[type=range] { width: 100%; accent-color: var(--acc); margin-top: 15px; height: 6px; border-radius: 5px; }
  .stat { font-size: 13px; font-weight: 500; color: #86868B; margin-top: 10px; display: block; }
</style></head>
<body><div class='container'>
  <h1 style='text-align:center; font-weight:700; margin-bottom:30px;'>Rotating Platform</h1>
  
  <div class='card'>
    <h2>System</h2>
    <button id='gp' class='btn btn-power' onclick="send('gp:1')">SYSTEM POWER</button>
  </div>

  <div class='card'>
    <h2>Movement Control</h2>
    <div class='joy-wrapper'>
      <button class='joy-btn joy-left' onclick="send('dir:-1')">LEFT</button>
      <button class='joy-btn joy-stop' onclick="send('mv:0')">STOP</button>
      <button class='joy-btn joy-right' onclick="send('dir:1')">RIGHT</button>
    </div>
    <button id='mp' class='btn' onclick="send('mp:1')">Enable Motor</button>
    <button id='dm' class='btn' onclick="send('dm:1')">Demo Mode</button>
    <span class='stat'>Current Speed: <b id='v-spd' style='color:var(--acc)'>-</b></span>
    <input type='range' min='100' max='1000' id='r-spd' oninput="send('spd:'+this.value)">
  </div>

  <div class='card'>
    <h2>Lighting Effects</h2>
    <button id='rp' class='btn' onclick="send('rp:1')">Enable Ring</button>
    <select id='s-rm' onchange="send('rm:'+this.value)"></select>
    <span class='stat'>Ring Brightness</span>
    <input type='range' min='0' max='255' id='r-rb' oninput="send('rb:'+this.value)">
  </div>
</div>
<script>
  let ws = new WebSocket('ws://'+location.hostname+'/ws');
  const sel = document.getElementById('s-rm');
  for(let i=0; i<15; i++) { let o = document.createElement('option'); o.value=i; o.text='Effect Mode '+(i+1); sel.add(o); }
  
  ws.onmessage = e => {
    let d = JSON.parse(e.data);
    document.getElementById('gp').className = d.gp ? 'btn btn-power' : 'btn btn-power off';
    document.getElementById('mp').className = d.mp ? 'btn active' : 'btn';
    document.getElementById('rp').className = d.rp ? 'btn active' : 'btn';
    document.getElementById('dm').className = d.dm ? 'btn active' : 'btn';
    document.getElementById('r-spd').value = d.s; document.getElementById('v-spd').innerText = d.s;
    document.getElementById('s-rm').value = d.rm; document.getElementById('r-rb').value = d.rb;
  };
  function send(m) { ws.send(m); }
</script></body></html>)rawliteral";

void handleCmd(String m) {
  if (m.startsWith("gp:")) globalPower = !globalPower;
  if (m.startsWith("mp:")) motorPower = !motorPower;
  if (m.startsWith("rp:")) ringPower = !ringPower;
  if (m.startsWith("dir:")) { motorDir = m.substring(4).toInt(); isMoving = true; motorPower = true; demoMode = false; }
  if (m.startsWith("mv:")) { isMoving = false; demoMode = false; auto360 = false; }
  if (m.startsWith("spd:")) motorSpeed = m.substring(4).toInt();
  if (m.startsWith("rm:")) { ringMode = m.substring(3).toInt(); ringPower = true; }
  if (m.startsWith("rb:")) ringBright = m.substring(3).toInt();
  if (m.startsWith("dm:")) { demoMode = !demoMode; if(demoMode) {motorPower=true; isMoving=true;} }
  notify();
}

void setup() {
  Serial.begin(115200);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ring.begin();
  motor.setMaxSpeed(2000);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(TOUCH_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) delay(500);
  
  ws.onEvent([](AsyncWebSocket* s, AsyncWebSocketClient* c, AwsEventType t, void* arg, uint8_t* d, size_t l){
    if(t == WS_EVT_DATA) { String m=""; for(size_t i=0; i<l; i++) m+=(char)d[i]; handleCmd(m); }
  });
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){ r->send_P(200, "text/html", index_html); });
  server.begin();
}

void loop() {
  ws.cleanupClients();
  auto checkB = [](BtnLogic &b, void (*h)(int, bool)) {
    bool s = digitalRead(b.pin);
    uint32_t now = millis();
    if (s == LOW && b.lastS == HIGH) { b.last = now; b.lastS = LOW; b.lp = false; }
    else if (s == LOW && !b.lp && (now - b.last > 3000)) { h(0, true); b.lp = true; }
    else if (s == HIGH && b.lastS == LOW) { if (!b.lp) b.clicks++; b.last = now; b.lastS = HIGH; }
    if (b.clicks > 0 && (now - b.last > 400) && s == HIGH) { h(b.clicks, false); b.clicks = 0; }
  };
  checkB(swB, handlePCB); checkB(tcB, handleTouch);

  if (globalPower) {
    if (demoMode) {
      if (abs(motor.distanceToGo()) < 5) { motorDir *= -1; motor.move(4096 * motorDir); ringMode = (ringMode + 1) % 15; notify(); }
      motor.setSpeed(600 * motorDir); motor.runSpeedToPosition();
    } else if (auto360) {
      if (abs(motor.distanceToGo()) < 5) { motorDir *= -1; motor.move(4096 * motorDir); notify(); }
      motor.setSpeed(motorSpeed * motorDir); motor.runSpeedToPosition();
    } else if (isMoving && motorPower) {
      motor.setSpeed(motorSpeed * motorDir); motor.runSpeed();
    }
    
    if (ringPower) {
      ring.setBrightness(ringBright);
      uint32_t t = millis();
      for(int i=0; i<RING_LEDS; i++) {
        switch(ringMode) {
          case 0: ring.setPixelColor(i, 255,255,255); break;
          case 1: ring.setPixelColor(i, ring.ColorHSV(t*5 + i*1500)); break;
          case 2: ring.setPixelColor(i, ring.ColorHSV(t*2, 255, 255)); break;
          default: ring.setPixelColor(i, ring.ColorHSV(ringMode*4000 + i*500)); break;
        }
      }
      ring.show();
    } else { ring.clear(); ring.show(); }
    
  } else { motor.stop(); ring.clear(); ring.show(); }

  static uint32_t oT = 0;
  if (millis() - oT > 250) {
    oT = millis(); oled.clearDisplay(); oled.setTextColor(1);
    oled.setCursor(0,0); oled.print(WiFi.localIP());
    oled.setCursor(0,20); oled.print(isMoving ? "MOVING" : "IDLE");
    oled.setCursor(0,40); oled.print("SPEED: "); oled.print(motorSpeed);
    oled.display();
  }
}