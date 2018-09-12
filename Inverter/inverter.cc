#include <iostream>
#include <fstream> 
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <sstream> 
#include <iterator> 
#include <stdio.h>
#include <ctype.h>

using namespace std; 

int main(int argc, char* argv[])
{
    //Create Map for files
	map<string, int> files;
	
	//Create Map for Words
	map<string, vector<int> > words;
	
	//Error check for the file being read
	ifstream File;
    int fileNumber = 0;

	//Test that argv works and fix later
	string fname = argv[1];
	//cout << argv[1];

    File.open(fname.c_str(), ios::in);
    if (!File.is_open()) 
	{
        //cerr << "Error: file could not be opened" << endl;
        return -1;
	}
	    
	//Loop for reading the file 
    while (File.good()) {
		string nextFileName;
		//cout << fileNumber << endl;
		getline(File, nextFileName, '\n');
		
		//Print out file
		//cout << nextFileName << endl; 
		
		//Insert into Map for files
		files.insert(make_pair(nextFileName, fileNumber)); 
		
		//Error check for the words being read
		ifstream File2;
		File2.open(nextFileName.c_str(), ios::in);
		
		if (!File2.is_open()) 
		{
			//cerr << "Error: file could not be opened" << nextFileName << endl;
			continue;
			fileNumber++;
			//cout << fileNumber << endl;
		} 
		string nextLine;
        
		//Loop for reading the words
		while (File2.good()){			
			getline(File2, nextLine, '\n');
			
			//Print out words
			//cout << nextLine << endl; 

			//Splitter
			istringstream iss(nextLine);
			do{
				string word;
				string sub;	
				iss >> word;
				//cout << word << endl;	
				int s= 0;				
				int l= 0;
				int count= 0;
				//cout << "I have s: " << s << endl;
				//cout << "I have l: " << l << endl;
				//cout << "Word Size: " << word.size() << endl;

				while(l != word.size()){		
				    //cout << "I have s: " << s << endl; 
				    //cout << "I have l: " << l << endl;	
				    //cout << "I have count: " << count << endl;
				    //cout << "Letter Index : " << word[s] << endl;
					//cout << "Letter I am at : " << word[l] << endl;
					count++;
					if(!isalpha(word[l])){
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;	
				   	 	//cout << "I have count: " << count << endl;
				   	 	//cout << "Letter Index : " << word[s] << endl;
						//cout << "Letter I am at : " << word[l] << endl;
						l--;
						count--;
						//if(-1 == l){ 
						//	cout << "I have a sub: " << sub << endl;
						//	l++;
						//	s = l+1;
						//	break;
						//}
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;	
				    	//cout << "I have count: " << count << endl;
				    	//cout << "Letter Index : " << word[s] << endl;
						//cout << "Letter I am at : " << word[l] << endl;
						sub = word.substr(s, count);	
						//cout << "I have a sub: " << sub << endl;					
						if(sub == "\0" ){ 							
						//cout << "Y";
						}				
						else{
							if(words.find(sub) != words.end()){
			
								//Insert Word if New File or Skip Word if Old File  
								if(find(words.at(sub).begin(), words.at(sub).end(), fileNumber) != words.at(sub).end()){
							
									//Why isnt this going off							
									//cout << "yes2" << endl; 						
									//continue; 
								}
				
								else{
									words[sub].push_back(fileNumber);
								}
							}		
			
							//Insert new Word into Map of Words
							else{
								words[sub].push_back(fileNumber);
							}
						}
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;	
						count = 0;
						l++;
						s = l+1;
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;
				   	 	//cout << "Next Letter: " << word[s] << endl;	
				   	 	//cout << "Word Size: " << word.size() << endl;
					}	
					l++;	
					//cout << "I have l: " << l << endl;				
				}					
				//cout << "I have count: " << count << endl;
				//cout << word << endl;					
				
				//Search for old Word
				if(word == "\0" ){ //cout << "Y";
				}				
				else{
					if(word.size() == count){
						if(words.find(word) != words.end()){
			
							//Insert Word if New File or Skip Word if Old File  
							if(find(words.at(word).begin(), words.at(word).end(), fileNumber) != words.at(word).end()){
							
								//Why isnt this going off							
								//cout << "yes3" << endl; 						
								continue; 
							}
				
							else{
								words[word].push_back(fileNumber);
							}
						}		
			
						//Insert new Word into Map of Words
						else{
							words[word].push_back(fileNumber);
						}
					}
					else{
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;	
				   	 	//cout << "I have count: " << count << endl;
				   	 	//cout << "Letter Index : " << word[s] << endl;
						//cout << "Letter I am at : " << word[l] << endl;
						l--;
						if(-1 == l){ 
							//cout << "I have a sub: " << sub << endl;
							break;
						}
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;	
				    	//cout << "I have count: " << count << endl;
				    	//cout << "Letter Index : " << word[s] << endl;
						//cout << "Letter I am at : " << word[l] << endl;
						sub = word.substr(s, count);	
						//cout << "I have a sub: " << sub << endl;					
						if(sub == "\0" ){ 							
						//cout << "Y";
						}				
						else{
							if(words.find(sub) != words.end()){
			
								//Insert Word if New File or Skip Word if Old File  
								if(find(words.at(sub).begin(), words.at(sub).end(), fileNumber) != words.at(sub).end()){
							
									//Why isnt this going off							
									//cout << "yes1" << endl; 						
									continue; 
								}
				
								else{
									words[sub].push_back(fileNumber);
								}
							}		
			
							//Insert new Word into Map of Words
							else{
								words[sub].push_back(fileNumber);
							}
						}
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;	
						count = 0;
						l++;
						s = l+1;
						//cout << "I have s: " << s << endl; 
				   	 	//cout << "I have l: " << l << endl;
				   	 	//cout << "Next Letter: " << word[s] << endl;	
				   	 	//cout << "Word Size: " << word.size() << endl;
					}
				}
			} while(iss);  		
		}
		
		//Increment file
		//cout << fileNumber << endl;
		//File2.close();		
		fileNumber++; 
    }
	
	//Print out the Words Map
	map<string, vector<int> >::iterator it;
	for(it=words.begin(); it!=words.end(); ++it){
		cout << (*it).first << ":";
		vector<int> iter = (*it).second;
		for(unsigned j=0; j<iter.size(); j++){
			cout << " " << iter[j];
     	}
		cout << endl; 
	}
}
