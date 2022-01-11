#include <iostream> //used for standard inputs and outputs
#include "windows.h" //contains declarations for all of the functions in the Windows API
#include <ctime> //used to call upon your localised time
#include <string> //used for manipulating and using strings
#include <conio.h> //used for advanced console inputs and outputs - clearing screen and registering keypresses
#include <dos.h> //used for producing system sounds
#include <fstream> //used for IO manipulation (leaderboard)
#include "MMSystem.h" //used for sound manipulation
#include <thread> //used for multithreading and joining threads
#include <cstdlib> //used for random number generation and DMA
#include <stdio.h> //used for maps and typing dialogue
#include <unistd.h> //used for IO manipulation (leaderboard)
#include <time.h> //time manipulation for plotting delays and sleep function
#include <chrono> //time manipulation for plotting delays
#include <cmath> //included for advanced maths
#include "resource.h" //for icon changing

using namespace std;

void delay1(unsigned int mSecs) //delay function used for plotting

        {
                clock_t endTime = mSecs + clock();
                while (endTime > clock());
                usleep(1000); // sleep instruction cycle
        }

struct powerups //declaring powerups structure
{
    string buffname; //powerup name
    int healthbuff; //different powerups
    int scorebonus;
    int damagebonus;
    powerups* p_next_powerup; //pointer to next powerup
};

powerups* getNewPowerup() //med-kit powerup setup
{
    powerups* p_powerup = new powerups; //declaring new powerup
    p_powerup->buffname = "Health Buff"; //powerup name
    p_powerup->healthbuff = 100; //initial health
    return p_powerup; //returning the value of the powerup
}

powerups* medkit (powerups* p_powerup) //additional health buff (changing powerup)
{
    p_powerup->healthbuff +=10; //add 10 to the value of health
    return p_powerup; //return value
}

char Map[50][50] = //original map used (game board)
{
    " _____________________________",
    "|                             |",
    "|    ~   ~   ~   ~   ~   ~    |",
    "|      ~   ~   ~   ~   ~      |",
    "|        ~   ~   ~   ~        |",
    "|                             |",
    "|                             |",
    "|                             |",
    "|                             |",
    "               ^               ",

};

char bossmap[50][50] = //boss map (game board)
{
    " _____________________________",
    "| ====                   ==== |",
    "|           ##  ##            |",
    "|           ######            |",
    "|         ###O@@O###          |",
    "|        ###      ###         |",
    "|-----------------------------|",
    "|                             |",
    "|                             |",
    "               ^               ",

};

char Map2[50][50] = // map 2 (game board)
{
    " _____________________________",
    "|                             |",
    "|                             |",
    "|   ____     ____      ____   |",
    "|                             |",
    "|   ____     ____      ____   |",
    "|                             |",
    "|                             |",
    "|                             |",
    "               ^               ",

};

char bossdia[] = //bosses dialogue - drawn in type writer style
{
    "This puny ship thinks he can destroy me?!\n\nNot on my watch...\n\nI don't need minions to destroy you!"
};
char bossdes[] = //bosses dialogue - drawn in type writer style
{
    "Arrrghhhhhhhhhhhh\n\n\n\n\n*BOOM*"
};


bool finish = false; //sets endgame initially to false
bool bossfinish = false; //sets boss finish initially to false
int speed = 50; // how fast the player can move and the game runs
int startinghealth = 100; // starting health of the player
int health = startinghealth; //health is set to 100
int score = 0; //score is set to 0
int randomNum, randomNum2, randomNum3, randomNum4, randomNum5; //random number initialisers
int remenemies = 14; //remaining enemies needed to clear wave
int bosshealth = 100; // boss health is 100
int grand (time(0)); // How fast the bombs are falling

