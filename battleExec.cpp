/** -----------------------------------------------------------------------------
*
* File Name: battleExec.cpp
* Author: Blake Morrell
* KU ID: 2866535
* Email Adress: blknm4598@ku.edu
* Assignment:   EECS-448 Project Battleship
* Description:  Handles User input for how many ships
* Date: 9-12-19
*
---------------------------------------------------------------------------- */
#include "battleExec.h"
#include <string>
#include <cctype>
#include <limits>
#include <iostream>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;

Executive::Executive()
{
    ship_num = 0; //sets starting ship amount to zero
    srand(time(NULL));
    setTargetCoordinates();
}

void Executive::run()
{
  selectGame();
  if (player_num == 1) {
    runPvAI();
  } else if (player_num == 2) {
    runPvP();
  }
}

void Executive::runPvP()
{
  startMenu(); //opens start menu

  CalculateWinHits(ship_num);//ship_num is inputted by user, this function is called to find total hits to win game

  /*
    Determine number of powerups desired for the game.
  */
  cout << "Enter number of power ups desired (Up to 64 allowed): ";
  int numPowerUps = 0;
  cin >> numPowerUps;
  while(1)
  {
    if(cin.fail())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      cout << "\n\nFor number of power ups, please enter a number between 0 and 64: "; //if not an int, must try again.
      cin >> numPowerUps;
      LootBoard.powerUpSetter(numPowerUps);
    }
    else
    {
      LootBoard.powerUpSetter(numPowerUps);
      break;
    }
  }

  HumanPlayer player1; //creates player 1 with the number of ships obtained from input
  player1.setNum(ship_num);
  cout<<"\n---------PLAYER 1----------\n\n";
  player1.print();
  while(!player1.isAllMarked())
  {
    cout<<"\nPick which column (A-H) and row (1-8) (Must be in the form [col][row] i.e.: A1): ";
    cin>>location;
    while(!transfor(location))
    {
      cout<<"Invalid Position. Try again: ";
      cin>>location;
    }
    cout<<"\nWhich direction will your ship face?\n";
    cout<<"Choose 1 for up, 2 for down, 3 for left, or 4 for right: ";
    cin>>dir;
    while(1) //checking for right input
    {
      if(cin.fail())
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
        cin >> dir;
      }
      else
      {
        break;
      }
    }
    while((dir < 1) || (dir > 4)) //error checking
    {
      cout<<"You must enter a number between 1 and 4. Try again: ";
      cin>>dir;
      while(1) //checking for right input
      {
        if(cin.fail())
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(),'\n');
          cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
          cin >> dir;
        }
        else
        {
          break;
        }
      }
    }
    try
    {
      player1.placement(row, col, dir);
      player1.print();
    }
    catch (const std::runtime_error &e)
    {
      cout << "enter a correct direction!" << endl;
    }
  }

    cout << "\nTYPE anything THEN PRESS ENTER to place Player2 Ships\n";
    string dummy;
    cin >> dummy;
    ClearScreen();

    HumanPlayer player2; //creates player 1 with the number of ships obtained from input
    player2.setNum(ship_num);
    cout<<"\n---------PLAYER 2----------\n\n";
    player2.print();

    while(!player2.isAllMarked())
    {
      cout<<"\nPick which column (A-H) and row (1-8) (Must be in the form [col][row] i.e.: A1): ";
      cin>>location;
      while(!transfor(location))
      {
        cout<<"Invalid Position. Try again: ";
        cin>>location;
      }
      cout<<"\nWhich direction will your ship face?\n";
      cout<<"Choose 1 for up, 2 for down, 3 for left, or 4 for right: ";
      cin>>dir;
      while(1) //checking for right input
      {
        if(cin.fail())
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(),'\n');
          cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
          cin >> dir;
        }
        else
        {
          break;
        }
      }
      while((dir < 1) || (dir > 4)) //error checking
      {
        cout<<"You must enter a number between 1 and 4. Try again: ";
        cin>>dir;
        while(1) //checking for right input
        {
          if(cin.fail())
          {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
            cin >> dir;
          }
          else
          {
            break;
          }
        }
      }
      try
      {
        player2.placement(row, col, dir);
        player2.print();
      }
      catch (const std::runtime_error &e)
      {
        cout << "enter a correct direction!" << endl;
      }
    }

    cout << "Setting player inventories...\n\n";
    player1.setInventoryFile("player1_powerUps.txt");
    player2.setInventoryFile("player2_powerUps.txt");
    player1.loadInventory();
    player2.loadInventory();


  cout << "\nATTACK phase, TYPE anything and PRESS ENTER to begin -> \n";
  cin >> dummy;
  ClearScreen();

  while(player1.getHits() != win_hits && player2.getHits() != win_hits)   /////add while loop to check win condition
  { 
    string shotType = "";
    string x;
    cout<<"\n---------PLAYER 1----------\n\n";
    cout << "Would you like to use a power up? (y/n): ";
    cin >> x;
    if(x == "y")
    {
      if(player1.inventorySelect(shotType))
      {
        cout << "Player 1 shotType is: " << shotType << "\n";
        p1PowerShot(player1, player2, shotType);

        if (player2.getHits() == win_hits)
        {
          cout<<"\nPLAYER 1 WINS. Thanks for playing!\n";
          break;
        }
      }
      else
      {
        p1StandardShot(player1, player2);
        if (player2.getHits() == win_hits)
        {
          cout<<"\nPLAYER 1 WINS. Thanks for playing!\n";
          break;
        }
      }      
    }
    else
    {
      p1StandardShot(player1, player2);
      if (player2.getHits() == win_hits)
      {
        cout<<"\nPLAYER 1 WINS. Thanks for playing!\n";
        break;
      }
    }

    cout<<"\n---------PLAYER 2----------\n\n";
    cout << "Would you like to use a power up? (y/n): ";
    shotType = "";
    cin >> x;
    if(x == "y")
    {
      if(player2.inventorySelect(shotType))
      {
        cout << "Player 2 shotType is: " << shotType << "\n";
        p2PowerShot(player2, player1, shotType);

        if (player1.getHits() == win_hits)
        {
          cout<<"\nPLAYER 2 WINS. Thanks for playing!\n";
          break;
        }
      }
      else
      {
        p2StandardShot(player2, player1);
        if (player1.getHits() == win_hits)
        {
          cout<<"\nPLAYER 2 WINS. Thanks for playing!\n";
          break;
        }
      }      
    }
    else
    {
      p2StandardShot(player2, player1);
      if (player1.getHits() == win_hits)
      {
        cout<<"\nPLAYER 2 WINS. Thanks for playing!\n";
        break;
      }
    }
    

    
    

    
    /*
     string x;

      cout<<"\n---------PLAYER 1----------\n\n";

        player1.printHidden();
        player1.print();
        LootBoard.displayPowerUps();
        cout << "Would you like to use a power up? (y/n): ";
        cin >> x;
        if(x == "y")
        {
          player1.inventorySelect();
        }

      cout << "\nEnter attack coordinates (A-H),(1-8) (i.e. A1): ";
      cin >> x;
      while(!transfor(x))
      {
        // cout <<x;
        // cout << row <<" " <<col;
        cout<<"Invalid position. Try again: ";
        cin>>x;
      }
      while(player1.hitRetry(row, col))
      {
        cout<<"You've already tried that spot before! Try again: ";
        cin>>x;
        while(!transfor(x))
        {
          // cout <<x;
          // cout << row <<" " <<col;
          cout<<"Invalid position. Try again: ";
          cin>>x;
        }
      }
      if(player2.attack(row,col)) //here we want to change map
      {
        player1.update(row,col, true); //here, we want to only update grid

        //if(player2.getHits() == win_hits) break;
      }
      else   {
        player1.update(row,col, false);

      }

      player1.printHidden();
      player1.print();
      if(LootBoard.update(row, col))
      {
        cout << "\n\nLOOT FOUND!\n\n";
        player1.inventoryRoll();
      }


      if (player2.getHits() == win_hits)
      {
        cout<<"\nPLAYER 1 WINS. Thanks for playing!\n";
        break;
      }

      cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to SWITCH players -> \n";
      cin >> dummy;
      ClearScreen();

      cout<<"\n---------PLAYER 2----------\n\n";

      player2.printHidden();
      player2.print();
      LootBoard.displayPowerUps();
      cout << "Would you like to use a power up? (y/n): ";
      cin >> x;
      if(x == "y")
      {
        player2.inventorySelect();
      }

      cout <<"\nEnter attack coordinates (A-H),(1-8) (i.e. A1): ";
      cin >>x;

      while(!transfor(x))
      {
        cout<<"Invalid position. Try again: ";
        cin>>x;
      }
      while(player2.hitRetry(row, col))
      {
        cout<<"You've already tried that spot before! Try again: ";
        cin>>x;
        while(!transfor(x))
        {
          // cout <<x;
          // cout << row <<" " <<col;
          cout<<"Invalid position. Try again: ";
          cin>>x;
        }
      }

      if(player1.attack(row,col)) //here we want to change map
      {
        player2.update(row,col, true); //here, we want to only update grid
        //if(player1.GetHits() == win_hits) break;
      }
      else {
        player2.update(row,col, false);

      }


      player2.printHidden();
      player2.print();
      if(LootBoard.update(row, col))
      {
        cout << "\n\nLOOT FOUND!\n\n";
        player2.inventoryRoll();
      }

      if (player1.getHits() == win_hits)
      {
        cout<<"\nPLAYER 2 WINS. Thanks for playing!\n";
        break;
      }

      cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to SWITCH players -> \n";
      //cin >> dummy;
      ClearScreen();
      */
  }  
}




















