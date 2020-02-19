#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
#include <MPU6050_tockn.h>
#include <Wire.h>

#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

Adafruit_SSD1306 display(128, 64);  // Create display
MPU6050 mpu6050(Wire);

int Variable1;  // Create a variable to have something dynamic to show on the display

void setup() {
  delay(100);  // This delay is needed to let the display to initialize
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
  display.clearDisplay();  // Clear the buffer
  display.setTextColor(WHITE);  // Set color of the text
  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3
  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
                               // To override this behavior (so text will run off the right side of the display - useful for
                               // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                               // with setTextWrap(true).
  display.dim(0);  //Set brightness (0 is maximun and 1 is a little dim)
  
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();
  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());

  Variable1++;  // Increase variable by 1
  if(Variable1 > 150)  // If Variable1 is greater than 150
  {
    Variable1 = 0;  // Set Variable1 to 0
  }




  // Convert Variable1 into a string, so we can change the text alignment to the right:
  // It can be also used to add or remove decimal numbers.
  char string[10];  // Create a character array of 10 characters
  // Convert float to a string:
  dtostrf(Variable1, 3, 0, string);  // (<variable>,<amount of digits we are going to use>,<amount of decimal digits>,<string name>)






  display.clearDisplay();  // Clear the display so we can refresh


  display.setFont(&FreeMono9pt7b);  // Set a custom font
  display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0


  // Print text:
  display.setCursor(0, 10);  // (x,y)
  display.println("Hello");  // Text or value to print


  // Draw triangle:
  display.drawTriangle(40,40,   50,20,   60,40, WHITE);  // Draw triangle. X, Y coordinates for three corner points defining the triangle, followed by a color

  // Draw filled triangle:
  display.fillTriangle(0,63,   15,45,   30,63, WHITE);  // Draw filled triangle. X, Y coordinates for three corner points defining the triangle, followed by a color

  // Draw line:
  display.drawLine(40, 63, 70, 63, WHITE);  // Draw line (x0,y0,x1,y1,color)

  // Draw circle:
  display.drawCircle(47, 36, 20, WHITE);  //  Draw circle (x,y,radius,color). X and Y are the coordinates for the center point

  // Draw a filled circle:
  display.fillCircle(12, 27, 10, WHITE);  // Draw filled circle (x,y,radius,color). X and Y are the coordinates for the center point

 // Draw rounded rectangle and fill:
  display.fillRoundRect(58, 0, 18, 18, 5, WHITE);  // Draw filled rounded rectangle (x,y,width,height,color)
                                                   // It draws from the location to down-right


  
  
  
  // Draw rectangle:
  display.drawRect(79, 0, 49, 27, WHITE);  // Draw rectangle (x,y,width,height,color)
                                           // It draws from the location to down-right
   
  display.setFont(&FreeMonoBold12pt7b);  // Set a custom font
  
  // Print variable with left alignment:
  display.setCursor(83, 20);  // (x,y)
  display.println(Variable1);  // Text or value to print



  

  // Draw rounded rectangle:
  display.drawRoundRect(79, 37, 49, 27, 8, WHITE);  // Draw rounded rectangle (x,y,width,height,radius,color)
                                                    // It draws from the location to down-right

  // Print variable with right alignment:
  display.setCursor(83, 57);  // (x,y)
  display.println(string);  // Text or value to print




  
  display.display();  // Print everything we set previously
}