int loadingscreen() //loading screen thread
{
    system("Space Invaders 3.0"); //names the console
	string home[4] = {"Play", "Options",  "Leaderboard", "Rules"}; //using a string for menu selector
    string options[3] = {"Easy", "Normal", "Hard"}; // using a string for difficulty selection

	int pointer = 0; //pointer is set to 0

	while(true) //while waits for a condition and a break
	{
		system("cls"); //clears console
		system("color 0a"); //sets the console colour to green

		/////////////////
		// MENU LAYOUT //
        /////////////////

        cout<<"    ##          ##"    <<endl;
        cout<<"      ##      ##    "  <<endl;
        cout<<"    ##############"    <<endl;
        cout<<"  ####  ######  ####"  <<endl;
        cout<<"######################"<<endl;
        cout<<"##  ##############  ##"<<endl;
        cout<<"##  ##          ##  ##"<<endl;
        cout<<"      ####  ####"      <<endl;
        cout<<"                       "<<endl;
        cout<<"    []           []    "<<endl;
        cout<<"  [][][]       [][][]  "<<endl;
        cout<<"[][][][][]   [][][][][]"<<endl;
        cout<<"[][][][][]   [][][][][]"<<endl;

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //set default text colour to green
		cout << "\n\nSPACER INVADERS 3.0\n\n"; //prints to the console

		for (int i = 0; i < 4; ++i) //for when i is less than 4 (amount of menu options)
		{
			if (i == pointer) //i is equal to the pointer
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); //set unhighlighted text colour to white
				cout << home[i] << endl; //print out the menu
			}
			else //if the menu option is not on that option
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //set highlighted text to green
				cout << home[i] << endl; //print out the menu
			}
		}

		while(true) //while waits for a condition and a break
		{
			if (GetAsyncKeyState(VK_UP) != 0) //when up key is pressed
			{
				pointer -= 1; //subtract one from pointer
				if (pointer == -1) //if pointer = -1
				{
					pointer = 2; //set pointer to 2
				}
				break; //break to next statement
			}
			else if (GetAsyncKeyState(VK_DOWN) != 0) //when down key is pressed
			{
				pointer += 1; //add one to pointer
				if (pointer == 4) //when pointer = 4
				{
					pointer = 0; //set pointer back to 0 to repeat the cycle
				}
				break; //break to next statement
			}
			else if (GetAsyncKeyState(VK_RETURN) != 0)//when return key is pressed
			{
				switch (pointer) //switch case for menu option selection
				{
					case 0: //case 0 is playing the game
					{
						cout << "\n\n\nLoading Level..."; //prints loading level
						Sleep(100); //slight delay
						return 0; //going to next thread
					}

					break; //break to next statement

					case 1://case 1 is for options
					{
						cout << "\n\n\nOptions: "; //prints options to console
						cout << "\n\nSpeed = "<<speed<<"\n"<< "Change Difficulty? "<<"Y/N";
						string input; cin >> input; string Y = "y"; string N = "n"; //asks user for input

                        if (input == Y) //if input is yes
                        {
                            /////////////////////////
                            // DIFFICULTY CHANGING //
                            /////////////////////////
                            cout << "\nEasy(1), Normal(2) or Hard(3)? "; int dinput; cin >> dinput;

                            switch(dinput) //statement to set difficulty
                            {
                                case 1: dinput = 1;
                                {int randomNum = randomNum-10; cout<<"The difficulty has been changed to easy.\n";return 0;}
                                break; //break to next statement
                                case 2: dinput = 2;
                                {int randomNum = randomNum; cout<<"The difficulty has been changed to Normal.\n";return 0;}
                                break; //break to next statement
                                case 3: dinput = 3;
                                {int randomNum = randomNum+30; cout<<"The difficulty has been changed to Hard.\n";return 0;}
                                break; //break to next statement
                            }
                        }
                        if (input == N) //if input is no
                        {
                            return(0); //break to next thread
                        }
                        else
                        {
                            return 0;
                        }
						system("\npause"); //pause console
					}

					break; //break to next statement

					case 2: //case for leaderboard
					{
                        system("cls"); //clear console
                        cout <<"LASTEST SCORES:"; //write to console
                        ofstream myfile ("LeaderBoard.txt"); //read from text file

                              if (myfile.is_open()) //if file is open correctly
                              {
                                {
                                  cout <<"\n"<< score << " Points!\n"; //write the score to the console
                                }
                                myfile.close(); //close the file
                                system("pause"); //pause the console
                              }
					} break; //break to next statement

					case 3: //case for rules
                        {
                            system("cls"); //clear the console
                            // writes the rules to the console
                            cout<< "RULES:\n\nYou must fend off waves of enemies by using your ships cannons.\n";
                            cout<< "In retro space invader fashion their will be enemies that fire back and move.\n";
                            cout<< "Your bullets will destroy enemy ships in 1 hit, but your ship can take 10 hits.\n";
                            cout<< "If an enemy reaches your spawn you will instantly lose the game.\n";
                            cout<< "Medical aid will occasionally spawn in the pre-game, but will not in the boss battle.\n";
                            cout<< "After you eliminate the correct number of ships a boss with spawn and will take 10 hits.\n";
                            cout<< "\n\nCONTROLS:\n\nLeft arrow: Move left\nRight arrow: Move right\nSpace bar: Shoot bullet\nControl: Shoot armour penetration bullet\nShift: Shoot multi-shot bullet\n\n";
                            system("pause");
                        }
                        break; //break to next statement
				}
				break; //break to next statement
			}
		}

		Sleep(150); //sleeps 150 milliseconds between switches
	}
	return 0;
}

