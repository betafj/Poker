void display(int card);
#include <iostream>
#include <vector>
#include "player.h"
#include "deck.h"
#include <string>

using namespace std;
class Game : Game_abs{
	public:
		//vector<int> displayed_cards;
		string hand_type[9]={"SINGLE", "PAIR", "TWO_PAIR", "TRIPLE", "STRAIGHT", "FLUSH", "FULL_HOUSE", "QUADRUPLE", "STRAIGHT_FLUSH"};
		Game(int num_players){
			table_size = num_players;
			for (int i =0; i < num_players; i++){
				Player p;
				if (i==0)
					p.set_human(true);
				p.add_chips(1000);
				players.push_back(p);
			}
			bet_record.resize(num_players, 4);
			reset_bets();
			
				
		}
		void reset_bets();
		int count_active_players();
		void stream_status();
		
		int last_man_standing(int round_num);
		void deal_cards(){
			for (int i = 0; i < players.size(); i++){
				int c_1 = D.get_top();
				int c_2 = D.get_top();
				players[i].set_hand(c_1, c_2);
				
			}
		
		}
		
		int find_loser_min(){
			int min_chips = 1000;
			for (int i =0; i < players.size(); i++){
				if (players[i].get_chips() < min_chips)
					min_chips = players[i].get_chips();
			}
			return min_chips;
		
		}
		
		void run_round(){
			deal_cards();
			show_players_hand(false);
			
			string t;
			//cout << "DO YOU FOLD?  Y/N" << endl;
			//cin >> t;
			int num_chips = 1001;
			int MAX_ALLOWABLE = 100;
			int bet_round = 0;
			int num_cards_to_reveal  = 0;
			bool raise_flag = false;
			//dealer+1  = small_blind, dealer+2 = big_blind
			for (bet_round=0; bet_round < 4; bet_round++){
				players[0].evaluate_hand(displayed_cards);
				stream_status();
				for (int idx=dealer+1; idx <= dealer+players.size(); idx++){
					int i = idx % players.size();
					if (players[i].is_human_check() && !players[i].get_folded()){
						cout << "Reminder  " << endl;
						display_hand(i);
						cout << "DO YOU FOLD?  Y/N" << endl;
						//cin >> t;
						getline(cin, t);
						if (t[0] == 'Y'){
							cout << "FOLDED " << endl;
							fold_player(0);
							num_chips = 0;
						}
						else{
							num_chips = 1001;
							int max_bet =  find_loser_min();
							while(num_chips > max_bet or num_chips < 0 or num_chips > MAX_ALLOWABLE){
								if (!raise_flag)
									cout << "BET! How many chips? Enter the number as an int: " << endl;
								else
									cout << "MATCH ? " << endl;
								try{
									string inp;
									getline(cin, inp);
									num_chips = stoi(inp);	
								}
								catch(exception & err) {
									num_chips = 0;
								}
								
								if (num_chips < 0 || num_chips > max_bet or num_chips > MAX_ALLOWABLE)
									cout << "Not a valid bet: try again :(" << endl;
							}
							pot+= num_chips;
							change_chips(i, -num_chips);
							bet_record(i, bet_round) = num_chips;
						}
						//bet_record[i][bet_round] = num_chips;
					}
					else if (!players[i].get_folded()){
						num_chips = min(players[i].decision(this, i, bet_round), MAX_ALLOWABLE);
						if (num_chips >= 0)
							cout << "COMPUTER PLAYER " << i << "BETS " << num_chips << endl;
						else{
							fold_player(i);
							cout << "COMPUTER PLAYER " << i << " FOLDS " << endl;
							
						}
						pot += max(num_chips, 0);
						change_chips(i, -max(num_chips, 0));
						bet_record(i, bet_round)= num_chips;
					
					}
				
				
				}
				int last_raise_idx = check_raise();
				if (last_raise_idx >= 0){
					int curr_bid = get_curr_max_bet();
					cout << " CURRENT MAX BET FOR THIS ROUND IS " << curr_bid << endl;
					cout << "YOU MUST MATCH TO CONTINUE TO PLAY " << endl;
					for (int k = last_raise_idx; k < players.size(); k++){
						int diff = curr_bid - bet_record(k, bet_round);
						if (bet_record(k, bet_round) < 0 )
							continue;
						if (players[k].is_human_check()){
							cout << "Match the max bet or fold? 'N' to fold, anything else to match: " << endl;
							string in_;
							//cin >> in_;
							getline(cin, in_);
							if (in_[0] != 'N'){
								
								change_chips(k, -diff);
								pot += diff;
								bet_record(k, bet_round) = curr_bid;
								cout << "You have added another " << diff << " to the pot " << endl;
								cout << "Current pot size = " << pot << endl;
							}
							else{
								fold_player(k);
								cout << "Player " << k << " has folded " << endl;
							
							}
							
							
						
						}
						else{
							bool match_decision = players[k].match_curr_bet();
							if (match_decision && diff > 0){
								cout << "Player " << k << "stays in and bets another " << diff << endl ;
								change_chips(k, -diff);
								pot += diff;
								bet_record(k, bet_round) = curr_bid;
							}	
							else if (diff > 0){
								fold_player(k);
								cout << "Player " << k << " has folded " << endl;
							}																																																		
						
						}
					
					
					}
				
				}
				
				if (bet_round == 3 || last_man_standing(bet_round) >= 0)
					break;
				num_cards_to_reveal = (bet_round==0)?3:1;
				
				reveal_cards(num_cards_to_reveal);
				cout << "NUMBER OF ACTIVE PLAYERS REMAINING " << Game_abs::count_active_players() << endl;
				cout <<"Press Y to see the NEXT " <<num_cards_to_reveal <<" cards " << endl;
				//cin >> t;
				getline(cin, t);
				for (int i = 0; i < displayed_cards.size(); i++){
					display(displayed_cards[i]);
				}
				cout << endl;
			}	
			//gm.test_eval();
			cout << "Press Y to reveal all hands" << endl;
			//cin >> t;
			getline(cin, t);
			show_players_hand(true);
			int remain = last_man_standing(bet_round);
			
			vector<int> winners;
			if (remain == -1)
				winners = find_winners();
			else
				winners.push_back(remain);
			cout << "CARDS ON THE TABLE " << endl;
			for (int i = 0; i < displayed_cards.size(); i++){
					display(displayed_cards[i]);
				}
			cout << endl;
			for (auto w: winners)
				cout << "Player "<< w << ",";
			cout << "WINS " << endl;
			adjust_chips(pot, winners);
			show_chips();
		
		
		}
		
				
		void run_game(){
			bool game_over = false;
			while (!game_over){
				D.reset();
				D.shuffle(30000);
				displayed_cards.clear();
				
				for (auto &p: players)
					p.set_folded(false);
				run_round();
				reset_bets();
				string a;
				cout << "Type whatever you want to reshuffle" << endl;
				//cin >> a;
				getline(cin, a);
			
			}
		
		}
		
