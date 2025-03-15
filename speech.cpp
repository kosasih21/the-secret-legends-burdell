//=================================================================
// The speech class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "speech.h"

#include "globals.h"
#include "hardware.h"
#include "map.h"

///////////////////////////////
//Static function declarations
///////////////////////////////
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);


///////////////////////////////
//Drawing function declarations
///////////////////////////////


//***
// HINT: for this function and below: Check out the ULCD demo in the docs to see what
// uLCD methods you can use to draw graphics 

//Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
//***


void draw_speech_bubble()
{
    //1. Draw a speech bubble at the bottom of the screen 
    uLCD.rectangle(0, 93, 127, 126, RED);
    uLCD.filled_rectangle(1, 94, 126, 125, BLACK);
    
}

void erase_speech_bubble()
{
    uLCD.filled_rectangle(0, 93, 127, 126, BLACK);
}

void draw_speech_line(const char* line, int which)
{
    // y = 13 for top and y = 14 for bottom
    int y = which + 13;
    uLCD.text_string((char *) line, 1, y, FONT_5X7, WHITE);
}

void speech_bubble_wait()
{
    while (1) {
        if (read_inputs().b2) {
            break;
        }
        pc.printf("Waiting for action to continue.\r\n");
        wait_ms(40);
    }
}

void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    wait_ms(1000);
    speech_bubble_wait();
    erase_speech_bubble();
}

void long_speech(const char* lines[], int n)
{

    //****************
    // TODO: Implement
    //****************

    //1. Create a speech bubble

    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)

    //3. Erase the speech bubble when you are done

}

