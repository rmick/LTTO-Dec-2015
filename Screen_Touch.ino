//  char Get ButtonPress()
//  bool GetTouch()
//  void DrawButton (PosX, PosY, Width, Height, BoxColour(uint16_t), Text(string), TextSize, TextColour(uint16_t) )
//  void DrawScreen(char* Label, uint16_t BackColour, uint16_t TextColour)
//  void PrintButtonArray()      - for deBug only
//  int CountDigits(int num)

static int TouchX;
static int TouchY;

int Buttons [12] [4];
char* ButtonPressed [12];

///////////////////////////////////////////////////////////////////////////////
char* GetButtonPress()
{
  if (GetTouch(False))
  {
    delay (200);  //TODO: fix the debounce properly.

    // Match it to a button
    byte row = 0;
    for (row=0; row<12; row++) 
    {
      if (TouchX > Buttons [row] [0] && TouchX < Buttons [row] [2])
      {
        if (TouchY > Buttons [row] [1] && TouchY < Buttons [row] [3])
        {
          return ButtonPressed [row];
          if (deBug)
          {
            Serial.print(F("Button Pressed: "));
            Serial.println(ButtonPressed [row]);
          }
        }
      }
    }
  }  
}

//////////////////////////////////////////////////////////////////////////////

bool GetTouch(bool Touched)
{ 
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  TouchX = map(p.x, MinValTouchX, MaxValTouchX, 0, tft.width() );
  TouchY = map(p.y, MinValTouchY, MaxValTouchY, 0, tft.height());
  
  if (p.z >10) Touched = True;
  return Touched;
}

///////////////

#define MINPRESSURE 10
#define MAXPRESSURE 1000

TSPoint getPressPosition()
{
 
   TSPoint p, q;
   int upCount = 0;
 
   // Wait for screen press
   do
   {
      q = ts.getPoint();
      delay(10);
   } while( q.z < MINPRESSURE || q.z > MAXPRESSURE );
 
   // Save initial touch point
   p.x = q.x; p.y = q.y; p.z = q.z;
 
   // Wait for finger to come up
   do
   {
      q = ts.getPoint();
      if ( q.z < MINPRESSURE || q.z > MAXPRESSURE  )
      {
         upCount++;
      }
      else
      {
         upCount = 0;
         p.x = q.x; p.y = q.y; p.z = q.z;
      }
 
      delay(10);             // Try varying this delay
 
   } while( upCount < 10 );  // and this count for different results.
 
   TouchX = map(p.x, MinValTouchX, MaxValTouchX, 0, tft.width() );
   TouchY = map(p.y, MinValTouchY, MaxValTouchY, 0, tft.height());
 
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
 
   return p;

}

//////////////////////////////////////////////////////////////////////////////////////////

void DrawButton(uint16_t PosX, uint16_t PosY, uint16_t Width, uint16_t Height, uint16_t BoxColour,
                char* Text, byte TextSize, uint16_t TextColour) 
{
  // Draw the button
  tft.fillRoundRect(PosX, PosY, Width, Height, 10, BoxColour);
  
  // Set the text size & colour
  tft.setTextColor(TextColour);
  tft.setTextSize(TextSize);
  
  // Centre the text on the button
  uint16_t CtrY = PosY+(Height/2)-(7*TextSize/2);                     // characters are 7 pixels high
  uint16_t TextWidth = (strlen(Text)*5)+(strlen(Text)-1);           // characters are 5 pixels wide + 1 pixel space
  uint16_t CtrX = PosX+(Width/2) - ((TextWidth*TextSize)/2);  
  tft.setCursor(CtrX, CtrY);
  tft.println(Text);

  //  Create an Array that contains a list of all the buttons and their locations.
  if (ButtonCount == 0)          //Clear the array
  {
    byte Row;
    byte Column;
    for (Row=0; Row<12; Row++)
    {
      for (Column=0; Column<4; Column++)
      {
        Buttons [Row] [Column] = 0;
      }
    }
    for (Row=0; Row<12; Row++)
    {
      ButtonPressed [Row] = "";
    }
  }
  Buttons[ButtonCount] [0]= (PosX);
  Buttons[ButtonCount] [1]= (PosY);
  Buttons[ButtonCount] [2]= (PosX+Width);
  Buttons[ButtonCount] [3]= (PosY+Height);
  ButtonPressed [ButtonCount] = Text;
  ButtonCount++;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DrawScreen(char Title, char* Label, uint16_t BackColour, uint16_t TextColour, uint8_t TextSize)
{
  tft.fillScreen(BackColour);
  
  uint16_t TextWidth = (strlen(Label)*5)+(strlen(Label)-1);           // characters are 5 pixels wide + 1 pixel space
  uint16_t CtrX = (240/2) - ((TextWidth*TextSize)/2);
  tft.setCursor(CtrX, 5);
  tft.setTextColor(TextColour);
  tft.setTextSize(TextSize);
  tft.println(Label);
 
  ButtonCount = 0;
  lastState = state;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PrintButtonArray()
{
  //return;
  byte Row;
  byte Column;
  for (Row=0; Row<12; Row++)
  {
    for (Column=0; Column<4; Column++)
    {
      Serial.print (Buttons[Row] [Column]);
      Serial.print (F(", "));
    }
    Serial.println();
  }
  for (Row=0; Row<12; Row++)
  {
    Serial.print (ButtonPressed [Row]);
    Serial.print (F(", "));
  }
  Serial.println();
}

////////////////////////////////////////////////////////////////////////

int CountDigits(int num)
{
  int count=0;
  while(num)
  {
    num=num/10;
    count++;
  }
  return count;
}