		void reveal_cards(int num){
			for (int i = 0; i < num; i++){
				int curr_card = D.get_top();
				displayed_cards.push_back(curr_card);
			}
		}
		
		void display_hand(int idx){
			int hand[2];
			players[idx].show_cards(hand);
			if (players[idx].is_human_check()){
				display(hand[0]);
				display(hand[1]);
				cout << endl;
			}
		
		}
		
		void show_players_hand(bool full_reveal){
		
			for (int i = 0; i < players.size(); i++){
				//if (players[i].get_folded())
				//	continue;
				int hand[2];
				players[i].show_cards(hand);
				cout << "Player " << i << ": " << endl;
				if (players[i].is_human_check() || full_reveal){
					display(hand[0]);
					display(hand[1]);
					cout << endl;
				}
				else{
					cout << "XX" << endl;
				
				}
				cout << "*******" << endl;
			
			}
		
		}
		
		void test_eval(){
			players[0].evaluate_hand(displayed_cards);
		}
		
		void fold_player(int i ){
			players[i].set_folded(true);
			int round_num=0;
			round_num=(displayed_cards.size()) ? displayed_cards.size()-2 :0;
			bet_record(i, round_num) = -1;
		}
		
		vector<int> find_winners(){
			
			Score best;
			best.r=(enum RANK)0;
			best.tie_breaker=0;
			vector<int> winners;
			for (int i = 0; i < players.size(); i++){
				if (players[i].get_folded())
					continue;
				Score curr_score = players[i].evaluate_hand(displayed_cards);
				cout << "Player " << i << " has a " << hand_type[(int)curr_score.r] << ": " <<curr_score.tie_breaker << endl;
				if (curr_score.r > best.r){
					best = curr_score;
					winners.clear();
					winners.push_back(i);
				}
				else if (curr_score.r == best.r && curr_score.tie_breaker > best.tie_breaker){
					best=curr_score;
					winners.clear();
					winners.push_back(i);
				}
				else if (curr_score.r == best.r && curr_score.tie_breaker == best.tie_breaker){
					winners.push_back(i);
				}
			
			}
			return winners;
		
		}
		
		void show_chips(){
			cout << "CURRENT COUNT of CHIPS " << endl;
			for (int i=0; i < players.size(); i++){
				
				cout << "Player " << i << ": " << players[i].get_chips() << endl;
			}
			cout << "***********************" << endl;
		
		}
		
		void change_chips(int player_id, int amt){
			players[player_id].add_chips(amt);
		}
		
		void adjust_chips(int pot_size, vector<int> winners){
			//vector<int> winners = find_winners();
			for (auto c: winners){
				players[c].add_chips(pot_size/winners.size());
			}
			pot = 0;
		}
		
		
	private:
		vector<Player> players;
		Deck D;
		int pot=0;
		int dealer = 3;
		

};