void Executive::runPvAI()
{
  srand(time(NULL));
  // AI difficulty setting could start either before startMenu() or after player setup, but AI set up must be done after player setup.

  startMenu(); //opens start menu

  CalculateWinHits(ship_num);//ship_num is inputted by user, this function is called to find total hits to win game

  HumanPlayer player; //creates player object
  player.setNum(ship_num); //sets number of ships for game from input
  AI ai;
  ai.setNum(ship_num);

  setAIDifficulty(ai); //sets AI difficulty

  cout<<"\n----------PLAYER-----------\n\n";
  player.print();
  //playerSetup(player);
  while(!player.isAllMarked())
  {
    cout<<"\nPick which column (A-H) and row (1-8) (Must be in the form [col][row] i.e.: A1): ";
    cin >> location;
    while(!transfor(location))
    {
      cout<<"Invalid Position. Try again: ";
      cin >> location;
    }
    cout<<"\nWhich direction will your ship face?\n";
    cout<<"Choose 1 for up, 2 for down, 3 for left, or 4 for right: ";
    cin>>dir;
    while(1) //checking for right input
    {
      if(cin.fail())
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
        cin >> dir;
      }
      else
      {
        break;
      }
    }
    while((dir < 1) || (dir > 4)) //error checking
    {
      cout<<"You must enter a number between 1 and 4. Try again: ";
      cin>>dir;
      while(1) //checking for right input
      {
        if(cin.fail())
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(),'\n');
          cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
          cin >> dir;
        }
        else
        {
          break;
        }
      }
    }
    try
    {
      player.placement(row, col, dir);
      player.print();
    }
    catch (const std::runtime_error &e)
    {
      cout << "enter a correct direction!" << endl;
    }
  }


  ClearScreen();

  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n******************************************\n"
       << "  WAITING FOR AI TO FINISH PLACING SHIPS\n"
       << "******************************************\n\n\n\n\n\n\n\n\n\n";


  int tempRow, tempCol;
  while(!ai.isAllMarked())
  {
    row = rand() % 8 * 6 % 8; //generates random number between 0-7
    col = rand() % 8 * 5 % 8; //generates random number between 0-7
    dir = rand() % 4 + 1; //generates random number between 1-4

    if(tempRow != row && tempCol != col)
    {
      tempRow = row;
      tempCol = col;

      try
      {
        ai.placement(row, col, dir);
      }
      catch (const std::runtime_error &e)
      {

      }
    }
  }

  while(player.getHits() != win_hits && ai.getHits() != win_hits)
  {
    string dummy;
    string x;

     cout << "\n----------PLAYER-----------\n\n";

       player.printHidden();
       player.print();
       //displayPowerUps();

     cout << "\nEnter attack coordinates (A-H),(1-8) (i.e. A1): ";
     cin >> x;
     while(!transfor(x))
     {
       // cout <<x;
       // cout << row <<" " <<col;
       cout << "Invalid position. Try again: ";
       cin >> x;
     }
     while(player.hitRetry(row, col))
     {
       cout<<"You've already tried that spot before! Try again: ";
       cin>>x;
       while(!transfor(x))
       {
         // cout <<x;
         // cout << row <<" " <<col;
         cout << "Invalid position. Try again: ";
         cin >> x;
       }
     }

    if(ai.attack(row,col)) //here we want to change map
    {
      player.update(row,col, true); //here, we want to only update grid

      //if(ai.getHits() == win_hits) break;
    }
    else
    {
      player.update(row,col, false);
    }

    if(ai.getHits() == win_hits)
    {
      cout << "\nPLAYER WINS. Thanks for playing!\n";
      break;
    }

    cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to let the AI attack\n";
    cin >> dummy;

    if(ai.getDifficulty() == "Easy" && transfor(targetCoordinates[turn]))
    {
      easyAIAttack(ai, player, row, col);

      if (player.getHits() == win_hits)
      {
        cout<<"\nAI WINS. Sorry human :(\n";
        break;
      }
    }

    else if(ai.getDifficulty() == "Medium")
    {

      mediumAIAttack(ai, player, row, col, turn);

      if (player.getHits() == win_hits)
      {
        cout<<"\nAI WINS. Sorry human :(\n";
        break;
      }
    }

    else if(ai.getDifficulty() == "Hard")
    {
      hardAIAttack(ai, player, row, col, turn);

      if (player.getHits() == win_hits)
      {
        cout<<"\nAI WINS. Sorry human :(\n";
        break;
      }
    }
  }
}

