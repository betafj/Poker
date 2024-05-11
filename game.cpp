#include "game.h"

void display(int card){
	string suits[4] = {"\u2663", "\u2662", "\u2661", "\u2660"};
	int num = card/4+2;
	if (num <= 10)
		cout << num  << suits[card%4] << ", ";
	else if (num == 11)
		cout << "J" << suits[card%4] << ", ";
	else if (num == 12)
		cout << "Q"  << suits[card%4] << ", ";
	else if (num == 13)
		cout << "K"  << suits[card%4] << ", ";
	else
		cout << "A"  << suits[card%4] << ", ";

}

void Game::reset_bets(){
	for (int i= 0; i < players.size(); i++)
		for (int j=0; j < 4; j++)
			bet_record(i,j) = NO_BET;

}

int Game::last_man_standing(int round_num){
	
	int remaining = 0;
	int player = -1;
	for (int i = 0; i < players.size(); i++){
		if (bet_record(i, round_num) >= 0){
			remaining++;
			player = i;
		}
	}
	if (remaining == 1)
		return player;
	return -1;

}

void Game::stream_status(){
	ofstream out_file("dump_poker.txt");
	int hand[2];
	players[0].show_cards(hand);
	out_file << "PLAYER 0:" << hand[0] << " " << hand[1] << endl;
	out_file.close();
}
/*
int Game::count_active_players(){
	int num_players = players.size();
	for (int i = 0; i < players.size();i++){
		for (int j=0; j < 4; j++){
			if (bet_record(i, j) == -1){
				num_players--;
				break;
			}
		
		}
	
	}
	return num_players;

}*/


