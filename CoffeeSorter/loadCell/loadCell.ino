#define DT A0
#define SCK A1
#define sw 9
 
long sample=0;
float val=0;
long count=0;
 
unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  pinMode(DT, OUTPUT);
  digitalWrite(DT,HIGH);
  digitalWrite(SCK,LOW);
  Count=0;
  pinMode(DT, INPUT);
  while(digitalRead(DT));
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT))
    Count++;
  }
  digitalWrite(SCK,HIGH);
  Count=Count^0x800000;
  digitalWrite(SCK,LOW);
  return(Count);
}
 
void setup()
{
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  calibrate();
  Serial.begin(9600);
}
 
void loop()
{
  count= readCount();
  int w=(((count-sample)/val)-2*((count-sample)/val));
  
  if(digitalRead(sw)==0)
  {
    val=0;
    sample=0;
    w=0;
    count=0;
    calibrate();
  }
}
 
void calibrate()
{
  Serial.println("start calibration");
  for(int i=0;i<100;i++)
  {
    count=readCount();
    sample+=count;
  }
  sample/=100;
  Serial.println("put 100g and wait");
  count=0;
  while(count<1000)
  {
    count=readCount();
    count=sample-count;
  }
  Serial.println("wait");
  delay(2000);
  for(int i=0;i<100;i++)
  {
    count=readCount();
    val+=sample-count;
  }
  val=val/100.0;
  val=val/100.0; // put here your calibrating weight
  Serial.println("done calibration");
}