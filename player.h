using namespace std;

#include <vector>
#include <map>
#include <iostream>
#include <bits/stdc++.h>
#include "game_abs.h"
#include "strat.h"



int tie_breaker_score(vector<int> &loose_cards);

class Player{
	public: 
		Player(){is_human=false;}
		Player(bool human){is_human = human;}
		void set_hand(int c_1, int c_2);
		void set_human(bool human){is_human = human;}
		bool is_human_check(){return is_human;}
		void show_cards(int *cards);
		Score evaluate_hand(const vector<int> &common);
		void set_folded(bool f){folded = f;}
		bool get_folded(){return folded;}
		void add_chips(int amount);
		int get_chips(){return chips;}
		int decision(Game_abs *gm, int idx, int round_num);
		bool match_curr_bet();
	private:
		int curr_hand[2];
		bool is_human;
		int chips=0;
		short blind;
		bool folded=false;

};