Executive::~Executive()
{

}

void Executive::selectGame()
{
  cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
       << "x                  Battleship                x\n"
       << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n"
       << "Player versus AI (1 player)\n"
       << "Player versus Player (2 players)\n\n"
       << "Enter the number of players for this game: ";
  cin >> player_num;
  while(1) //checking for right input
  {
    if(cin.fail())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
      cin >> player_num;
    }
    else
    {
      break;
    }
  }
  while((player_num < 1) || (player_num > 2))
  {
    cout << "You must enter a number between 1 and 2, try again.\n\n"
         << "Player versus AI (1 player)\n"
         << "Player versus Player (2 players)\n\n"
         << "Enter the number of players for this game: ";
    cin >> player_num;
    while(1) //checking for right input
    {
      if(cin.fail())
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
        cin >> player_num;
      }
      else
      {
        break;
      }
    }
  }
}

void Executive::startMenu()
{
  cout << "Enter the number of ships for this game (You may have up to 5 ships): ";
  cin >> ship_num;
  while(1) //checking for right input
  {
    if(cin.fail())
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
      cin >> ship_num;
    }
    else
    {
      break;
    }
  }
  while((ship_num < 1) || (ship_num > 5))
  {
    cout << "You must enter a number between 0 and 5, try again.\n"
         << "Enter the number of ships for this game (You may have up to 5 ships): ";
    cin >> ship_num;
    while(1) //checking for right input
    {
      if(cin.fail())
      {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
        cin >> ship_num;
      }
      else
      {
        break;
      }
    }
  }
}

