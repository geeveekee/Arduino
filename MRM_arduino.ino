class LED
{
  private:
  	byte pin;
  	
  public:
  	LED(byte pin_val)
    {
      pin = pin_val;
      init();
    }
  
  	void init()
    {
      pinMode(pin, OUTPUT);
      off();
    }
  	
  	void off()
    {
      digitalWrite(pin, LOW);
    }
  
  	void on()
    {
      digitalWrite(pin, HIGH);
    }
};

class Button
{
  private:
  	byte pin;
  	byte state;
  	byte lastReading;
  	unsigned long lastDebounceTime =0;
  	unsigned long debounceDelay =50;
  	int prevReading = 0;
    int currentReading;
    int ledState =0;
  	int flag;
  	
  public:
  	Button(byte pin_val)
    {
      pin = pin_val;
      lastReading =LOW;
      init();
    }
  	
  	void init()
    {
      pinMode(pin, INPUT);
      update();
    }
  	
  	void update()
    {
      byte newReading = digitalRead(pin);
      
      if(newReading != lastReading)
      {
        lastDebounceTime = millis();
      }
      
      if((millis() - lastDebounceTime) > debounceDelay)
      {
        state = newReading;
      }
      
      lastReading = newReading;
    }
  
  	byte getState()
    {
      update();
      //Serial.println(state);
      return state;
    }
  
  	bool isPressed()
    {
      
      return (getState() == HIGH);
    }
  
  	int isToggled()
    {
      currentReading = getState();
      if(currentReading == 0 && prevReading ==1)
      {
        if(flag ==0)
        {
        	flag =1;
        }
        else
        {
          flag =0;
        }
      }
      
      prevReading = currentReading;
      return flag;
    }	
};

LED leds[4] = {13, 12, 11, 10};
Button btn[4] ={9, 8, 7, 6};
Button btn2[4] = {5, 4, 3, 2};
int pot = A0;
int potReading;
void setup()
{
  pinMode(pot, INPUT);
  Serial.begin(9600);
}
int i;
void allOn()
{
  for(i=0; i<4; i++)
    {
      leds[i].on();
    }
}

void allOff()
{
  for(i=0; i<4; i++)
    {
      leds[i].off();
    }
}
int val;
void toggleButtons()
{
  for(i=0; i<4; i++)
  {
    val =btn[i].isToggled();
    //Serial.println(val);
    if(val ==1)
    {
      leds[i].on();
    }
    else
    {
      leds[i].off();
    }
  }
  
  if(btn2[0].isToggled())
  {
    allOn();
  }
  
  if(btn2[1].isToggled())
  {
    allOff();
  }
}
int frequency;
int rN;
int rO = 0;
int end = 0;
void pulsateLed()
{   
    potReading = analogRead(pot);
    frequency = (1000./1023.)*potReading;
    for(i=0; i<4; i++)
    {
      leds[i].on();
    }
    delay(frequency);
    for(i=0; i<4; i++)
    {
      leds[i].off();
    }
    delay(frequency);
  
  
  Serial.print(btn2[2].isPressed());
}

int stop =0;
int stateO =0;
int stateN;
Button rec[10];
int j;
void memMode()
{
  while(!stop)
  {
    Serial.println("still here");
    
    stateN = btn2[1].isToggled();
    while(!stateN)
    {
      for(i=0; i<4; i++)
      {
      	if(btn[i].isToggled())
        {
          q.push(btn[i]);
        }
      }

        if(stateN)
        {
          Serial.println("yio");
          stop =1;
          break;
        }
    }
  }
  
 
}
int counter=0;
int valOld =0;
int valNew;
void loop()
{
  valNew =btn2[2].isToggled();
  if((valNew == 1 && valOld ==0) || (valNew ==0 && valOld ==1))
  {
    counter++; 
  }
  valOld = valNew;
  Serial.println(counter);
  
  if(counter ==1)
  {
    toggleButtons();
  }
  
  if(counter ==2)
  {
    pulsateLed(); 
  }
  
  if(counter ==3)
  {
    memMode();
  }
  
  
  delay(10);
}