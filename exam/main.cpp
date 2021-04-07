#include "mbed.h"
#include "uLCD_4DGL.h"
#define ARROWMAX 3
#define ARROWMIN 0

uLCD_4DGL uLCD(D1, D0, D2);

InterruptIn upBtn(D11);
InterruptIn dwBtn(D9);
InterruptIn selBtn(D10);
//DigitalOut led1(LED1);

int arrow = ARROWMAX;




void arrowUp()
{
    if(arrow != ARROWMAX) arrow++;
}
void arrowDw()
{
    if(arrow != ARROWMIN) arrow--;
}
void menuArrowUpdate()
{
    int yPos = 1;
    for (int i = ARROWMAX; i >= 0; i--) {
        uLCD.locate(10, yPos);
        if (i == arrow)
            uLCD.printf("%s", "<--");
        else
            uLCD.printf("%s", "   ");
        yPos++;
    }
}



int main() {
    uLCD.locate(1,1);
    uLCD.printf("%s", "1");
    uLCD.locate(1,2);
    uLCD.printf("%s", "1/2");
    uLCD.locate(1,3);
    uLCD.printf("%s", "1/4");
    uLCD.locate(1,4);
    uLCD.printf("%s", "1/8");
    menuArrowUpdate();

    upBtn.rise(&arrowUp);
    dwBtn.rise(&arrowDw);
    while (1){
        menuArrowUpdate();
        ThisThread::sleep_for(100ms);
        
        //if (upBtn.read()) {
        //    arrowUp();
        //    menuArrowUpdate();
        //    ThisThread::sleep_for(100ms);
        //}
        //if (dwBtn.read()) {
        //    arrowDw();
        //    menuArrowUpdate();
        //    ThisThread::sleep_for(100ms);
        //}
        //if (selBtn.read()) {
        //    ThisThread::sleep_for(100ms);
        //}
    }
}