bool Executive::inputChecker(string x)
{
  string y = "ABCDEFGH";
  string z = "abcdefgh";
  string nums = "12345678";
  char first_char = x.at(0);
  char second_char = x.at(1);
  bool flag = false;
  for(int i =0; i< 8; i++)
  {
    if ((first_char == y.at(i) || first_char == z.at(i)))
    {
      for(int j=0; j < 8; j++)
      {
        if(second_char == nums.at(j))
        {
          flag = true;
        }
      }
    }
  }
  return flag;
}
bool Executive::transfor(string x)
{
  //int value = stoi(x.substr(0, 1));
  //cout << value << endl;
  if (x.size() < 2 || x.size() != 2)
  {
    return false;
  }
  else if(!inputChecker(x))
  {
    return false;
  }
  else if (transtoint(x.at(0)) && transfromchar(stoi(x.substr(1, 1))))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Executive::transfromchar(int x)
{
  bool flag = false;
  for (int i = 1; i < 9; i++)
  {
    if (x == i)
    {
      row = i-1;
      flag = true;
    }
  }
  return flag;
}
bool Executive::transtoint(char x)
{
  string y = "ABCDEFGH";
  string z = "abcdefgh";
  bool flag = false;
  for (int i = 0; i < 8; i++)
  {
    if (x == y.at(i)||x==z.at(i))
    {
      col = i;
      flag = true;
    }
  }
  return flag;
}

void Executive::ClearScreen()
{
  cout << "\nPLAYER finishing placing ships in\n";
  usleep(1000000);
  cout << "3\n";
  usleep(1000000);
  cout << "2\n";
  usleep(1000000);
  cout << "1\n";
  usleep(1000000);
  for(int i = 0; i < 20; i++)
  {
    cout << '\n';
  }
}

void Executive::CalculateWinHits(int ship_num)
{
    for(int i = ship_num; i > 0; i--)
    {
        win_hits += i;
    }
}

void Executive::setAIDifficulty(AI& someAI)
{
  int AIDifficulty;

    cout << "\n\n";
    cout << "What difficulty of AI do you wish to play against? \n";
    cout << "1. Easy\n"
         << "2. Medium\n"
         << "3. Hard\n";
    cout << "Choice: ";

    cin >> AIDifficulty;

    while(AIDifficulty < 1 || AIDifficulty > 3)
    {
      cout << "You must enter a number between 1 and 3. Please try again: ";
      cin >> AIDifficulty;

      while(1)
      {
        if(cin.fail())
        {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(),'\n');
          cout << "Bad entry.  Enter a NUMBER: "; //if not an int, must try again.
          cin >> AIDifficulty;
        }
        else
        {
          break;
        }
      }
    }

  someAI.setDifficulty(AIDifficulty);
}

