#define led 12
#define buzzer 11
#define button 2
int bState = 0;
char data='0';
int x=0;
int etat=0;
char CIN[9]={'0','0','0','0','0','0','0','1'};

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  pinMode(button,INPUT);
  Serial.begin(9600);
  
}

void loop() {
  if(Serial.available()){
    data=Serial.read();
  }
    
  bState=digitalRead(button);
  
    //
    
  if(bState==HIGH || data =='1'){
    digitalWrite(led,HIGH);
    tone(buzzer,450);
    delay(200);
    noTone(buzzer);
    delay(200);
    data ='1';
    if(x==0){
    Serial.print('2');
    x=1;
    }
  }
  else if(data=='0'){
    digitalWrite(led,LOW);
    noTone(buzzer);
    if(x==1)
      x=0;
  }
}
