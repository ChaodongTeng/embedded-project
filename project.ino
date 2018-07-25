int Light=54;
int DHpin=8;
byte dat [5];
int pump=7;
int led=13;
void setup()
{
Serial.begin(250000);
Serial1.begin(250000);
pinMode(Light,INPUT);
pinMode(DHpin,OUTPUT);
analogReadResolution(10);
pinMode(pump,OUTPUT);
pinMode(led,OUTPUT);
  }
 byte read_data(){
      byte data;
     for (int i = 0; i < 8; i ++)
     { 
      if (digitalRead (DHpin) == LOW)
      {while (digitalRead (DHpin) == LOW); // wait for 50us       
     delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'       
     if (digitalRead (DHpin) == HIGH)         
     data |= (1 << (7-i)); // high front and low in the post       
     while (digitalRead (DHpin) == HIGH); 
     //{;}// data '1 ', wait for the next one receiver      
     }   
     } 
     return data; 
     }   
     void start_test () 
     {   
      digitalWrite (DHpin, LOW); // bus down, send start signal   
     delay (30); // delay greater than 18ms, so DHT11 start signal can be detected     
     
     digitalWrite (DHpin, HIGH);   
     delayMicroseconds (40); // Wait for DHT11 response     
     
     pinMode (DHpin, INPUT);   
     while (digitalRead (DHpin) == HIGH);   
     delayMicroseconds (80); // DHT11 response, pulled the bus 80us   
     if (digitalRead (DHpin) == LOW);   
     delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data     
     
     for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered     
     dat[i] = read_data ();     
     
     pinMode (DHpin, OUTPUT);   
     digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal 
     }
  void loop()
{
if(Serial1.available())
{
  delay(1000);
  if(Serial1.read()==1)
  {Serial.println("It's rainy,we don't need to water");
  delay(1000);}
  if(Serial1.read()==0)
  {
    Serial.println("Not rainy ,let's think think about the temperature and humidity!");
  delay(1000);
  int light=analogRead(Light);
  Serial.print("lumen is:");
  Serial.println(light);
  delay(1000);
   start_test();
   Serial.print ("Current humidity =");  
   Serial.print (dat [0], DEC); 
   // display the humidity-bit integer;  
   Serial.print ('.');   
   Serial.print (dat [1], DEC); // display the humidity decimal places;  
   Serial.println ('%');   
   Serial.print ("Current temperature =");   
   Serial.print (dat [2], DEC); // display the temperature of integer bits;   
   Serial.print ('.');   
   Serial.print (dat [3], DEC); // display the temperature of decimal places;   
   Serial.println ('C'); 
  delay(1000);
  if(light<200)
  {
    Serial.print("It's night!");
    digitalWrite(led,HIGH);
   delay(10000);
   digitalWrite(led,LOW); 
   
    }
  if(dat[0]>80)
  {
    Serial.print("humidity is:");
    Serial.print(dat[0]);
    Serial.println("humidity is fine,we do not need to water it.");
    }
  if(dat[0]<80)//realtively dry
    {
      if(dat[2]<=0)//what if it's too cold
      {
        Serial.println("It's frezzing cold,do not water now");
        }
      if(dat[2]>0&&dat[2]<=30)//t's dry and not too hot
      {
        Serial.print("watering!");
        digitalWrite(pump,HIGH);
        delay(500);
        digitalWrite(pump,LOW);
        delay(500);
        }
      if(dat[2]>30)//relatively hot
      {
        if(light>=800)//may be it is high noon
        {
          Serial.println("may be it is high noon,it is unappropriate to water the plant");
          }
        if(light<800)
        {
          Serial.print("watering");
          digitalWrite(pump,HIGH);
          delay(500);
          digitalWrite(pump,LOW);
          delay(500);
          }
        }
      }
  }
  delay(10000);
}
if(!Serial1.available())
{
  Serial.println("no weather information,let's wait a minute");
  delay(10000);
  }

    }
  

