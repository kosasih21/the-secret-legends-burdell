//=================================================================
// The main program file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// Projet includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <math.h>

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))

// Helper function declarations
void playSound(char* wav);


/////////////////////////
// Struct of Player 
/////////////////////////

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;            // Current locations
    int px, py;         // Previous locations
    bool has_key;       // flag when obtained the key
    bool game_solved;   // flag when game is complete
    bool talked_to_npc; // flag when you've talked to npc
    bool slain_buzz;    // flag when buzz is slain
    bool ramblin;       // flag when in ramblin mode

    //You may add more flags as needed

} Player;



/////////////////////////
// Get Action
/////////////////////////

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define GO_RAMBLIN 7

int get_action(GameInputs inputs)
{
    // 1. Check your action and menu button inputs and return the corresponding action value
    if (inputs.b3) {
        return MENU_BUTTON;
    } else if (inputs.b2) {
        return ACTION_BUTTON;
    } else if (inputs.ns_down) {
        return GO_DOWN;
    } else if (inputs.ns_up) {
        return GO_UP;
    } else if (inputs.ns_left) {
        return GO_LEFT;
    } else if (inputs.ns_right) {
        return GO_RIGHT;
    } else if (inputs.ns_center) {
        return GO_RAMBLIN;
    }
    // If no button is pressed, just return no action value
    return NO_ACTION;
}




/////////////////////////
// Helper Functions:
/////////////////////////
// Feel free to define any helper functions here for update game





/////////////////////////
// Update Game
/////////////////////////

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    MapItem* item = NULL;
    

    //******************
    // TODO: Implement
    //******************

    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.

    switch(action)
    {
        MapItem* currItem;
        case GO_UP:
            currItem = get_north(Player.x, Player.y);
            if (currItem->walkable || Player.ramblin) {
                Player.y--; // moves up if walkable
            }
            break;
            
        case GO_LEFT:
            currItem = get_west(Player.x, Player.y);
            if (currItem->walkable || Player.ramblin) {
                Player.x--; // moves up if walkable
            }
            break;
            
        case GO_DOWN:
            currItem = get_south(Player.x, Player.y);
            if (currItem->walkable || Player.ramblin) {
                Player.y++; // moves up if walkable
            }
            break;
            
        case GO_RIGHT:
            currItem = get_east(Player.x, Player.y);
            if (currItem->walkable || Player.ramblin) {
                Player.x++; // moves up if walkable
            }
            break;
            

        case GO_RAMBLIN:
            if (Player.ramblin) {
                Player.ramblin = false;
            } else {
                Player.ramblin = true;
            }
            break;


        case ACTION_BUTTON:

            // 1. Check if near NPC, 
            if (get_south(Player.px, Player.py)->type == NPC || get_north(Player.px, Player.py)->type == NPC ||
            get_east(Player.px, Player.py)->type == NPC || get_west(Player.px, Player.py)->type == NPC) {
                if (!Player.talked_to_npc) {
                    // give quest
                    speech("SIRE! Please, I ", "need YOUR help.");
                    speech("Go to the cave", "there! Slay buzz!");
                    speech("He HATES water!", "It...changes him");
                    Player.talked_to_npc = true;
                } else if (Player.slain_buzz) { // buzz is already slain
                    speech("You did it! HOW?", "No matter how.");
                    speech("You saved us!", "Here is the key!");
                    speech("It will free you!", "You deserve this.");
                    Player.has_key = true; // give player key
                } else { // buzz not slain but already talked to NPC
                    speech("The cave is not ", "far, go south!");
                    speech("Find him! Hurry!", "Make haste!");
                }
                return FULL_DRAW; // return FULL_DRAW to redraw the scene
            }
            

            // 2. Check if near a door
            if (get_south(Player.x, Player.y)->type == DOOR || get_north(Player.x, Player.y)->type == DOOR ||
            get_east(Player.x, Player.y)->type == DOOR || get_west(Player.x, Player.y)->type == DOOR) {
                if (Player.has_key) {
                    speech("Your time is now,", "worthy one, rise.");
                    return GAME_OVER;
                } else {
                    speech("Only worthy to ", "pass. Find Diamond.");
                    return FULL_DRAW;
                }
            }
            


            // 3. Check if on Buzz's cave
            if (get_here(Player.x, Player.y)->type == CAVE) {
                if (Player.talked_to_npc) {
                    // start speech bubble to fight buzz
                    Player.x = 8;
                    Player.y = 14; // set the players coordinates for the small map
                    set_active_map(1);
                    speech("BUZZEZZZZZZEZZ", "MUAHAHAHAAHAHH");
                    speech("I must find the", "powerful spell!");
                    return FULL_DRAW;
                }
            }


            // 4. Check if on a stairs
            if (get_here(Player.x, Player.y)->type == STAIRS) {
                Player.x = 5;
                Player.y = 5;
                set_active_map(0);
                return FULL_DRAW;
            }


            // 5. Check if at the right spell to end the game
            if (get_here(Player.x, Player.y)->type == WATER) {
                // declare buzz is defeated using speech bubble
                Player.slain_buzz = true;
                Player.game_solved = true;
                add_slain_buzz(8, 8);
                speech("    *SWOOSH*", "     *THUMP*");
                speech("I... did it,", "that was easy.");
                return FULL_DRAW;
            }
            break;
    }
    
    return NO_RESULT;
}




