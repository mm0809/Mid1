#include "mbed.h"
#include "uLCD_4DGL.h"
#define ARROWMAX 3
#define ARROWMIN 0
#define Ratio (0.91)

uLCD_4DGL uLCD(D1, D0, D2);

InterruptIn upBtn(D11);
InterruptIn dwBtn(D9);
InterruptIn selBtn(D10);
//DigitalOut led1(LED1);
AnalogOut Aout(PA_4);
AnalogIn Ain(A0);

int arrow = ARROWMAX;

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t(osPriorityLow);

EventQueue queueSample(32 * EVENTS_EVENT_SIZE);
Thread tSample(osPriorityNormal);

float ADCdata[1000];
//Arrow
//3  2   1   0
//1 1/2 1/4 1/8
//
int jId1[4] = {5, 10, 20, 40};
int jId2[4] = {115, 110, 100, 80};
float unit[4] = {0.2,0.1,0.05,0.025};

void Sample(int id)
{
    ADCdata[id] = Ain;
}

void menuArrowUpdate();
void genWave()
{
    int id = 0;

    printf("500\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 120; j++) {
            if (j < jId1[arrow]) {
                Aout = Ratio * (j * unit[arrow]);
            } else if (j < jId2[arrow]) {
                Aout = Ratio * 1;
            } else {
                Aout = Ratio * (1 - (j - jId2[arrow]) * unit[arrow]);
            }
            queueSample.call(Sample, id);
            id++;
            ThisThread::sleep_for(2ms);
        } 
    }
    Aout = 0;
    for (int i = 0; i < 500; i++) {
        printf("%f\n", ADCdata[i]);
    }
    //printf("max data %d\n", id);
}

void arrowUp()
{
    if(arrow != ARROWMAX) arrow++;
    queue.call(menuArrowUpdate);
}
void arrowDw()
{
    if(arrow != ARROWMIN) arrow--;
    queue.call(menuArrowUpdate);
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

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    tSample.start(callback(&queueSample, &EventQueue::dispatch_forever));
    upBtn.rise(&arrowUp);
    dwBtn.rise(&arrowDw);
    selBtn.rise(queue.event(genWave));

    while (1){
        //menuArrowUpdate();
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
