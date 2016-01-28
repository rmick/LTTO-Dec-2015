//  void DrawTextLabel(uint16_t CursorX, uint16_t CursorY, uint16_t BoxColour, String Text, byte TextSize, uint16_t TextColour, uint8_t MaxCharacters) 
//  char Get ButtonPress()
//  bool GetTouch()
//  void DrawButton (PosX, PosY, Width, Height, BoxColour(uint16_t), Text(string), TextSize, TextColour(uint16_t) )
//  void DrawScreen(char* Label, uint16_t BackColour, uint16_t TextColour)
//  void PrintButtonArray()      - for deBug only
//  int CountDigits(int num)

static int TouchX;
static int TouchY;

int Buttons [12] [4];
char* buttonPressed [12];


///////////////////////////////////////////////////////////////////////////////

void DrawTextLabel(uint16_t CursorX, uint16_t CursorY, uint16_t BoxColour,
                String Text, byte TextSize, uint16_t TextColour, uint8_t MaxCharacters) 
{
  bool Centre = FALSE;
  
  // Calculate values if 0 is passed.
  if (CursorX == 0) { CursorX = (240-((Text.length()*5)+Text.length()-1)*TextSize)/2; Centre = TRUE; }
  if (BoxColour == 0) BoxColour = tft.readPixel(CursorX, CursorY);
  if (MaxCharacters == 0) MaxCharacters = Text.length();

  ///////////// The problem is that Cursor X is for the text, which is centred and we need a new Variable for BoxX

  uint16_t BoxPosX;
  // Draw the box
  int BoxWidth = (((MaxCharacters*5)+(MaxCharacters-1))*TextSize)+2;
  if (Centre == TRUE)
  {
    BoxPosX = (240-BoxWidth)/2;
  }
  else
  {
    BoxPosX = CursorX - 1;
  }
  tft.fillRect( BoxPosX, CursorY-1, BoxWidth, (TextSize*7)+2, BoxColour);
  
  // Set the text size & colour
  tft.setTextColor(TextColour);
  tft.setTextSize(TextSize);
  tft.setCursor(CursorX, CursorY);
  tft.println(Text);

  Serial.print(F("DeBug DrawTextLabel - Box Width: "));
  Serial.print (BoxWidth);
  Serial.print(F(", Box Location: "));
  Serial.print((240-BoxWidth)/2);
  Serial.print(F(", "));
  Serial.println(CursorY-1); 
}

///////////////////////////////////////////////////////////////////////////////

char* GetButtonPress()
{
  if (GetTouch(FALSE))
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
          return buttonPressed [row];
          if (deBug)
          {
            Serial.print(F("Button Pressed: "));
            Serial.println(buttonPressed [row]);
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
  
  if (p.z >10) Touched = TRUE;
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
  if (buttonCount == 0)          //Clear the array
  {
    byte row;
    byte column;
    for (row=0; row<12; row++)
    {
      for (column=0; column<4; column++)
      {
        Buttons [row] [column] = 0;
      }
    }
    for (row=0; row<12; row++)
    {
      buttonPressed [row] = "";
    }
  }
  Buttons[buttonCount] [0]= (PosX);
  Buttons[buttonCount] [1]= (PosY);
  Buttons[buttonCount] [2]= (PosX+Width);
  Buttons[buttonCount] [3]= (PosY+Height);
  buttonPressed [buttonCount] = Text;
  buttonCount++;
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
 
  buttonCount = 0;
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
    Serial.print (buttonPressed [Row]);
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