/////////////////////////
// Draw Game
/////////////////////////

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
//            if (init && i == 0 && j == 0) // Only draw the player on init
            if ( i == 0 && j == 0) // always draw the player
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
                else if (curr_item && curr_item->type == CLEAR)
                {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}

void draw_game_over() {
    uLCD.filled_rectangle(0, 0, 300, 300, DGREY); // Top
    uLCD.text_string("GAME OVER!", 4, 4, FONT_12X16, WHITE);
}



/////////////////////////
// Map Intialization
/////////////////////////

// Important locations for all maps
int cb_loc[2] = {5,20}; //Location of the center of the cave


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    //Initialize and sets current map to the first map
    Map* map = set_active_map(0);

    /////////////////////////
    //Initial Environmnet
    /////////////////////////

    //Adding random plants
    pc.printf("Adding Plants!\r\n");
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }

    //Adding other plants.
    pc.printf("Adding Alternate Plants!\r\n");
    for(int i = map_width() + 7; i < map_area(); i += 39)
    {
        add_other_plant(i % map_width(), i / map_width());
    }

    //Adding wall borders 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    
    //Adding extra chamber borders 
    pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    add_door(33, 10, HORIZONTAL, 4);

    //Adding extra cave to Buzz's evil lair
    pc.printf("Add cave\r\n");
    add_cave(cb_loc[0],cb_loc[1],1,1,5,5);      //Cave is set as a 4x4 block to be bigger
    add_cave(cb_loc[0]+1,cb_loc[1],2,1,5,5);
    add_cave(cb_loc[0],cb_loc[1]+1,3,1,5,5);
    add_cave(cb_loc[0]+1,cb_loc[1]+1,4,1,5,5);

    pc.printf("Initial environment completed\r\n");

    /////////////////////////////////
    // Characters and Items for the map
    /////////////////////////////////

    // Add NPC
    add_npc(10, 5);  //NPC is initialized to (x,y) = 10, 5. Feel free to move him around

    //Prints out map
    print_map();
       pc.printf("Initial environment completed\r\n");
}







void init_small_map()
{
    //Sets current map to the second map
    set_active_map(1);

    //Adding wall borders 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, 16);
    add_wall(0,              16-1, HORIZONTAL, 16);
    add_wall(0,              0,              VERTICAL,   16);
    add_wall(16-1,  0,              VERTICAL,   16);

    // 2. Add your three spells at different locations
    pc.printf("Adding spells!\r\n");
    add_water(4, 8);
    add_fire(12, 8);
    add_earth(8, 12);

    // 3. Add Evil Buzz at the center of the map
    pc.printf("Adding buzz!\r\n");
    add_buzz(8,8);

    // Add stairs back to main (map 0)
    pc.printf("Adding stairs!\r\n");
    add_stairs(4, 6, 0, cb_loc[0], cb_loc[1]);
    
}


/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initialize the maps
    maps_init();
    init_main_map();
    init_small_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = false;
    Player.game_solved = false;
    Player.talked_to_npc = false;
    Player.slain_buzz = false;
    Player.ramblin = false;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1)
    {

        ////////////////////////////////
        // TODO: Implement 
        ////////////////////////////////

        // Timer to measure game update speed
        Timer t; t.start();
        
        GameInputs temp = read_inputs();
        int ac = get_action(temp);
        int result = update_game(ac);  // Set this variable "result" for the resulting state after update game

        // 3b. Check for game over based on update game result
        if (result == GAME_OVER) {
            // switch to end game screen
            draw_game_over();
            break;
        }
        // 4. Draw screen to uLCD
        bool full_draw = false;
        if (result == FULL_DRAW) full_draw = true;
        draw_game(full_draw);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}




/////////////////////////////
//Advanced Features
/////////////////////////////

// Plays a wavfile
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
    return;
}
