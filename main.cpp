
/* coinswap.cpp
 Make moves to solve the puzzle where the objective is to swap the place of
 the pieces on either side of the board.  X can only move to the right into
 an empty square, or can jump to the right over an O into an empty square.
 Conversely O can only move to the left into an empty square, or can jump
 to the left over an X into an empty square.
 */

#include <iostream>
using namespace std;

//Global constants
const int BoardSize=5;

//Struct used to store game information
struct listInfo{
    char board[BoardSize+1];
    int moveNum;
    listInfo *pNext;
};


//--------------------------------------------------------------------------------

// Display instructions
void displayInstructions()
{
    cout << "Welcome to the coin swap puzzle.                                  \n"
    << "Make moves to solve the puzzle where the objective is to swap the \n"
    << "place of the pieces on either side of the board.  X can only move \n"
    << "to the right into an empty square, or can jump to the right over  \n"
    << "an O into an empty square. Conversely O can only move to the left \n"
    << "into an empty square, or can jump to the left over an X into an   \n"
    << "empty square.  \n"
    << " \nEnter 'x' to exit the game.\n"
    << "Enter 'u' if you wish to undo your most recent move.\n"
    << " \n"
    << "For each move enter the source (1..5) and destination (1..5).     "
    <<  "\n";
}//end displayInstructions()


//--------------------------------------------------------------------------------
// See if board pieces have finished being swapped.  This is the case when
// the left-most two pieces are both 'O' and the right-most two pieces are 'X'
bool notDone( char board[])
{
    return board[0]!='O' || board[1]!='O' || board[3]!='X' || board[4]!='X';
}//End of notDone


//--------------------------------------------------------------------------------

void displayList(listInfo *pHead){
    listInfo *pTemp = pHead;
    cout << "List: ";
    while(pTemp != NULL){
        cout << "->" << pTemp->moveNum << "";
        pTemp = pTemp->pNext;
    }
    cout << "\n";
}//End of displayList

void displayBoard( char board[])
{
    cout << endl;
    cout << "   1 2 3 4 5\n";
    cout << "   ";
    for( int i=0; i<BoardSize; i++) {
        cout << board[ i] << " ";
    }
    cout << "\n";
    cout << endl;
}//End of displayBoard


//--------------------------------------------------------------------------------
void promptForAndGetMove( char board[], int &moveNumber, int &source, int &destination, listInfo *&pHead)
{
    char userInput;
    listInfo *pTemp;
    
    // Infinite loop to handle possible multiple undo of moves
    while( true) {
        cout << moveNumber << ". Enter source and destination: ";
        cin >> userInput;
        // See if user input of 0 was given to exit the program
        if( userInput == 'x') {
            cout << "\n";
            cout << "Exiting program...\n";
            exit( 0);
        }
        if( userInput == 'u') {
            if(pHead->pNext != NULL){
            moveNumber--;
            pTemp = pHead;
            pHead = pHead->pNext;
            delete pTemp;
            
            for(int i = 0; i < 6; i++){
                board[i] = pHead->board[i];
            }
        }//End of if
            
            else{
             cout << "Error. You can not go past the beginning of the game.\n";
            }//End of else
            
            
            displayBoard( board);
            displayList(pHead);
            continue;   // Prompt to retry move
        }
        // Input is likely numeric and is not 'x' or 'u'.  Convert to a number.
        source = userInput - '0';
        
        // Also get destination portion of user input
        cin >> destination;
        
        // Adjust user entry to compensate for 0-based indexing, rather than 1-based
        source--;
        destination--;
        
        // break out of enclosing loop, as we don't need to undo a move
        break;
    }
}//end promptForAndGetMove()

//--------------------------------------------------------------------------------
bool moveNotValid(
                  char board[],
                  int source,
                  int destination)
{
   /*
    THIS IS THE STATEMENT THAT WAS CAUSING ISSUES FOR ME. CHECK ABOVE FOR EXPLANATION.
    -------------------------------------------------------------------------
    //Checking source and destination positions
    if((source > 5 || source < 1) || (destination > 5 || destination < 1)){
        return true;
    }
    -------------------------------------------------------------------------
    AS MENTIONED ABOVE, EVERY ERROR CHECK WORKS WITHOUT THIS STATEMENT. HOWEVER, IF IT DOES NOT WORK FOR YOU,
    THEN REMOVE THE COMMENTS AND USE THIS STATEMENT.
    */
    
    //Check if destination square is empty
    if(board[destination] != ' '){
        return true;
    }
    
    //Make sure that 'X' pieces can only move to the left
    if(board[source] == 'X' && destination < source){
        return true;
    }
    
    //Make sure that 'O' pieces can only move to the right
    if(board[source] == 'O' && destination > source){
        return true;
    }
    
    //Destination must be adjacent or one square away
    if(destination > source + 2 || destination < source - 2){
        return true;
    }
    
    //Jumped piece may only be an opponent's piece
    if(board[source] == 'X' && board[source+1] != 'O'){
        if(board[source+1] == ' '){
            return false;
        }
        return true;
    }
    
    if(board[source] == 'O' && board[source-1] != 'X'){
        if(board[source-1] == ' '){
            return false;
        }
        return true;
    }

    return false;
}//End of moveNotValid


//--------------------------------------------------------------------------------

int main()
{
    char board[ BoardSize + 1] = "XX OO";  // extra character for the NULL
    int source, destination;
    int moveNumber = 1;
    listInfo *pHead = NULL;
    listInfo *pTemp;
    
    //Our base condtion. Used to ensure that player can not undo past the first move.
    pTemp = new listInfo;
    
    for(int i = 0; i < 6; i++){
        pTemp->board[i] = board[i];
    }
    
    pTemp->moveNum = moveNumber;
    pTemp->pNext = pHead;
    pHead = pTemp;
    
    // Display identifying information, the instructions, and the initial board, and initial list.
    displayIdentifyingInformation();
    displayInstructions();
    displayBoard( board);
    displayList(pHead);
    
    // Game play loop
    while( notDone( board) ) {
        promptForAndGetMove( board, moveNumber, source, destination, pHead);
        if( moveNotValid( board, source, destination)) {
            cout << "Invalid move, please retry. \n";
            continue;
        }
        
        // Make move.  Note that no move validation is being done.
        board[ destination] = board[ source];
        board[ source] = ' ';
        moveNumber++;
        
        //Prepending node on linked list
        pTemp = new listInfo;
        
        for(int i = 0; i < 6; i++){
            pTemp->board[i] = board[i];
        }
        
        pTemp->moveNum = moveNumber;
        pTemp->pNext = pHead;
        pHead = pTemp;

        //Displaying board and list each turn.
        displayBoard( board);
        displayList(pHead);
        
        
    }//End of game while loop
    
    cout << "Congratulations, you did it! \n"
    << "\n"
    << "Exiting program ...\n";
    return 0;         // return value to keep C++ happy
}//end main()
