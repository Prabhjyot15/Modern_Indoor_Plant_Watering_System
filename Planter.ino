int blue =0;  // for incoming serial data
int moisture_sensor  = A0;//int moisture;
int sensor_analog;
int limit = 30.00;
int pump = 3;
int led = 13;
int pump_time = 3000;
String apiKey = "BQDCWOY1BTR2DSEK";  //Change this key to your "Write API key"
void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  Serial1.begin(115200);   // Arduino to ESP01 Communication
  connectWiFi();//Connecting to the wifi
  pinMode(moisture_sensor,INPUT);
  pinMode(pump,OUTPUT);
  pinMode(led,OUTPUT);
  //Serial.begin(38400); // Default communication rate of the Bluetooth module
}
void loop() {
  float moisture;
  sensor_analog = analogRead(moisture_sensor);
  moisture = (100 - ((sensor_analog/1023.00)*100));
  Serial.print("Moisture=");
  Serial.print(moisture);
  Serial.println();
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    blue = Serial.read(); // Reads the data from the serial port
  if (blue == '0') {
  Pump(0);
  blue = 0;
 }
 else if (blue == '1') {
 Pump(1);
 blue = 0;} }
 Serial1.println("AT+CIPMUX=0\r\n");      // To Set MUX = 0
  delay(1000);                             // Wait for 1 sec
 // TCP connection 
  String cmd = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n\r\n";   // TCP connection with https://thingspeak.com server
  Serial1.println(cmd);                    // Display above Command on PC
  Serial.println(cmd);                     // Send above command to Rx1, Tx1
  delay(1000);                            // Wait for 1 Sec
  if(Serial1.find("ERROR"))                // If returns error in TCP connection
  { 
    Serial.println("AT+CIPSTART error");   // Display error msg to PC 
  }  // prepare GET string 
  String getStr = "GET /update?api_key="+ apiKey +"&field1="+moisture+"\r\n\r\n"; // Add apiKey and Sensor Value
  Serial.println(getStr);                 // Display GET String on PC
  cmd = "AT+CIPSEND="+String(getStr.length())+"\r\n";                    // send data length 
  Serial.println(cmd);                   // Display Data length on PC
  Serial1.println(cmd);                  // Send Data length command to Tx1, Rx1
  delay(3000);                           // wait for 3 sec
  if(Serial1.find(">"))                  // If prompt opens //verify connection with cloud
  {
    Serial.println("connected to Cloud");  // Display confirmation msg to PC
    Serial1.print(getStr);                 // Send GET String to Rx1, Tx1
  }
  else
  { 
    Serial1.println("AT+CIPCLOSE\r\n");    // Send Close Connection command to Rx1, Tx1
    Serial.println("AT+CIPCLOSE");         // Display Connection closed command on PC
  } 
  // thingspeak free version needs 16 sec delay between updates 
  delay(16000); 
}
void Pump(int stat)
{
  if(stat == 0)
  {
    digitalWrite(pump,stat);
    digitalWrite(led,stat);
    Serial.println("Pump OFF");
  }
  if(stat == 1)
  {
    digitalWrite(pump,stat);
    digitalWrite(led,stat);
    Serial.println("Pump ON");  }}

boolean connectWiFi() {               // Connect to Wifi Function
  Serial1.println("AT+CWMODE=1\r\n"); // Setting Mode = 1 
  delay(1000);                         // wait for 100 mSec
  
  String cmd = "AT+CWJAP=\"riya\",\"blahblah\"\r\n";         // Connect to WiFi
  Serial.println(cmd);                // Display Connect Wifi Command on PC
  Serial1.println(cmd);               // send Connect WiFi command to Rx1, Tx1 
  delay(5000);                       // wait for 10 sec
  Serial1.println("AT+CWJAP?");       // Verify Connected WiFi
  if(Serial1.find("+CWJAP"))        
  {
    Serial.println("OK, Connected to WiFi.");         // Display Confirmation msg on PC
    return true;
  }else
  {
    Serial.println("Can not connect to the WiFi.");   // Display Error msg on PC
    return false;
  }
}