void Executive::setTargetCoordinates()
{
  string letters = "ABCDEFGH";
  string nums = "12345678";
  int letterIndex = 0;
  int numIndex = 0;

  for (int i = 0; i < 64; i++) {
    targetCoordinates[i] = letters.at(letterIndex);
    numIndex++;
    if (numIndex >= 8) {
      numIndex = 0;
      letterIndex++;
    }
  }

  for (int i = 0; i < 64; i++) {
    targetCoordinates[i] = targetCoordinates[i] + nums.at(numIndex);
    numIndex++;
    if (numIndex >= 8) {
      numIndex = 0;
      letterIndex++;
    }
}

  srand(time(0));

  int shuffleNumber = rand() % 1024;

  for (int i = 0; i < shuffleNumber; i++) {
    shuffleCoordinates(targetCoordinates, 64);
  }
}

// shuffle vs random_shuffle in C++
// https://www.geeksforgeeks.org/shuffle-vs-random_shuffle-c/
void Executive::shuffleCoordinates(string arr[], int n)
{
  // Shuffling our array using random_shuffle
  random_shuffle(arr, arr + n);
}

void Executive::printRandomCoordinates()
{
  int index = 0;
  for (int i = 0; i < coordinatesArraySize; i++) {
    cout << targetCoordinates[i] << ", ";
    index++;
    if(index == 8)
    {
      cout << "\n";
      index = 0;
    }
  }
  cout << "\n";
}

void Executive::easyAIAttack(AI& someAI, HumanPlayer& somePlayer, int& row, int& col)
{
  if(somePlayer.attack(row, col)) //here we want to change map
  {
    someAI.update(row,col, true); //here, we want to only update grid

    //if(ai.getHits() == win_hits) break;
  }
  else
  {
    someAI.update(row,col, false);
  }

  this->turn = this->turn + 1;
}

