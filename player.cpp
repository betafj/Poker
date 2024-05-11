#include "player.h"
//#define PRINT 1
void Player::set_hand(int c_1, int c_2){
	curr_hand[0] = c_1;
	curr_hand[1] = c_2;
}

void Player::show_cards(int *cards){
	cards[0] = curr_hand[0];
	cards[1] = curr_hand[1];

}



void Player::add_chips(int amount){
	if (chips + amount < 0)
		chips = 0;
	else
		chips += amount;

}

Score Player::evaluate_hand(const vector<int> &common){
	Strategy st(common, curr_hand[0], curr_hand[1]);
	int count = st.count_better_hands();
	float total = 50  - common.size();
	float prob = count/(total*(total-1)/2);
#ifdef PRINT
	cout << "Probability that a random hand is better than yours " << prob << endl;
#endif	
	return st.evaluate();
}


int Player::decision(Game_abs *gm, int idx, int round_num){
	double max = 0;
	srand(time(NULL));
	for (int i = 0; i < gm->bet_record.rows(); i++)
		if (gm->bet_record(i, round_num) > max)
			max = gm->bet_record.coeff(i, round_num);
	Strategy st(gm->displayed_cards, curr_hand[0], curr_hand[1]);
	int count = st.count_better_hands();
	float total = 50  - gm->displayed_cards.size();
	float rank_prob  = count/(total*(total-1)/2);
	int num_players = gm->count_active_players();
	//cout << "PLAYER " << idx << " THINKS THAT THERE ARE " << num_players << endl ;
#ifdef PRINT	
	cout << "PLAYER " << idx << " estimates rank " << rank_prob << endl;
	cout << "CURR POT " << gm->get_curr_pot_size() << endl;
#endif
	float win_prob = 1;
	for (int i = 0; i < num_players; i++)
		win_prob *= (1 - rank_prob);
#ifdef PRINT	
	cout << "PLAYER " << idx << " estimates that his chance of winning is " << win_prob << endl;
#endif
	int raise_amount = 0;
	
	float max_raise = win_prob*gm->get_curr_pot_size()/ (1 - win_prob);
	
	float expected_value  = win_prob*gm->get_curr_pot_size() - (max)*(1 - win_prob);
#ifdef PRINT
	cout << "PLAYER " << idx << " EST expected gain/loss " << expected_value << endl;
#endif
	int limit = (int) max_raise;
#ifdef PRINT
	cout << "BET LIMIT FOR PLayer " << idx << " is " << limit << endl;
#endif

	if (expected_value < 0)
		return -1;
	int diff = limit - (int) max;
	diff = (diff  > 0) ? diff: 1;
	raise_amount = rand()% diff;
	expected_value = win_prob*gm->get_curr_pot_size() - (max + raise_amount)*(1 - win_prob);
	while (expected_value < 0){
		expected_value = win_prob*gm->get_curr_pot_size() - (max + raise_amount)*(1 - win_prob);
	}	
		
	return (int)max + raise_amount;

}

bool Player::match_curr_bet(){
	return true;

}
