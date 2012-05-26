/* Conway's Game of Life
*
*  2012-05-26
*  Mike Szczys
*  jumptuck.com
*  CC-BY-SA
*
*  Hardware: Arduino and 20x4 character LCD
*
*  This program will play Conway's Game of Life
*  of a 20x4 character LCD. It uses custom characters
*  to divide each row in two for a total resolution of 20x8
*
*  REQUIRES:
*  LiquidCrystal library that is built into the Arduino IDE
*
*  TODO:
*  -Fix bug that generates garbage at column 0
*  -Add a button for more random input
*
*/


#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*Display buffer:
*  index 0-19 for characters left to right
*
*  -each column is a bye
*  -LSB is top row (four character rows, eight virtual rows)
********************************/
char lifeboard[20] = {
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256),
  random(256)
};

// make some custom characters:
byte box_full[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b01110
};

byte box_N[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte box_S[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b01110,
  0b01110
};

void evolve() {
  //Setup a temporary game baord for the next generation
  unsigned char nextGeneration[20];
  for (unsigned char i=1; i<20; i++) nextGeneration[i] = 0;
  
  //Iterate through all current cells to calculate next generation
  for (unsigned char row=0; row<8; row++) {
    for (unsigned char col=0; col<20; col++) {
      //get state of this cell
      unsigned char thisCell = 0;
      if (lifeboard[col]&(1<<row)) thisCell = 1;
      
      
      //===start check of all neighbors===
      unsigned char neighbor_count = 0;
      
      //get search coordinates
      
      unsigned char neighbor_cols[3] = {col-1, col, col+1};
      //fix wrap around for board
      if (col == 0) neighbor_cols[0] = 19;
      if (col == 19) neighbor_cols[2] = 0;
      
      unsigned char neighbor_rows[3] = {row-1, row, row+1};
      //fix wrap around for board
      if (row == 0) neighbor_rows[0] = 7;
      if (row == 7) neighbor_rows[2] = 0;
      
      for (unsigned char i=0; i<3; i++) {
        for (unsigned char j=0; j<3; j++) {
          if (lifeboard[neighbor_cols[i]]&(1<<neighbor_rows[j])) neighbor_count += 1;
        }
      }
      
      //Subtract current cell as it was counted as a neighbor
      neighbor_count -= thisCell;
      
      //===end check of all neighbors===
      
      //use neighbor_count to check Conway's Game of Life rules
      if (thisCell) {
        if ((neighbor_count < 2) || (neighbor_count > 3)) nextGeneration[col] &= ~(1<<row);
        else nextGeneration[col] |= (1<<row); 
      }
      else if (neighbor_count == 3) nextGeneration[col] |= 1<<row;      
    }
  }
  //copy all temp values to the screen game board
  for (unsigned char i=0; i<20; i++) {
    lifeboard[i] = nextGeneration[i];
  }
}


void setup() {
  lcd.createChar(0, box_full);
  lcd.createChar(1, box_N);
  lcd.createChar(2, box_S);

  lcd.begin(20, 4);
}

void loop() { 

  for (unsigned char row=0; row<4; row++){
    lcd.setCursor(0, row);
    unsigned char v_row1 = row*2;
    unsigned char v_row2 = v_row1 + 1;
    for (unsigned char col=0; col<20; col++) {
      if ((lifeboard[col]&(1<<v_row1)) && (lifeboard[col]&(1<<v_row2))) lcd.write((byte)0);
      else if (lifeboard[col]&(1<<v_row1)) lcd.write((byte)1);
      else if (lifeboard[col]&(1<<v_row2)) lcd.write((byte)2);
      else lcd.print(' ');
      
      
    }
  }
  
  evolve();
  delay(300);
  
  //lcd.print(millis()/1000);
}

