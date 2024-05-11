#include <vector>
using namespace std;


enum RANK{
	SINGLE=0,
	PAIR,
	TWO_PAIR,
	TRIPLE,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	QUADRUPLE,
	STRAIGHT_FLUSH
};


struct Score{
	RANK r;
	int tie_breaker;
};

int detect_straight(int * numbers);

int tie_breaker_score(vector<int> &loose_cards);

class Strategy{
	public:
		Strategy(vector<int> common_cards);
		Strategy(vector<int> common_cards, int c_1, int c_2);
		Score evaluate(int curr_1, int curr_2);
		Score evaluate();
		int count_better_hands(int curr_1, int curr_2);
		int count_better_hands();
		bool detect_valid_pair(int p_1, int p_2);

	private:
		vector<int> common;
		int my_hand[2];



};