void Executive::mediumAIAttack(AI& someAI, HumanPlayer& somePlayer, int& row, int& col, int& turn)
{
  if(!(someAI.getIsAHit()))
  {
    if(transfor(this->targetCoordinates[turn]))
    {
      while(someAI.hitRetry(row, col))
      {
        turn++;
        transfor(this->targetCoordinates[turn]);
      }
      if(somePlayer.attack(row, col))
      {
        someAI.update(row, col, true);
        someAI.setMediumCol(col);
        someAI.setMediumRow(row);
        someAI.setIsAHit(true);
      }
      else
      {
        someAI.update(row, col, false);
        someAI.setIsAHit(false);
      }
    }
  }

//Priority
//1. Up
//2. Right
//3. Left
//4. Down

  else
  {

    // UP
    if(someAI.getMediumRow() > 0 && !(someAI.hitRetry(someAI.getMediumRow()-1, someAI.getMediumCol())))
    {
      if(somePlayer.attack(someAI.getMediumRow()-1, someAI.getMediumCol()))
      {
        someAI.update(someAI.getMediumRow()-1, someAI.getMediumCol(), true);
        someAI.setMediumRow(someAI.getMediumRow()-1);
        someAI.setMediumCol(someAI.getMediumCol());
        someAI.setIsAHit(true);
      }
      else
      {
        someAI.update(someAI.getMediumRow()-1, someAI.getMediumCol(), false);
      }
    }

    //RIGHT
    else if(someAI.getMediumCol() < 7 && !(someAI.hitRetry(someAI.getMediumRow(), someAI.getMediumCol()+1)))
    {
      if(somePlayer.attack(someAI.getMediumRow(), someAI.getMediumCol()+1))
      {
        someAI.update(someAI.getMediumRow(), someAI.getMediumCol()+1, true);
        someAI.setMediumRow(someAI.getMediumRow());
        someAI.setMediumCol(someAI.getMediumCol()+1);
        someAI.setIsAHit(true);
      }
      else
      {
        someAI.update(someAI.getMediumRow(), someAI.getMediumCol()+1, false);
      }
    }

    //LEFT
    else if(someAI.getMediumCol() > 0 && !(someAI.hitRetry(someAI.getMediumRow(), someAI.getMediumCol()-1)))
    {
      if(somePlayer.attack(someAI.getMediumRow(), someAI.getMediumCol()-1))
      {
        someAI.update(someAI.getMediumRow(), someAI.getMediumCol()-1, true);
        someAI.setMediumRow(someAI.getMediumRow());
        someAI.setMediumCol(someAI.getMediumCol()-1);
        someAI.setIsAHit(true);
      }
      else
      {
        someAI.update(someAI.getMediumRow(), someAI.getMediumCol()-1, false);
      }
    }

    //DOWN
    else if(someAI.getMediumRow() < 7 && !(someAI.hitRetry(someAI.getMediumRow()+1, someAI.getMediumCol())))
    {
      if(somePlayer.attack(someAI.getMediumRow()+1, someAI.getMediumCol()))
      {
        someAI.update(someAI.getMediumRow()+1, someAI.getMediumCol(), true);
        someAI.setMediumRow(someAI.getMediumRow()+1);
        someAI.setMediumCol(someAI.getMediumCol());
        someAI.setIsAHit(true);
      }
      else
      {
        someAI.update(someAI.getMediumRow()+1, someAI.getMediumCol(), false);
      }
    }

    else
    {
      someAI.setIsAHit(false);
      if(transfor(this->targetCoordinates[turn]))
      {
        while(someAI.hitRetry(row, col))
        {
          turn++;
          transfor(this->targetCoordinates[turn]);
        }
        if(somePlayer.attack(row, col))
        {
          someAI.update(row, col, true);
          someAI.setMediumCol(col);
          someAI.setMediumRow(row);
          someAI.setIsAHit(true);
        }
        else
        {
          someAI.update(row, col, false);
          someAI.setIsAHit(false);
        }
      }
    }
  }
}

void Executive::hardAIAttack(AI& someAI, HumanPlayer& somePlayer, int& row, int& col, int& turn)
{
  int tempTurn = 0;
  turn = 0;
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(somePlayer.getBoard().getMap()[i][j] != '~' && somePlayer.getBoard().getMap()[i][j] != '*')
      {
        if(tempTurn == turn)
        {
          row = i;
          col = j;
          break;
        }
        else
        {
          tempTurn++;
        }
      }
    }
  }
  if(somePlayer.attack(row, col)) //here we want to change map
  {
    turn++;
    someAI.update(row,col, true); //here, we want to only update grid

    //if(ai.getHits() == win_hits) break;
  }
  else
  {
    someAI.update(row,col, false);
  }

}

void Executive::p1StandardShot(HumanPlayer& player1, HumanPlayer& player2)
{
  string dummy;

    player1.printHidden();
    player1.print();
    LootBoard.displayPowerUps();

  p1InputCheck(player1, player2);

  if(player2.attack(row,col)) //here we want to change map
  {
    player1.update(row,col, true); //here, we want to only update grid

    //if(player2.getHits() == win_hits) break;
  }
  else   {
    player1.update(row,col, false);

  }

  player1.printHidden();
  player1.print();
  if(LootBoard.update(row, col))
  {
    cout << "\n\nLOOT FOUND!\n\n";
    player1.inventoryRoll();
  }

  
  if (player2.getHits() == win_hits)
  {
    return;
  }
  

  cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to SWITCH players -> \n";
  cin >> dummy;
  ClearScreen();
}

void Executive::p2StandardShot(HumanPlayer& player2, HumanPlayer& player1)
{
  string dummy;

  player2.printHidden();
  player2.print();
  LootBoard.displayPowerUps();

  p2InputCheck(player2, player1);

  if(player1.attack(row,col)) //here we want to change map
  {
    player2.update(row,col, true); //here, we want to only update grid
    //if(player1.GetHits() == win_hits) break;
  }
  else {
    player2.update(row,col, false);

  }


  player2.printHidden();
  player2.print();
  if(LootBoard.update(row, col))
  {
    cout << "\n\nLOOT FOUND!\n\n";
    player2.inventoryRoll();
  }
  
  if (player1.getHits() == win_hits)
  {
    return;
  }
  

  cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to SWITCH players -> \n";
  cin >> dummy;
  ClearScreen();
}

