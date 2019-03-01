#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
using namespace std;
 //creating class for the dictionary
class dictionaryItem {
private:
	string name{ "none" };
	string description{ "none" };
	string speech{ "none" };
public:
	dictionaryItem() {}
	dictionaryItem(string name, string description, string speech) : name(name), description(description), speech(speech) {};

	string getName() const {
		return this->name;
	}
	string getDescription() const {
		return this->description;
	}
	string getSpeech() const {
		return this->speech;
	}
};

bool isSpeech(const string&);// for part of speech
string trimString(string);
void printSearchValues(string, string);//prints
void openFile(const string&);

multimap<string, dictionaryItem> dictionary;

int main()
{
	openFile("Data.CS.SFSU.txt");
	cout << "----DICTIONARY 340 C++----\n\n";
	while (true) {

		string srchLine{ "" };
		string name{ "" };
		string partOfSpeech{ "" };

		cout << "Search: ";
		getline(cin, srchLine);
		trimString(srchLine);

		transform(srchLine.begin(), srchLine.end(), srchLine.begin(), ::tolower);

		if (srchLine == "!q") {
			cout << "\n -----THANK YOU-----" << endl;
			break;
		}

		size_t found = srchLine.find(' ');

		if (found != string::npos) {
			
			name = trimString(srchLine.substr(0, srchLine.find(' ')));
			partOfSpeech = trimString(srchLine.substr(srchLine.find(' ') + 1));

			size_t extraWords = partOfSpeech.find(' ');

			if ((extraWords != string::npos)) {
				cout << "\t|" << endl;
				cout << "\t<Please enter a search key (and a part of speech).>" << endl;
				cout << "\t|" << endl;
			}
			else if (isSpeech(partOfSpeech)) {

				printSearchValues(name, partOfSpeech);
			}
			else {
				cout << "\t|" << endl;
				cout << "\t<2nd argument must be a part of speech.> \n";
				cout << "\t|" << endl;
			}
		}
		else {
			
			name = trimString(srchLine);
			printSearchValues(name, "none");

		}
	}
	system("pause");
	return 0;
}

void openFile(const string& location) {
	ifstream inFile;
	cout << "! Opening data file... ./Data.CS.SFSU.txt" << endl;
	inFile.open(location);
	string currentLine{ "" };
	cout << "! Loading data..." << endl;

	while (getline(inFile, currentLine)) {

		int lastPos = currentLine.find('|');
		string name{ trimString(currentLine.substr(0, lastPos)) };
		currentLine.erase(0, lastPos + 1);

		stringstream stream(currentLine);
		string posAndDef{ "" };
		while (getline(stream, posAndDef, '|')) {
			string speech{ trimString(posAndDef.substr(0, posAndDef.find("=>"))) };
			string description{ trimString(posAndDef.substr((posAndDef.find("=>") + 2))) };

			dictionary.insert(make_pair(name, *(new dictionaryItem(name, description, speech))));
		}
	}
	cout << "! Loading data completed..." << endl;
	cout << "! Closing data file... ./Data.CS.SFSU.txt \n" << endl;
	inFile.close();
}

void printSearchValues(string key, string partOfSpeech = "none") {
	if (key.size() == 0) {
		cout << "\t|" << endl;
		cout << "\t<Please enter a search key (and a part of speech).>" << endl;
		cout << "\t|" << endl;
		return;
	}
	else if (dictionary.count(key) == 0) {
		cout << "\t|" << endl;
		cout << "\t<Not found>" << endl;
		cout << "\t|" << endl;
		return;
	}
	typedef multimap<string, dictionaryItem>::iterator dictionaryItemsIterator;
	pair<dictionaryItemsIterator, dictionaryItemsIterator> result = dictionary.equal_range(key);
	if (partOfSpeech == "none") {
		cout << "\t|" << endl;
		for (dictionaryItemsIterator it = result.first; it != result.second; it++) {
			cout <<"\t"<< it->first << "[" << (it->second).getSpeech() << "] : " << (it->second).getDescription() << endl;
		}cout << "\t|" << endl;
	}
	else {
		int numberMatches{ 0 };
		cout << "\t|" << endl;
		for (dictionaryItemsIterator it = result.first; it != result.second; it++) {
			
			if ((it->second).getSpeech() == partOfSpeech) {
				
				cout <<"\t" <<it->first << "[" << (it->second).getSpeech() << "] : " << (it->second).getDescription() << endl;
				
				numberMatches++;
			}
		}cout << "\t|" << endl;
		if (numberMatches == 0) {
			cout << "\t<Not found>" << endl;
			cout << "\t|" << endl;
		}
	}
}

bool isSpeech(const string& arg) {
	
	string typesOfSpeech[3] = { "noun", "verb", "adjective" };
	for (int i = 0; i < 3; i++) {
		if (typesOfSpeech[i] == arg) {

			return true;

		}
	}

	return false;
}
string trimString(string arg) {
	if (arg.length() >= 2) {
		if (arg[0] == ' ') {

			arg = arg.substr(1);
		}
		if (arg[arg.length() - 1] == ' ') {

			arg = arg.substr(0, arg.length() - 1);
		}
	}
	return arg;
}