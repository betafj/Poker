#include <vector>
#include <Eigen/Dense>
#define NO_BET -98
class Game_abs{
	public:
		vector<int> displayed_cards;
		Eigen::MatrixXd bet_record; 
		int table_size;
		int count_active_players(){
			int num_players = table_size;
			for (int i = 0; i < table_size;i++){
				for (int j=0; j < 4; j++){
					if (bet_record(i, j) == -1){
						num_players--;
						break;
					}
				
				}
			}
			return num_players;
	
		}
		
		int get_curr_pot_size(){
			int pot = 0;
			for (int i = 0; i < table_size; i++){
				for (int j=0; j < 4; j++){
					pot += max((int)bet_record(i, j), 0);
				
				}
			
			}
			return pot;
		
		
		}
		
		int determine_round(){
			for (int i =0; i < table_size; i++){
				for (int j=3; j >=0; j--){
					if (bet_record(i, j) != NO_BET)
						return j;
				}
			
			}
		
			return -1;
		}
		
		int get_curr_max_bet(){
			int rnd = determine_round();
			double max_bet = 0;
			for (int i =0 ; i < table_size; i++)
				if (bet_record(i, rnd) > max_bet)
					max_bet = bet_record(i, rnd);
			return (int)max_bet;
		
		}
		
		int check_raise(){
			int rnd = determine_round();
			
			int max_bet = get_curr_max_bet();
			
			for (int i =0; i < table_size; i++)
				if (bet_record(i, rnd) >= 0 && bet_record(i, rnd) < max_bet)
					return i;
			return -1;
		
		
		}
		
	

	private:
		
		


};
