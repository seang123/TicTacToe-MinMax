

#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>
//#include "tictactoe.h"
#include "assert.h"
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;


int grids[8][3] = {
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {0,3,6},
    {1,4,7},
    {2,5,8},
    {0,4,8},
    {2,4,6}};


class Board{

    public:
        vector<char> board;
        char winner = '?';

        Board(){
            board = vector<char> {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
        };
        Board(vector<char> b){
            board = b;
        };

        bool win(char c){
            for(int i=0;i<8;i++){
                if (board[grids[i][0]] == c &&
                    board[grids[i][1]] == c &&
                    board[grids[i][2]] == c ){
                        return true;
                }
            }
            return false;
        };

        bool game_over(){
            if( win('O') ){
                winner = 'O';
                return true;
            }
            else if( win('X') ){
                winner = 'X';
                return true;
            }
            else{
                return board_full();
            }
        };

        bool board_full(){
            bool full = true;
            for(int i = 0; i < 9; i++){
                if (board[i] == '.'){
                    full = false;
                }
            };
            return full;
        };

        void print(){
            // Print a 3x3 board
            for(int i = 0; i < 3; i++){
                cout << " " <<  board[grids[i][0]] << " | " << board[grids[i][1]] << " | " << board[grids[i][2]] << "\n";
            }
            cout << "-----------\n";
        };

        void fill(int x, char c){
            board[x] = c;
        };

        vector<int> possible_moves(char current_player){
            vector<int> moves;
            for(int i = 0; i<9; i++){
                if(board[i] == '.'){
                    moves.push_back(i);
                }
            }
            return moves;
        };

};

struct state{
    Board board;
    int move;
}; 

vector<Board> generate_states(Board board, vector<int> moves, char current_player){
    // Generate the possible next state boards
    vector<Board> children;
    for(int i= 0; i<moves.size();i++){
        vector<char> new_board (board.board);
        new_board[moves[i]] = current_player;
        Board new_board_obj = Board(new_board);
        children.push_back(new_board_obj);
    }
    return children;
};


int minmax_(int depth, Board board, char current_player, int* move_to_make){
    /* Main min-max method
     * Returns the score and updates the move to be made
    */
    char opponent = (current_player == 'X') ? 'O' : 'X';
    if(board.game_over()){
        if (board.winner == 'O'){//(board.winner == current_player){
        //if (board.winner == (board.winner == current_player)){
            return 10-depth;
        }
        else if (board.winner == 'X'){//(board.winner == opponent){
        //else if (board.winner == (board.winner == opponent)){
            return depth-10;
        }
        else{
            return 0;
        }
    }

    //if( depth > 2 ){ return 0;}

    vector<int> scores;
    vector<int> moves = board.possible_moves(current_player);  
    vector<Board> states = generate_states(board.board, moves, current_player);


    for(uint i=0;i<states.size();i++){
        char new_player = (current_player == 'O') ? 'X' : 'O';
        scores.push_back(
                minmax_(depth+1, states[i], new_player, move_to_make));
    }

    /*for(int i=0;i<states.size();i++){
        states[i].print();
        cout << scores[i] << "\n";
        cout << moves[i] << "\n";
        cout << "--------------\n";
    }
    throw 0;*/

    if( current_player == 'O' ){
        int max_idx = 0;
        int max_score = -10000;
        for(uint i=0;i<scores.size();i++){
            if(scores[i] > max_score){
                max_idx = i;
                max_score = scores[i];
            }
        }
        if(depth == 0){
            *move_to_make = moves[max_idx];
        }
        return max_score;
    }
    else{
        int min_idx = 0;
        int min_score = 10000;
        for(uint i=0;i<scores.size();i++){
            if(scores[i] < min_score){
                min_idx = i;
                min_score = scores[i];
            };
        }
        if(depth == 0){
            *move_to_make = moves[min_idx];
        }
        return min_score;
    }
};

int* minmax(int depth, Board board, char current_player){
    /* Wrapper around min-max method
     *
     * Creates a pointer to a integer. This memory address gets updated by the min-max method and 
     * indicates the best move to make for the player
     * This is super hacky. Would be better to return a wrapperd score/move combo
    */
    int* move_to_make = new int;
    int score = minmax_(depth, board, current_player, move_to_make);
    cout << "Player: " << current_player << " move: " << *move_to_make << " score: " << score << "\n";

    return move_to_make;
};

class PlayerHuman{
    // Human player
    // Asks for x,y coordinate input each move
    public:
        char symbol;
        Board* board;
        PlayerHuman(Board* b, char s){
            board = b;
            symbol = s;
        };

        void move(){
            // Make a move
            int x;
            int y;
            cout << "> x,y\n";
            cin >> x >> y;
            board->fill(((x*3)+y), symbol);
        };
};

class Player{
    // Computer Player
    public:
        char symbol;
        Board* board;
        Player(Board* b, char s){
            board = b;
            symbol = s;
        };


        void move(){
            int* move_to_make  = minmax(0, *board, symbol);
            board->fill(*move_to_make, symbol);
            delete move_to_make;
        };

};


int main(){
    cout << "--==--\n";

    vector<char> test_board = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};

    Board board = Board(test_board);
    //PlayerHuman p1 = PlayerHuman(&board, 'O');
    Player p1 = Player(&board, 'O');
    Player p2 = Player(&board, 'X');

    while( true ){
        if( !board.game_over() ){
            p1.move();
            board.print();
        }else{
            cout << "Player " << board.winner << " wins\n";
            break;
        }
        if( !board.game_over() ){
            p2.move();
            board.print();
        }else{
            cout << "Player " << board.winner << " wins\n";
            break;
        }
    }

    return 0;
};
