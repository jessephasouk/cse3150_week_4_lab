#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

void write_board_csv(const vector<vector<int>>& board, bool first) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void print_board(const vector<vector<int>>& board, bool first) {
    // TODO: implement print_board here
	for(auto& row: board){
		for(auto value: row){
			cout << value << "\t";
		}
		cout << "\n";
	}
    	write_board_csv(board, first);
}

// TODO: use algorithms to spawn properly
void spawn_tile(vector<vector<int>>& board) {
	vector<pair<int,int>> empties;
	for (int r=0;r<4;r++){
        	for (int c=0;c<4;c++){
			if (board[r][c]==0) {
				empties.push_back({r,c});
			}
		}
	}
	if (empties.empty()) {
		return;
	}
	static random_device rd;
	static mt19937 gen(rd());

	uniform_int_distribution<> dist_idx(0, empties.size() - 1);
	auto [r, c] = empties[dist_idx(gen)];
       	
	std::uniform_int_distribution<> dist_tile(0, 9);
    	int roll = dist_tile(gen);
    	board[r][c] = (roll == 0) ? 4 : 2;
}


// TODO: Compress a row, remove zeroes, and then pad with zeroes at the end
std::vector<int> compress_row(const std::vector<int>& row) {
	vector<int> compressed;

	copy_if(row.begin(), row.end(), std::back_inserter(compressed), [](int x) {return x != 0;});

	while(compressed.size() < 4) {
		compressed.push_back(0);
	}
	return compressed;
}
// TODO: Merge a row (assumes the row is already compressed)
std::vector<int> merge_row(std::vector<int> row) {
	for (int i = 0; i<3; ++i){
		if (row[i] == row[i+1] && row[i] != 0) {
			row[i] *= 2;
			row[i+1] = 0;
			i++;
			
		}
	}	
	return compress_row(row);
}
// TODO: use copy_if and iterators
bool move_left(vector<vector<int>>& board) {
	bool mv = false;
	for (int i = 0; i < 4; ++i){
		auto old_row = board[i];
		auto new_row = compress_row(old_row);
		new_row = merge_row(new_row);
		if(old_row != new_row){
			mv = true;
			board[i] = new_row;
		}
	}
	return mv;
}

// TODO: use reverse iterators
bool move_right(vector<vector<int>>& board){
	bool mv = false;
	for(int i = 0; i<4; ++i){
		auto old_row = board[i];
		vector<int> reversed = old_row;
		//reverse
		reverse(reversed.begin(), reversed.end());
		
		auto new_row = compress_row(reversed);
		new_row = merge_row(new_row);
		//undo reverse
		reverse(new_row.begin(), new_row.end());

		if(old_row != new_row){
			mv = true;
			board[i] = new_row;
		}
		
	}
	return mv;
}
// TODO: use column traversal
bool move_up(vector<vector<int>>& board){
	bool mv = false;
	
	for(int c = 0; c < 4; ++c){
		vector<int> col;
		for(int r = 0; r < 4; ++r){
			col.push_back(board[r][c]);
		}
		auto old_col = col;
		auto new_col = compress_row(col);
		new_col = merge_row(new_col);
		
		if(new_col != old_col){
			mv = true;
			for(int r = 0; r < 4; ++r){
                        	board[r][c] = new_col[r];
                	}
		}
	}
	return mv;
}
// TODO: use column traversal with reverse
bool move_down(vector<vector<int>>& board){
	bool mv = false;
	
	for(int c = 0; c < 4; ++c){
		vector<int> col;
		for(int r = 0; r < 4; ++r){
			col.push_back(board[r][c]);
		}
		auto old_col = col;
		//reverse 
		reverse(col.begin(), col.end());
		auto new_col = compress_row(col);
		new_col = merge_row(new_col);
		//undo reverse
		reverse(new_col.begin(), new_col.end());

                if(new_col != old_col){
                        mv = true;
			for(int r = 0; r < 4; ++r){
                        	board[r][c] = new_col[r];
                	}
                }
	}
		
	return mv;
}

int main(){
    srand(time(nullptr));
    vector<vector<int>> board(4, vector<int>(4,0));
    spawn_tile(board);
    spawn_tile(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board, first);
        first=false;
        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: get the history and print the board and continue
		if(!history.empty()){
			board = history.top();
			history.pop();	
		} 
		continue;
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Store the previous state here!
            	history.push(prev);
		spawn_tile(board);
	}

    }
    return 0;

}
