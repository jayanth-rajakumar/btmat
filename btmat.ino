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

 Serial.begin(115200);
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
  
 delayMicroseconds(416);
  
            
            for(int lx=0;lx<30;lx++)
            {
              arr[lx]=digitalRead(serial_in);    
              delayMicroseconds(810);
            }
            
            arr[30]=digitalRead(serial_in);           
            delayMicroseconds(820);
            
            for(int lx=31;lx<90;lx++)
            {
              arr[lx]=digitalRead(serial_in);    
              delayMicroseconds(810);
            }
            
            
            
  pulse=1;packet="";current_time="";
  for(int k=0;k<90;k++)
  {
    if(arr[k]==0){
      packet+="1";
    }
    else
    packet+="0";
    }
    
 for(int k=1;k<=5;k++)
 getbytes(packet.substring(k*10+1,(k+1)*10-1));

 current_time=current_time.substring(0,1)+':'+current_time.substring(1,3)+'.'+current_time.substring(3,5);
  
  char charbuf[10];
  current_time.toCharArray(charbuf,10);

  if(isDigit(charbuf[0]) and isDigit(charbuf[6]) and isDigit(charbuf[2]) and isDigit(charbuf[3]) and isDigit(charbuf[5]))
  {
   S.println(current_time);
 
  }
  else
  {
   //Hack to resolve last seconds on QJ Timer. TODO: Improve timing of digitalRead to fix this

   charbuf[5]=qj_fix(charbuf[5]);
   charbuf[6]=qj_fix(charbuf[6]);
    current_time=String(charbuf);
    
        if(isDigit(charbuf[0]) and isDigit(charbuf[6]) and isDigit(charbuf[2]) and isDigit(charbuf[3]) and isDigit(charbuf[5]))
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



char qj_fix(char ch)
{
  switch(ch)
  {
    case '`':
    return '0';
    break;

    case 'b':
    return '1';
    break;

    case 'd':
    return '2';
    break;

    case 'f':
    return '3';
    break;

    case 'h':
    return '4';
    break;

    case 'j':
    return '5';
    break;

    case 'l':
    return '6';
    break;

    case 'n':
    return '7';
    break;

    case 'p':
    return '8';
    break;

    case 'r':
    return '9';
    break;

    
  }
}