void Executive::p1PowerShot(HumanPlayer& player1, HumanPlayer& player2, string shotType)
{
  cout << ">>>>>>>>>>PLAYER 1 POWERSHOT CALLED<<<<<<<<<<\n";
  cout << "shotType: " << shotType << "\n\n";
  string dummy;

    player1.printHidden();
    player1.print();
    LootBoard.displayPowerUps();

  p1InputCheck(player1, player2);

  /*
    Here we select and implement power shots
  */

  if(shotType == "Bomb")
  {
    bombShot(player1, player2, row, col);
  }
  else if(shotType == "Cross")
  {
    crossShot(player1, player2, row, col);

  }
  else if(shotType == "Vertical")
  {
    verticalShot(player1, player2, row, col);

  }
  else if(shotType == "Horizontal")
  {
    horizontalShot(player1, player2, row, col);

  }
  else if(shotType == "Nuke")
  {
    nukeShot(player1, player2, row, col);

  }
  else
  {
    cout << "Invalid shotType...\n\n";
  }
  


  /*if(player2.attack(row,col)) //here we want to change map
  {
    player1.update(row,col, true); //here, we want to only update grid

    //if(player2.getHits() == win_hits) break;
  }
  else   {
    player1.update(row,col, false);

  }
  */

   /*
    End selecttion and implementation of power shots
  */

  player1.printHidden();
  player1.print();

  /*
  if(LootBoard.update(row, col))
  {
    cout << "\n\nLOOT FOUND!\n\n";
    player1.inventoryRoll();
  }
  */

  if (player2.getHits() == win_hits)
  {
    return;
  }
  
  cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to SWITCH players -> \n";
  cin >> dummy;
  ClearScreen();
}

void Executive::p2PowerShot(HumanPlayer& player2, HumanPlayer& player1, string shotType)
{
  cout << ">>>>>>>>>>PLAYER 2 POWERSHOT CALLED<<<<<<<<<<\n";
  cout << "shotType: " << shotType << "\n\n";

  string dummy;

  player2.printHidden();
  player2.print();
  LootBoard.displayPowerUps();

  p2InputCheck(player2, player1);

  if(shotType == "Bomb")
  {
    bombShot(player2, player1, row, col);
  }
  else if(shotType == "Cross")
  {
    crossShot(player2, player1, row, col);
  }
  else if(shotType == "Vertical")
  {
    verticalShot(player2, player1, row, col);
  }
  else if(shotType == "Horizontal")
  {
    horizontalShot(player2, player1, row, col);
  }
  else if(shotType == "Nuke")
  {
    nukeShot(player2, player1, row, col);
  }
  else
  {
    cout << "Invalid shotType...\n\n";
  }


  player2.printHidden();
  player2.print();

  /*
  if(LootBoard.update(row, col))
  {
    cout << "\n\nLOOT FOUND!\n\n";
    player2.inventoryRoll();
  }
  */
  
  if (player1.getHits() == win_hits)
  {
    return;
  }
  
  cout << "\nEND OF TURN, TYPE anything and PRESS ENTER to SWITCH players -> \n";
  cin >> dummy;
  ClearScreen();
}

void Executive::p1InputCheck(HumanPlayer& player1, HumanPlayer& player2)
{
  string x;
  cout << "\nEnter attack coordinates (A-H),(1-8) (i.e. A1): ";
  cin >> x;
  while(!transfor(x))
  {
    // cout <<x;
    // cout << row <<" " <<col;
    cout<<"Invalid position. Try again: ";
    cin>>x;
  }
  while(player1.hitRetry(row, col))
  {
    cout<<"You've already tried that spot before! Try again: ";
    cin>>x;
    while(!transfor(x))
    {
      // cout <<x;
      // cout << row <<" " <<col;
      cout<<"Invalid position. Try again: ";
      cin>>x;
    }
  }
}

