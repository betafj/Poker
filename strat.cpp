#include "strat.h"
#include <bits/stdc++.h>
#include <set>

bool Strategy::detect_valid_pair(int p_1, int p_2){
	set<int> all_cards;
	all_cards.insert(p_1);
	all_cards.insert(p_2);
	for (int i = 0; i < common.size(); i++)
		all_cards.insert(common[i]);
	all_cards.insert(my_hand[0]);
	all_cards.insert(my_hand[1]);
	return all_cards.size()==(common.size()+4);
	

}

int tie_breaker_score(vector<int> &loose_cards){
	sort(loose_cards.begin(), loose_cards.end(), std::greater<int>());
#ifdef NOTDEF
	cout << "**********************" <<endl;
	for (auto c: loose_cards)
		cout << c << endl;
#endif
	int sum = 0;
	int counter = 0;
	int i = 0;
	while (counter < 5 && i < loose_cards.size()){
		sum*=100;
		sum+=loose_cards[i];
		
		counter += (int)(loose_cards[i]/13 + 1);
		i++;
	}
	return sum;
}

int detect_straight(int * numbers){
	int curr_record = -1;
	for (int i=0; i <9; i++){
	
		bool test= true;
		for (int j=0; j<5; j++){
			test = test && numbers[i+j];
		
		}
		if (test)
			curr_record=i;
	}
	return curr_record;


}


Strategy::Strategy(vector<int> common_cards){
	for (auto c: common_cards)
		common.push_back(c);

}


Score Strategy::evaluate(){
	return evaluate(my_hand[0], my_hand[1]);
}

Strategy::Strategy(vector<int> common_cards, int c_1, int c_2){
	my_hand[0] = c_1;
	my_hand[1] = c_2;
	for (auto c: common_cards)
		common.push_back(c);

}

int Strategy::count_better_hands(int curr_1, int curr_2){

	int counter = 0;
	Score my_score = evaluate();
	for (int i = 0; i < 52; i++){
		for (int j = i+1; j < 52; j++){
			if (detect_valid_pair(i, j)){
				Score sim_score = evaluate(i, j);
				if (sim_score.r > my_score.r)
					counter++;
				if (sim_score.r == my_score.r && sim_score.tie_breaker > my_score.tie_breaker)
					counter++;
			
			}
		
		}
	
	
	}
	return counter;

}

int Strategy::count_better_hands(){

	return count_better_hands(my_hand[0], my_hand[1]);

}


Score Strategy::evaluate(int curr_1, int curr_2){
	int numbers[13];
	int suits[4] = {0,0,0,0};
	for (int i=0; i < 13; i++)
		numbers[i] = 0;
	int curr_hand[2] = {curr_1, curr_2};
	
	numbers[curr_hand[0]/4] ++;
	numbers[curr_hand[1]/4] ++;
	for (auto c: common){
		suits[c%4]++;
		numbers[c/4]++;
	}
	suits[curr_hand[0] %4]++;
	suits[curr_hand[1] %4]++;
	
	int flush_color=-1;
	for (int i =0; i < 4; i++)
		if (suits[i] >= 5)
			flush_color = i;
	int straight_orig = detect_straight(numbers);
	
	map<int, vector<int>> frequency_counter;
	
	for (int i=0;i<5; i++){
		vector<int> new_obj;
		frequency_counter[i] = new_obj;
	}
	
	for (int i =0; i < 13; i++)
		frequency_counter[numbers[i]].push_back(i);
		
#ifdef NOTDEF
	cout << "TTTTT" << endl;
	for (auto c: frequency_counter){
		if (c.second.size()){
			cout << c.first << ": ";
			for (auto t: c.second)
				cout << t << ",";
			cout << endl;
		}
			

	}
#endif
	Score output;
	vector<int> loose_cards;
	
	
	if (flush_color>=0 ){
		int flush_nums[13];
		memset(flush_nums, 0, 13*sizeof(int));
		for (int i =0; i < 2; i++)
			if (curr_hand[i]% 4 == flush_color)
				flush_nums[curr_hand[i]/4]++;
		for (auto c: common)
			if (c%4 == flush_color)
				flush_nums[c/4]++;
		int straight_check = detect_straight(flush_nums);
		if (straight_check >=0){
			output.r = STRAIGHT_FLUSH;
			output.tie_breaker = straight_check;
			return output;
		}
		
		output.r = FLUSH;
		for (int i =0; i < 2; i++)
			if (curr_hand[i]% 4 == flush_color)
				loose_cards.push_back(curr_hand[i]/4);
		for (auto c: common)
			if (c%4 == flush_color)
				loose_cards.push_back(c/4);
		output.tie_breaker = tie_breaker_score(loose_cards);
	}
	
	else if (straight_orig >=0){
		output.r = STRAIGHT;
		output.tie_breaker = straight_orig;
	
	}
	
	else if (frequency_counter[4].size() > 0){
		output.r = QUADRUPLE;
		for (int i = 12; i >=0; i--){
			if (i!= frequency_counter[4][0] && numbers[i] > 0){
				loose_cards.push_back(i);
				output.tie_breaker = tie_breaker_score(loose_cards);
				break;
			}
		}
	
	}
	
	else if (frequency_counter[3].size() == 2){
		output.r = FULL_HOUSE;
		loose_cards.push_back(frequency_counter[3][1]+26);
		loose_cards.push_back(frequency_counter[3][0]+13);
		output.tie_breaker = tie_breaker_score(loose_cards);
	}
	
	
	else if (frequency_counter[3].size() > 0 && frequency_counter[2].size()> 0){
		output.r = FULL_HOUSE;
		loose_cards.push_back(frequency_counter[3][0]+26);
		for (int i =0; i < frequency_counter[2].size(); i++)
			loose_cards.push_back(frequency_counter[2][i]+13);
		output.tie_breaker = tie_breaker_score(loose_cards);
		//for (int i = 0; i < loose_cards.size(); i++)
		//	cout << loose_cards[i] << ",";
		//cout << endl;
	}
	
	
	
	else if (frequency_counter[3].size() > 0){
		output.r = TRIPLE;
		loose_cards.push_back(frequency_counter[3][0]+26);
		for (int i =0; i < frequency_counter[1].size(); i++)
			loose_cards.push_back(frequency_counter[1][i]);
			
		output.tie_breaker = tie_breaker_score(loose_cards);
	}
	
	else if (frequency_counter[2].size() >= 2){
		output.r = TWO_PAIR;
		int length = frequency_counter[2].size()-1;
		int pair_high = frequency_counter[2][length];
		int pair_low = frequency_counter[2][length-1];
		int high_single;
		for (int i = 12; i>=0; i--){
			if (i == pair_high || i == pair_low)
				continue;
			if (numbers[i] >= 1){
				high_single = i;
				break;
			}
			
		}
		loose_cards.push_back(pair_high+13);
		loose_cards.push_back(pair_low + 13);
		loose_cards.push_back(high_single);
		output.tie_breaker = tie_breaker_score(loose_cards);
	
	}

	else if (frequency_counter[2].size() == 1){
		output.r = PAIR;
		loose_cards.push_back(frequency_counter[2][0]+13);
		for (auto c: frequency_counter[1])
			loose_cards.push_back(c);
		output.tie_breaker =  tie_breaker_score(loose_cards);		
	
	}
		
	else {
		output.r = SINGLE;
		for (auto c: frequency_counter[1])
			loose_cards.push_back(c);
		output.tie_breaker = tie_breaker_score(loose_cards);
	
	
	}
	return output;


}