int main(int argc, char* argv[]) //main body of code
{
//  start:
    std::thread t1(loadingscreen); //initialises thread - loading screen
    t1.join(); //joins thread 1 (loading screen) and this main thread together

    //starts to play WAVE file from local document. ASYNC makes it plays in the background.
//  PlaySound(TEXT("Soviet_Union_-_National_Anthem_Chiptune_Cover-8ilcx_c8OpU.wav"), NULL, SND_FILENAME|SND_ASYNC);
    system("color 0a");// set console colour to green again
    system("cls"); //clear console

        while (finish == false) //when the game is not finished

        {
            system("cls"); //clear console in loop for console refresh and plotting
            for (int y = 0; y<20; y++) //for when y is less than 20

              {
               cout << Map[y] << endl; //draw the map to each line (20 lines)

                 }
                    cout << "Health:" << health << "/" << startinghealth << endl; //write your health and starting health to screen
                    cout << "Score:" << score << endl; //write score to screen
                    cout << "Aliens Left to clear: " << remenemies; //write remaining enemies to screen

                   for (int y = 0; y<20; y++) //loop for if y is less than 20 - moves player in array

                    {

                    for (int x = 0; x<32; x++) //loop for if x is less than 32 - moves player in array

                        {
                            switch (Map[y][x]) //switch case for our map

                            {
                               //////////////
                               // MOVEMENT //
                               //////////////

                               case '^': //case for the ^ character (player)

                               if (GetAsyncKeyState(VK_LEFT) != 0) //if left keyboard button is pressed
                                {
                                    int newX = x-1; //update x position to the left
                                    switch (Map[y][newX]) //switch case for the updated map
                                    {

                                    case ' ': //if the space is empty
                                    Map[y][x] = ' '; //set current position to empty
                                    x--; //update x position in iterations
                                    Map[y][newX] = '^'; //draw the character at the new position

                                    break; //break to next statement

                                    }
                                }
                                if (GetAsyncKeyState(VK_RIGHT) != 0) //if right keyboard button is pressed
                                {
                                     int newX = x+1; //update x position to the right
                                     switch (Map[y][newX])//switch case for the updated map
                                     {

                                      case ' ':  //if the space is empty
                                      Map[y][x] = ' '; //set current position to empty
                                      x++; //update x position in iterations
                                      Map[y][newX] = '^'; //draw the character at the new position

                                      break;//break to next statement

                                    }
                                }
                                if (GetAsyncKeyState(VK_SPACE) != 0) //if space button is pressed
                                {
                                    y--;//y position is updated in iterations
                                    Map[y][x] = '.';//. character (bullet) shoots up the map
                                }

                                break; //break to next statement

                                ///////////////////////////////////////////////////////
                                // ENEMY MOVEMENT, BULLETS, HEALTH, SCORE & POWERUPS //
                                ///////////////////////////////////////////////////////

                                      case '.': //case for the bullet character

                                        Map[y][x] = ' '; //draws the previous position as empty
                                        y--; //bullet goes up in iterations until:

                                        //if the bullet does not hit the side of the map or enemy
                                        if(Map[y][x] != '_'&& Map [y][x] != '~')
                                            {
                                                Map[y][x] = '.'; //change the character to a bomb
                                            }

                                        else if  (Map [y][x] == '~') //otherwise if it hits the enemy
                                        {
                                            Map[y][x] = ' '; //replace the enemy with an empty space
                                            score+=10; // add 10 to your current score
                                            remenemies-=1; // remove an enemy from remaining enemies
                                            Beep(523,50); //make a noise when the enemy is hit

                                        }

                             break; //break to next statement

                                    case '~': //case for the enemy

                                        randomNum = rand() %50 + 1; //random number in the range 0 to 50
                                        randomNum2 = rand() %200 + 1; //random number in the range 0 to 200

                                        if (randomNum == 1) //if random number is 1
                                            {
                                                //spawn a bomb and make it fall down the map
                                                y++; //going down in iterations
                                                Map[y][x] = '*'; //draw this character to the map

                                            }
                                        //if the position of the enemy character is not 6 and random number is 1
                                        //or there is not a enemy character in that position
                                        else if ((y!=6 && randomNum2 == 1) || Map[y][x] != '~')
                                            {
                                               //move a random enemy character down by 1 position
                                               Map[y][x] = ' '; //update previous position to empty space
                                               y++; // enemy moves down by 1 position
                                               Map[y][x] = '~';  //updates position of enemy
                                            }
                                        if (y==9) //if the enemy reaches your character
                                        {
                                            finish = true; //set endgame to true
                                        }

                                        break; //break to next statement

                                    case '*': //case for the enemy bomb

                                    //bomb falling down the map
                                    Map[y][x] = ' '; //update previous position to empty space
                                    y++; //update y position in iterations until:

                                    //if the bomb will not hit the map, player or other enemy
                                    if (Map [y][x]!= '_' && Map [y][x] != '^' && Map[y][x] != '~')
                                    {
                                        Map[y][x] = '*'; //update the character to the bomb
                                    }

                                    else if (Map[y][x] == '^') //else if the bomb hits the player
                                        {
                                            health -=10; //subtract 10 health points from the player
                                        }

                                    break; //break to next statement

                                    case '_': //case for roof of map

                                    randomNum5 = rand() %10000 + 1; //random number in the range 0 to 10000

                                    if (randomNum5 == 1) //if random number is 1
                                            {
                                                //spawn a med kit that falls down the map
                                                y++; //going down in iterations
                                                Map[y][x] = '+'; //draw this character to the map

                                            }

                                    break; //break to next statement

                                    case '+': //case for med-kit powerup

                                    Map[y][x] = ' '; //update previous position to empty space
                                    y++; //update y position in iterations until:

                                    //if the health kit will not hit the map, player or other enemy
                                    if (Map [y][x]!= '_' && Map [y][x] != '^' && Map[y][x] != '~')
                                    {
                                        Map[y][x] = '+'; //update the character to the bomb
                                    }

                                    else if (Map [y][x] == '^') //if the med kit hits the player
                                    {
                                        health +=5; //add an additional 5 points of health to the player
                                    }

                                    break; //break to next statement
                            }

                        }

                    }

                if (remenemies <= 0) //if the wave is cleared

                {

                //////////////////////
                // BOSS BATTLE CALL //
                //////////////////////

                system("cls"); //clear the console
                break; //break out of the loop into the boss battle

                }

                 if (health <=0) //if health is 0 or less
                    {

                        string name; //store name
                        cout <<"\nPlease enter your name:"; //write to console
                        cin >> name; //stores user input to name string
                        finish = true; //set finish to true

                        ofstream myfile ("LeaderBoard.txt"); //opens the leaderboard text file to write

                        if (myfile.is_open()) //checks if the file is open correctly
                            {
                                myfile << name << " " << score << " Points!" ; //write score and name to the file
                                myfile.close(); //close the file
                            }
                        }

                Sleep(speed); //sleep for the set gamespeed
        }
        system("cls"); //clear the console

        if (finish == true) // when the game is finished
        {

        //////////////////////
        // GAME OVER SCREEN //
        //////////////////////

cout <<" _______ _______ _______ _______     "<< endl;
cout <<"|     __|   _   |   |   |    ___|    "<< endl;
cout <<"|    |  |       |       |    ___|    "<< endl;
cout <<"|_______|___|___|__|_|__|_______|    "<< endl;
cout <<"                                     "<< endl;
cout <<" _______ ___ ___ _______ ______      "<< endl;
cout <<"|       |   |   |    ___|   __ |     "<< endl;
cout <<"|   -   |   |   |    ___|      <     "<< endl;
cout <<"|_______||_____||_______|___|__|     "<< endl;
cout <<"                                     "<< endl;
cout <<"                                     "<< endl;
cout <<"LASTEST SCORES:                      "<< endl;

  ifstream myfile ("LeaderBoard.txt"); //reads from the leaderboard file
  if (myfile.is_open()) //if the file is open correctly
  {
    {
      cout << score << '\n'; //read the player score and write to console
    }
    myfile.close(); //close the document
  }
   system ("PAUSE"); //pause the console
   return EXIT_SUCCESS; //return the exit status

}
    //////////////////////////////////////////
    // BOSS MAP/BATTLE - SOME REPEATED CODE //
    //////////////////////////////////////////

    for (unsigned int x = 0; x < strlen(bossdia); x++) //writes out the string character by character
        {
		  cout << bossdia[x]; //writes one letter to the console (x direction)
		  Sleep(100); // pause for 0.1 second
        }

        while (finish == false) //while loop for boss battle
        {
            system("cls"); // clear the console
            cout << "\tBOSS HEALTH: " << bosshealth << endl; // write out the bosses health to the top of the console

            for (int y = 0; y<20; y++) //loops until the board is printed

              {
               cout << bossmap[y] << endl; //print the new 'boss map' line by line

              }

                    // write the following to the bottom of the map
                    cout << "Oh no! They have a force field up!\nUse the 'Shift' key to blast through and 'Control' to finish him!"<< endl;
                    cout << "\n\nHealth:" << health << "/" << startinghealth << endl; //prints health over maxhealth
                    cout << "Score:" << score << endl; //prints current score (stays updated)

                    for (int y = 0; y<20; y++) //loop for if y is less than 20 - moves player in array

                    {

                    for (int x = 0; x<32; x++) //loop for if x is less than 32 - moves player in array

                        {
                            switch (bossmap[y][x]) //switch statement for boss map

                            {

                               //////////////
                               // MOVEMENT //
                               //////////////

                               case '^': //case for the ^ character (player)

                               if (GetAsyncKeyState(VK_LEFT) != 0) //if left keyboard button is pressed
                                {
                                    int newX = x-1; //update x position to the left
                                    switch (bossmap[y][newX]) //switch case for the updated map
                                    {

                                    case ' ': //if the space is empty
                                    bossmap[y][x] = ' '; //set current position to empty
                                    x--; //update x position in iterations
                                    bossmap[y][newX] = '^'; //draw the character at the new position

                                    break;//break to next statement

                                    }
                                }
                                if (GetAsyncKeyState(VK_RIGHT) != 0) //if right keyboard button is pressed
                                {
                                     int newX = x+1; //update x position to the right
                                     switch (bossmap[y][newX])//switch case for the updated map
                                     {

                                      case ' ':  //if the space is empty
                                      bossmap[y][x] = ' '; //set current position to empty
                                      x++; //update x position in iterations
                                      bossmap[y][newX] = '^'; //draw the character at the new position

                                      break;//break to next statement

                                    }
                                }
                                if (GetAsyncKeyState(VK_SPACE) != 0) //if space button is pressed
                                {
                                    y--;//y position is updated in iterations
                                    bossmap[y][x] = '.';//. character (bullet) shoots up the map
                                }

                                //power up key

                                else if(GetAsyncKeyState(VK_SHIFT) != 0) //if shift is clicked
                                {
                                    y--; //y position is updated in iterations
                                    bossmap[y][x] = 'o'; //o character (bunker buster) shoots up the map
                                }

                                else if (GetAsyncKeyState(VK_CONTROL) != 0) //if control is clicked
                                {
                                    y--; //y position is updated in iterations
                                    bossmap[y][x] = '+';//+ character (multi bullet) shoots up the map
                                }

                                break; //break to next statement

                                ///////////////////////////////////////////////////////
                                // ENEMY MOVEMENT, BULLETS, HEALTH, SCORE & POWERUPS //
                                ///////////////////////////////////////////////////////

                                case '.': //case for the bullet character

                                        bossmap[y][x] = ' '; //draws the previous position as empty
                                        y--; //bullet goes up in iterations until:

                                        //if the bullet does not hit the side of the map or enemy
                                        if(bossmap[y][x] != '_'&& bossmap [y][x] != '~')
                                            {
                                                bossmap[y][x] = '.'; //change the character to a bomb
                                            }

                                        //if bullet hits any part of the main boss
                                        if  (bossmap[y][x] == '#'||bossmap[y][x] == '@'||bossmap[y][x] == 'O')
                                        {
                                            score+=10; //add 10 to your score
                                            bosshealth -=5; //subtract 5 from bosses health
                                            Beep(523,50); //play beep noise


                                        }
                                        else if  (bossmap [y][x] == '~') //otherwise if it hits the enemy
                                        {
                                            bossmap[y][x] = ' '; //replace the enemy with an empty space
                                            score+=10; // add 10 to your current score
                                            Beep(523,50); //make a noise when the enemy is hit
                                        }

                                 break; //break to next statement

                                 case 'o': //case for the bunker buster

                                        bossmap[y][x] = ' '; //draws the previous position as empty
                                        y--; //bullet goes up in iterations until:

                                        //if the o character is not about hit any of the following characters:
                                        if(bossmap[y][x] != '_'&& bossmap[y][x] != '~'&& bossmap[y][x] !='#'&& bossmap[y][x] !='O'&& bossmap[y][x] !='@')
                                        {
                                                bossmap[y][x] = 'o'; //replace the character with the bullet
                                        }

                                        //if the o character comes into contact with any of these characters:
                                        if  (bossmap[y][x] == '#'||bossmap[y][x] == '@'||bossmap[y][x] == 'O')
                                        {
                                            cout <<"\nThe boss is immune to this power!"; //print this to the console
                                            Sleep(200); //pause the console - this will then clear

                                        }
                                        else if (bossmap[y][x] == '~') //else if the o hits the ~ character
                                        {
                                           bossmap[y][x] = ' '; // replace the enemy with a blank space
                                           score+=10; //add 10 to your score
                                           Beep(523,50); //make beep noise
                                        }

                                        break; //break to next statement

                                 case '+': // case for the multi shot

                                        bossmap[y][x] = ' '; //draws the previous position as empty
                                        y--; //bullet goes up in iterations until:

                                        //if the + character is not about hit any of the following characters:
                                        if(bossmap[y][x] != '_'&& bossmap[y][x] != '~'&& bossmap[y][x] !='#'&& bossmap[y][x] !='O'&& bossmap[y][x] !='@'&& bossmap[y][x] !='-')
                                        {
                                                bossmap[y][x] = '+'; //replace the character with the bullet
                                        }

                                        //if the + character comes into contact with any of these characters:
                                        if  (bossmap[y][x] == '#'||bossmap[y][x] == '@'||bossmap[y][x] == 'O')
                                        {
                                            bosshealth -=20; //subtract 20 from the bosses health
                                            Beep(523,50); //play beep noise
                                            cout <<"\nCritical Hit! You have scored no points from this hit."; //print this to the console
                                            Sleep(200); //pause the console - this will then clear
                                        }

                                        break; //break to next statement

                                 case 'O': //case for the eyes of the boss

                                    randomNum3 = rand() %100 + 1; //random number generation (between 1-100)
                                    randomNum3 = rand() %100 + 1; //random number generation (between 1-100)

                                    if (randomNum3 == 1) //if the random number = 1
                                            {
                                                //three V characters drop vertically
                                                y++;
                                                bossmap[y][x] = 'v';
                                                y++;
                                                bossmap[y][x] = 'v';
                                                y++;
                                                bossmap[y][x] = 'v';
                                            }

                                        break; //break to next statement

                                 case 'v': // case for the v character

                                    bossmap[y][x] = ' '; //previous position is replaced with blank space
                                    y++; // v moves down in iterations

                                    //if the v character is not about hit any of the following characters:
                                    if (bossmap [y][x]!= '_' && bossmap [y][x] != '^' && bossmap[y][x] != '~')
                                    {
                                        bossmap[y][x] = 'v'; //replace the character with a v
                                    }

                                    else if (bossmap[y][x] == '^') //else if the v comes into contact with the player
                                        {
                                            health -=20; //subtract 20 from your health
                                        }

                                        break; //break to next statement

                                 case '=': //case for the boss pipes

                                    randomNum4 = rand() %100 + 1; //random number generation (between 1-100)

                                    if (randomNum4 == 1) //if random number = 1
                                            {
                                                //four 8 characters drop vertically from the pipe
                                                y++;
                                                bossmap[y][x] = '8';
                                                y++;
                                                bossmap[y][x] = '8';
                                                y++;
                                                bossmap[y][x] = '8';
                                                y++;
                                                bossmap[y][x] = '8';
                                                y++;
                                                bossmap[y][x] = '8';
                                                y++;
                                                bossmap[y][x] = '8';
                                            }

                                        break; //break to next statement

                                 case '8': //case for the enemy bombs

                                    bossmap[y][x] = ' '; //previous position is replaced with blank space
                                    y++; // 8 moves down in iterations

                                    //if the 8 character is not about hit any of the following characters:
                                    if (bossmap [y][x]!= '_' && bossmap [y][x] != '^' && bossmap[y][x] != '~')
                                    {
                                        bossmap[y][x] = '8'; //replace the character with an 8
                                    }

                                    else if (bossmap[y][x] == '^') //else if the 8 hits your player
                                    {
                                        health -=30; //subtract 30 from your health
                                    }

                                        break; //break to next statement

                    }

                }
            }
                                    if (bosshealth <=0) //if boss health gets to 0
                                    {
                                      system("cls"); //clear the console

                                    for (unsigned int x = 0; x < strlen(bossdes); x++) //writes out the string character by character
                                        {
                                          cout << bossdes[x]; //writes one letter to the console (x direction)
                                          Sleep(100); // pause for 0.1 second
                                        }
                                            string name; //store name
                                            cout <<"\nPlease enter your name:"; //write to console
                                            cin >> name; //stores user input to name string

                                            ofstream myfile ("LeaderBoard.txt"); //opens the leaderboard text file to write

                                            if (myfile.is_open()) //checks if the file is open correctly
                                                {
                                                    myfile << name << " " << score << " Points!" ; //write score and name to the file
                                                    myfile.close(); //close the file
                                                }
                                      bossfinish = true; //set boss finish to true

                                    }

                                    //////////////////////
                                    // GAME OVER SCREEN //
                                    //////////////////////

                                    if (bossfinish == true) //when game is finished
                                            {
                                    cout << endl;
                                    cout <<" CONGRATULATIONS! YOU BEAT THE GAME! "<< endl;
                                    cout <<" _______ _______ _______ _______     "<< endl;
                                    cout <<"|     __|   _   |   |   |    ___|    "<< endl;
                                    cout <<"|    |  |       |       |    ___|    "<< endl;
                                    cout <<"|_______|___|___|__|_|__|_______|    "<< endl;
                                    cout <<"                                     "<< endl;
                                    cout <<" _______ ___ ___ _______ ______      "<< endl;
                                    cout <<"|       |   |   |    ___|   __ |     "<< endl;
                                    cout <<"|   -   |   |   |    ___|      <     "<< endl;
                                    cout <<"|_______||_____||_______|___|__|     "<< endl;
                                    cout <<"                                     "<< endl;
                                    cout <<"                                     "<< endl;
                                    cout <<"LASTEST SCORES:                      "<< endl;

                                      ifstream myfile ("LeaderBoard.txt"); //reads from the leaderboard file
                                      if (myfile.is_open()) //if the file is open correctly
                                      {
                                        {
                                          cout << score << '\n'; //read the player score and write to console
                                        }
                                        myfile.close(); //close the document
                                      }
                                       system ("PAUSE"); //pause the console
                                       return EXIT_SUCCESS; //return the exit status

                                    }
                                    else if (health <=0)
                                    {

                                    system("cls");

                                    cout <<" _______ _______ _______ _______     "<< endl;
                                    cout <<"|     __|   _   |   |   |    ___|    "<< endl;
                                    cout <<"|    |  |       |       |    ___|    "<< endl;
                                    cout <<"|_______|___|___|__|_|__|_______|    "<< endl;
                                    cout <<"                                     "<< endl;
                                    cout <<" _______ ___ ___ _______ ______      "<< endl;
                                    cout <<"|       |   |   |    ___|   __ |     "<< endl;
                                    cout <<"|   -   |   |   |    ___|      <     "<< endl;
                                    cout <<"|_______||_____||_______|___|__|     "<< endl;
                                    cout <<"                                     "<< endl;
                                    cout <<"                                     "<< endl;
                                    cout <<"LASTEST SCORES:                      "<< endl;

                                      ifstream myfile ("LeaderBoard.txt"); //reads from the leaderboard file
                                      if (myfile.is_open()) //if the file is open correctly
                                      {
                                        {
                                          cout << score << '\n'; //read the player score and write to console
                                        }
                                        myfile.close(); //close the document
                                      }
                                       system ("PAUSE"); //pause the console
                                       return EXIT_SUCCESS; //return the exit status
                                    }

            Sleep(speed); //sleep for the set gamespeed
        }
}

