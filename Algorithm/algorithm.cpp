//Thomas Passaro

/*To do:
	- Dynamic memory for reqs vector and dictionary??
	- Implement credits counting
	- Pointers
	- Output
	- Compare
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <map>

using namespace std;

//Takes in a datastructure which it will add classes to
void parse_reqs(vector<vector<string> > &reqs, string &f_name, vector<string> &comments){
	ifstream inFile(f_name);
	if (inFile.is_open()){
		string temp;
		string most_recent_comment = "";
    	while(getline(inFile,temp)){
    		//If comment then update field
    		int comment_index = temp.find("//");
		    if( comment_index != -1){
		    	most_recent_comment = temp.substr(comment_index+2);
		    	continue;
		    }

		    vector<string> options;
		    //Loop through and parse
	    	int a = temp.find("||");
	    	while(a != -1){
	    		string course = temp.substr(0,a);
	    		//If it cant be taken to satisfy req
	    		if(course.substr(0,1).compare("!") == 0)
	    			options.insert(options.begin(), course);
	    		else
	    			options.push_back(course);
	    		temp=temp.substr(a+2);
	    		a = temp.find("||");
	    	}
	    	if(temp.substr(0,1).compare("!") == 0)
	    		options.insert(options.begin(),temp);
	    	else
	    		options.push_back(temp);
	    	reqs.push_back(options);
	    	comments.push_back(most_recent_comment);
    	}
    	inFile.close();
    }
    else
    	cout << "Could not open " << f_name << endl;
}

bool special_compare(string &req, map<string, int> &classes, set<string> &unacceptable, bool noRepeat){
	//Iterate through set checking for a match
	for(map<string, int>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		int courseNum = stoi(itr->first.substr(5,4));
		int reqNum = stoi(req.substr(5,4));
		bool isUnacceptable = false;
		if(itr->first.substr(0,4).compare(req.substr(0,4)) == 0 && courseNum >= reqNum  && unacceptable.find(itr->first) == unacceptable.end()){
			if(noRepeat)
				classes.erase(itr);
			return true;
		}
	}
	return false;
}

bool concentration_compare(string &concentration, map<string, int> &classes_credits){
	//Checks if repition in multiple requirements is allowed (ex Comm intensive)
	bool canRepeat = false;
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		canRepeat = true;
	}

	int num_courses = stoi(concentration.substr(0,1));
	concentration = concentration.substr(2);
	int a = concentration.find("&&");
	string f_name;
	if(a == -1)
		f_name = "../Concentrations/" + concentration + ".txt";
	else
		f_name = "../Concentrations/" + concentration.substr(0,a) + ".txt";
	vector<vector<string> > conc_reqs;
	vector<int> not_taken;
	//parse_reqs(conc_reqs, f_name, comments);
	//  IMPLEMENT COURSE COUNTING
	//compare_courses(conc_reqs, classes, not_taken, !canRepeat, num_courses);
	if(conc_reqs.size()-not_taken.size() == num_courses)
		return true;
	return false;
}

int free_electives(map<string, int> &classes, string credits ){
	int numCreds = stoi(credits);
	int currentCreds = 0;
	cout << "h";
	for(map<string, int>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		cout << currentCreds << ": " << itr->first << endl;
		currentCreds += itr->second;
		if(currentCreds >= numCreds)
			return 0;
	}
	return numCreds - currentCreds;
}

void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed, bool noRepeat){
	for(int i = 0; i < reqs.size(); ++i){
		//Find unacceptable course
		set<string> unacceptable;
		int before = 0;
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}
		bool satisfied = false;
		for(int j = before; j < reqs[i].size(); ++j){
			string element = reqs[i][j];
			if(element.find("$") == 0){
				string credits = element.substr(1);
				int dif = free_electives(classes, credits);
				if(dif > 0){
					vector<string> creds;
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}
				else
					satisfied = true;
			}
			else if(element.find("(") != -1)
				element = element.substr(1).substr(0,element.length()-2);
				int index; 
				string temp;
			while(true){
				satisfied = false;
				index = element.find("&&");
				if(index == -1)
					temp = element;
				else{
					temp = element.substr(0,index);
					element = element.substr(index+2);
				}
				//Range(CSCI-4000+)
				if(temp.substr(0,1).compare("@") == 0){
					string name = temp.substr(1);
					satisfied = concentration_compare(name, classes);
				}
				else if(temp.find("+") != -1){
					satisfied = special_compare(temp, classes, unacceptable, noRepeat);
				}
				//Regular courses
				else{
					if(classes.find(temp) != classes.end()){
						satisfied = true;
						if(noRepeat)
							classes.erase(reqs[i][j]);
					}
				}
				if(!satisfied)
					break;
				if(index == -1)
					break;
			}
			if(satisfied)
				break;
		}
		if(!satisfied)
			needed.push_back(i);
	}
}

bool file_output(vector<vector<string> > &reqs, vector<int> &needed, string f_name, vector<string> &comments){
	ofstream outFile(f_name);
  	if (!outFile.is_open())
    	return false;
    outFile << "CAUTION: Read the READ.ME before using this program." << endl;
    outFile << "It contains extremely vital information about the current state of the program and it's known weaknesses/shortcomings." << endl;
	//Loop through in needed and output each req
	for(int i = 0; i < needed.size(); ++i){
		outFile << comments[needed[i]] << endl;
		for(int j = 0; j < reqs[needed[i]].size(); ++j){
			outFile << reqs[needed[i]][j] << " ";
		}
		outFile << endl;
	}
	outFile.close();
	return true;
}

void read_classes(map<string, int> &classes, string fname){
	ifstream inFile(fname);
	if (inFile.is_open()){
		string temp;
    	while(getline(inFile,temp)){
    		//If comment then skip
		    if(temp.find("//") != -1)
		    	continue;
		    int index = temp.find(":");
		    classes[temp.substr(0, index)] = stoi(temp.substr(index+1));
		}
		inFile.close();
    }
    else
    	cout << "error";
}

/*&pair<map<string, string>, map<string, string> > runAlgo(string req_file, map<string, int> courses){
	return NULL;
}*/

//pair<map<string, string>, map<string, string> > generateCappReport(string input_file, map<string, int> courses){
int main(int argc, const char* args[]){
	if(args[1]==NULL || args[2]==NULL){
		cerr << "One or more arguments is NULL." << endl;
		return 0;
	}

	vector<vector<string> > reqs;
	vector<string> comments;
	vector<int> needed;
	string input_file_name= args[1];
	string classes_fname = args[2];
	string OUTPUT_FILE_NAME = "Test_Files/algorithm_output.txt";
	map<string, int> classes;
	read_classes(classes, classes_fname);
	parse_reqs(reqs, input_file_name, comments);

	
	compare_courses(reqs, classes, needed, true);
	for(int i = 0; i < reqs.size(); ++i){
		for(int j = 0; j < reqs[i].size(); ++j)
			cout << reqs[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	for(int i = 0; i <needed.size(); ++i)
		cout << needed[i] << endl;
	for(int i = 0; i < comments.size(); ++ i)
		cout << comments[i] << endl;
	file_output(reqs, needed, OUTPUT_FILE_NAME, comments);
}

