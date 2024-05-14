#define DEBUG 1 
#include <iostream>
//#include "deck.h"
//#include "player.h"
#include <vector>
#include "game.h"


using namespace std;
//random comment
//random comment P2
int main(){
	/*vector<int> common = {0,10,18,32,41};
	int test_p[2] = {45, 51};
	for (auto t: common)
		display(t);
	display(test_p[0]);
	display(test_p[1]);
	Player test;
	test.set_hand(test_p[0], test_p[1]);
	Score out = test.evaluate_hand(common);
	cout << out.r <<": " << out.tie_breaker << endl;
	return 0;*/
	Game gm(4);
	gm.run_game();
	return 0;
}
