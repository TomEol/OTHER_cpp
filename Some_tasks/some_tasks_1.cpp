#include<iostream>
#include<vector>
#include <string> 
#include<algorithm>
using namespace std;

void show(const vector<string>&change);
void amount_symbol(const vector<string>&symbols,vector<string>&main_data);

int main() {
	vector<string>main_data{ "A","B","C","D","D","A","B","C","A","B","A","B","C","D","D","A","B","C","A","B" };
	vector<string>symbols{ "A","B","C","D" };

	sort(main_data.begin(), main_data.end());
	show(main_data);

	cout << "\n\n";
	amount_symbol(symbols, main_data);

	cout << "\n\n";
	show(main_data);

	cout << "\n\n";
	return 0;
}
void show(const vector<string>&ch) {
	for (auto it_i = ch.begin(); it_i != ch.end(); ++it_i) {
		cout << " " << *it_i;
	}
}

void amount_symbol(const vector<string>&symbols, vector<string>&main_data) {

	int amount_right_symbol = 0;
	vector<int>amount_symbols;
	string symbol = "";

	for (size_t i = 0; i < symbols.size(); i++) {
		symbol = symbols[i];
		amount_right_symbol = count_if(main_data.begin(), main_data.end(), [symbol](string forCheckSymbol)->bool {
			if (forCheckSymbol == symbol) { return true; }
			else { return false; }});
		cout << "\n amount symbol [" << symbol << "] is = " << amount_right_symbol;
		amount_symbols.push_back(amount_right_symbol);
	}

	main_data.clear();

	for (size_t i = 0; i < symbols.size(); ++i) {
		main_data.push_back(to_string(amount_symbols[i]));
		main_data.push_back(symbols[i]);
	}
}
