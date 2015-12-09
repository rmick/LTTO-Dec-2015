//  TaggerMode
//  GetButtonPress
//  DrawGunScreen
//  DrawButton (PosX, PosY, Width, Height, BoxColour(uint16_t), Text(string), TextSize, TextColour(uint16_t) )


int Buttons [12] [4];
char ButtonPressed [12];


///////////////////////////////////////////////////////////////////////////////
static int TouchX;
static int TouchY;
bool Shields;

void TaggerMode()
{ 
  char Action = GetButtonPress();
  //Serial.println(Action);   // Feeds lots of blank lines.........
  if      (Action == '1' && Shields == False)   SendIR('T', B0000000);
  else if (Action == '2' && Shields == False)   SendIR('T', B0000001);
  else if (Action == '3' && Shields == False)   SendIR('T', B0000010);
  else if (Action == '4' && Shields == False)   SendIR('T', B0000011);
  // TODO: Adjust ammo : tft.fillScreen(YELLOW);
  else if (Action == 'S') Shields = !Shields;
  else if (Action == 'R') ;                                 // TODO: Reload ammo
  else if (Action == 'E') state = PinCode;
  //if (GetTouch) delay (200);  //delay to stop repeat buttons presses 
}

///////////////////////////////////////////////////////////////////////////////
char GetButtonPress()
{
  if (GetTouch(0))
  {
    if (deBug)
    {
      Serial.print(F("gbpTouch:"));
      Serial.print(TouchX);
      Serial.print(F(", "));
      Serial.println(TouchY);
    }
    delay (200);
    // Match it to a button
    byte row = 0;
    for (row=0; row<13; row++) 
    {
      if (TouchX > Buttons [row] [0] && TouchX < Buttons [row] [2])
      {
        if (TouchY > Buttons [row] [1] && TouchY < Buttons [row] [3])
        {
          Serial.print(F("Button #: "));
          Serial.println(row);
          Serial.print(F("Which is button - "));
          Serial.print(ButtonPressed [row]);
          return ButtonPressed [row];
        }
      }
    }
  }  
}

//////////////////////////////////////////////////////////////////////////////

bool GetTouch(bool Touched)
{ 
  Touched = False;
  
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  TouchX = map(p.x, MinValTouchX, MaxValTouchX, 0, tft.width() );
  TouchY = map(p.y, MinValTouchY, MaxValTouchY, 0, tft.height());
  
  if (p.z >10) Touched = True;
  //Serial.print(Touched);
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

/////////////////// 

///////////////////////////////////////////////////////////////////////////////

void DrawTaggerScreen()
{
  Serial.println(F("DrawTaggerScreen"));
  tft.fillScreen(GREEN);
  
  tft.setCursor(50, 10);
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.println(F("TAG MODE"));

  ButtonCount= 0;
  DrawButton(  5,  40,  100, 55, WHITE,  "1 Tag", 2, BLACK);
  DrawButton(  5, 100,  100, 55, WHITE,  "2 Tag", 2, BLACK);
  DrawButton(  5, 160,  100, 55, WHITE,  "3 Tag", 2, BLACK);
  DrawButton(  5, 220,  100, 55, WHITE,  "4 Tag", 2, BLACK);
  DrawButton(135,  40,  100, 55, BLACK,  "ReLoad",  2, WHITE);
  DrawButton(135, 220,  100, 55, RED,    "Shields", 2, GREEN);
  DrawButton( 70, 290,  100, 30, YELLOW, "EXIT",    2, BLACK);
  if (deBug) PrintButtonArray();
}

//////////////////////////////////////////////////////////////////////////////////////////

void DrawButton(uint16_t PosX, uint16_t PosY, uint16_t Width, uint16_t Height, uint16_t BoxColour,
                String Text, byte TextSize, uint16_t TextColour) 
{
  // Draw the button
  tft.fillRoundRect(PosX, PosY, Width, Height, 10, BoxColour);
  
  // Set the text size & colour
  tft.setTextColor(TextColour);
  tft.setTextSize(TextSize);
  
  // Centre the text on the button
  uint16_t CtrY = PosY+(Height/2)-(7*TextSize/2);                     // characters are 7 pixels high
  uint16_t TextWidth = (Text.length()*5)+(Text.length()-1);           // characterare 5 pixels wide + 1 pixel space
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
      ButtonPressed [Row] = ' ';
    }
  }
  Buttons[ButtonCount] [0]= (PosX);
  Buttons[ButtonCount] [1]= (PosY);
  Buttons[ButtonCount] [2]= (PosX+Width);
  Buttons[ButtonCount] [3]= (PosY+Height);
  ButtonPressed [ButtonCount] = Text.charAt(0);
  ButtonCount++;
}

//////////////////////////////////////////////////////////////////////////////////////////


void PrintButtonArray()
{
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
  for (Column=0; Column<12; Column++)
  {
    Serial.print (ButtonPressed [Column]);
    Serial.print (F(", "));
  }
  Serial.println();
}

