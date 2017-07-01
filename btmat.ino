#include<SoftwareSerial.h>

int serial_in = 2,pulse;
long temp;
bool begin_=1;
volatile long duration=0,m1,m2;
volatile int i=0,arr[90],toggle=0;
String packet="",current_time="",packet_="";

void getbytes(String strbyte);
int twopwr(int);
SoftwareSerial S(10,11);


void setup()
{
 
  S.begin(9600);
  pinMode(serial_in, INPUT);
  digitalWrite(serial_in,HIGH);

     while(1)
   
  {
    temp=pulseIn(serial_in, LOW);
    if(temp>56000 and temp<66000)
    break;
  }
  
}

void loop() {

  i=0,toggle=0;
  for(int k=0;k<90;k++)
  arr[k]=0;
  long test_=millis();
  
  attachInterrupt(digitalPinToInterrupt(serial_in), isr, CHANGE);
  while(toggle==0)
  {
    if(millis()-test_>90)
    {
      S.println("-:--.--");
    }
  }
  detachInterrupt(digitalPinToInterrupt(serial_in));
 
  pulse=1;packet="";current_time="";
  for(int k=1;arr[k]!=0;k++)
  {
    for(int j=0;j<round((float)arr[k]/828);j++)
    {
      packet+=String(!pulse);
    }
    pulse=!pulse;
  }
    
 for(int k=1;k<=5;k++)
 getbytes(packet.substring(k*10+1,(k+1)*10-1));

 current_time=current_time.substring(0,1)+':'+current_time.substring(1,3)+'.'+current_time.substring(3,5);
  
  char charbuf[10];
  current_time.toCharArray(charbuf,10);

  if(isDigit(charbuf[0]) and isDigit(charbuf[6]))
  {
    S.println(current_time);
  }
  else
  {
   //Temporary hack to fill in skipped bits on G4 Pro timer. TODO: Implement packet detection alg from scratch using staggered sampling and remove this section.
   
    packet_=packet.substring(0,18) + "0" + packet.substring(18,38) + "1" + packet.substring(38,48)+"0"+ packet.substring(48);

    current_time="";
   
    for(int k=1;k<=5;k++)
    getbytes(packet_.substring(k*10+1,(k+1)*10-1));

    current_time=current_time.substring(0,1)+':'+current_time.substring(1,3)+'.'+current_time.substring(3,5);
    
    char charbuf2[10];  
    current_time.toCharArray(charbuf2,10);
   
    if(charbuf2[6]=='`')
      charbuf2[6]='0';
   
    if(isDigit(charbuf2[0]) and isDigit(charbuf2[6]))
    {
       S.println(current_time);
    }
   
  }

  
  long test=millis();
  while(digitalRead(serial_in)==0)
  {
    if(millis()-test>90)
    {
      Serial.println("-:--.--");
    }
  }
  
}

void isr()
{
  m1=micros();

 if(m1-m2>58000 and begin_==true)
  {
    Serial.println("Start");begin_=false;
    
  }
  else
  {
     arr[i]=m1-m2;
    if(i>5 and round((float)arr[i]/750)==4 and round((float)arr[i-1]/750)==2 and round((float)arr[i-2]/750)==1 and round((float)arr[i-3]/750)==1 and round((float)arr[i-4]/750)==1 and round((float)arr[i-5]/750)==1)
   {
    
   // Serial.println();
    m2=micros();
    toggle=1;
    return;
   }
   
   // Serial.print(String(arr[i])+"-");
    i++;
  }
  m2=micros();

}

void getbytes(String strbyte)
{

  int digit=0;
  for(int k=0;k<8;k++)
  {
   if(strbyte[k]=='1')
   {
    digit+=twopwr(k);
      
   }
  }

  char c=digit;
 current_time+=String(c);
  
}

//Returns 2 raised to the specified integer. Required because pow() from math.h has rounding errors.
int twopwr(int a)
{
  int out=1;
  for(int l=0;l<a;l++)
  out=out*2;

  return out;
}