void Executive::p2InputCheck(HumanPlayer& player2, HumanPlayer& player1)
{
  string x;
  cout <<"\nEnter attack coordinates (A-H),(1-8) (i.e. A1): ";
  cin >>x;

  while(!transfor(x))
  {
    cout<<"Invalid position. Try again: ";
    cin>>x;
  }
  while(player2.hitRetry(row, col))
  {
    cout<<"You've already tried that spot before! Try again: ";
    cin>>x;
    while(!transfor(x))
    {
      // cout <<x;
      // cout << row <<" " <<col;
      cout<<"Invalid position. Try again: ";
      cin>>x;
    }
  }
}

void Executive::bombShot(HumanPlayer& player1, HumanPlayer& player2, int coordinateR, int coordinateC)
{
  int row;
  int col;

  for (int i = -2; 2 >= i; i++)
	{
		for (int j = -2; 2 >= j; j++)
		{
      row = coordinateR + i;
      col = coordinateC + j;
			if (checkFirstAxis(row) && checkSecondAxis(col))
      {
        if(player2.attack(row,col)) //here we want to change map
        {
          player1.update(row,col, true); //here, we want to only update grid

          //if(player2.getHits() == win_hits) break;
        }
        else   
        {
          player1.update(row,col, false);

        }
      }
		}
	} 
}

void Executive::crossShot(HumanPlayer& player1, HumanPlayer& player2, int coordinateR, int coordinateC)
{
  int row;
  int col;

  for (int j = -8; 8 >= j; j++)
  {
    row = coordinateR + j;
    col = coordinateC;
    if (checkFirstAxis(row) && checkSecondAxis(col))
    {
      if(player2.attack(row,col)) //here we want to change map
      {
        player1.update(row,col, true); //here, we want to only update grid

        //if(player2.getHits() == win_hits) break;
      }
      else   
      {
        player1.update(row,col, false);

      }      
    }
    row = coordinateR;
      col = coordinateC + j;
      if (checkFirstAxis(row) && checkSecondAxis(col))
      {
        if(player2.attack(row,col)) //here we want to change map
        {
          player1.update(row,col, true); //here, we want to only update grid

          //if(player2.getHits() == win_hits) break;
        }
        else   
        {
          player1.update(row,col, false);

        }
      }
  } 
}

void Executive::verticalShot(HumanPlayer& player1, HumanPlayer& player2, int coordinateR, int coordinateC)
{
  int row;
  int col;

  for (int j = -8; 8 >= j; j++)
  {
    row = coordinateR + j;
    col = coordinateC;
    if (checkFirstAxis(row) && checkSecondAxis(col))
    {
      if(player2.attack(row,col)) //here we want to change map
      {
        player1.update(row,col, true); //here, we want to only update grid

        //if(player2.getHits() == win_hits) break;
      }
      else   
      {
        player1.update(row,col, false);

      }      
    }
  }
}

void Executive::horizontalShot(HumanPlayer& player1, HumanPlayer& player2, int coordinateR, int coordinateC)
{
  int row;
  int col;

  for (int j = -8; 8 >= j; j++)
  {
    row = coordinateR;
    col = coordinateC + j;
    if (checkFirstAxis(row) && checkSecondAxis(col))
    {
      if(player2.attack(row,col)) //here we want to change map
      {
        player1.update(row,col, true); //here, we want to only update grid

        //if(player2.getHits() == win_hits) break;
      }
      else   
      {
        player1.update(row,col, false);

      }      
    }
  }
}

void Executive::nukeShot(HumanPlayer& player1, HumanPlayer& player2, int coordinateR, int coordinateC)
{
  for (int i = 0; 7 >= i; i++)
	{
		for (int j = 0; 7 >= j; j++)
		{
			if(player2.attack(i,j)) //here we want to change map
      {
        player1.update(i,j, true); //here, we want to only update grid

        //if(player2.getHits() == win_hits) break;
      }
      else   
      {
        player1.update(i,j, false);

      }
		}
	}
}



bool Executive::checkFirstAxis(int Check)
{
	if (Check == 0 ||
		Check == 1 ||
		Check == 2 ||
		Check == 3 ||
		Check == 4 ||
		Check == 5 ||
		Check == 6 ||
		Check == 7)
		return true;
	else
		return false;
}


bool Executive::checkSecondAxis(int Check)
{
	if (Check == 0 ||
		Check == 1 ||
		Check == 2 ||
		Check == 3 ||
		Check == 4 ||
		Check == 5 ||
		Check == 6 ||
		Check == 7)
		return true;
	else
		return false;
}