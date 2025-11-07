#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>

using namespace std;
const int prefix=0,suffix=1,exact=2;
const int NONE=0,AND=1,OR=2;

struct node{
public:
    bool is_leaf;
    node* child[26];
};

class Root{
public:
    node *pos;
    node *neg;

    Root(){
        pos=getNode();
        neg=getNode();
    }
    ~Root(){
        delete pos;
        delete neg;
    }

    node *getNode(void){
        node *pNode =  new node;
        pNode->is_leaf=false;
        for (int i = 0; i < 26; i++)
            pNode->child[i] = NULL;
        return pNode;
    }

    void Tri_insert(string key){
        node* tmpPos = pos;
        node* tmpNeg = neg;
        int len=key.length();
        for(int i=0;i<len;i++){
            int index = isupper(key[i]) ? key[i] - 'A': key[i] - 'a';
            if(!(tmpPos->child[index])){
                tmpPos->child[index] = getNode();
            }
            tmpPos = tmpPos->child[index];
        }
        tmpPos->is_leaf = true;

        for(int i = len-1;i>=0;i--){
            int index = isupper(key[i]) ? key[i] - 'A': key[i] - 'a';
            if(!(tmpNeg->child[index]))
                tmpNeg->child[index] = getNode();
            tmpNeg = tmpNeg->child[index];
        }
        //tmpNeg->is_leaf = true;
    }

    bool Tri_search(string key,int type){
        int len=key.length();
        if(type==suffix){
            node* tmpR=neg;
            for(int i=len-1;i>=0;i--){
                int index = isupper(key[i]) ? key[i] - 'A': key[i] - 'a';
                if(!tmpR->child[index])
                    return false;
                tmpR=tmpR->child[index];
            }
            return true;
        }
        else{
            node* tmpR=pos;
            for(int i=0;i<len;i++){
                int index = isupper(key[i]) ? key[i] - 'A': key[i] - 'a';
                if(!tmpR->child[index])
                    return false;
                tmpR=tmpR->child[index];
            }
            if(type==prefix)
                return true;
            return(tmpR->is_leaf);
        }
    }
};

vector<Root*> root;
// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}

string alpha(string str){
    string new_str;
    for(auto & i : str){
        if(isalpha(i))
            new_str.push_back(i);
    }
    return new_str;
}


int main(int argc, char *argv[])
{

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");//"data/"
	string query = string(argv[2]);//query.txt
	string output = string(argv[3]);//output.txt

	// Read File & Parser Example
	string file, title_name, tmp,Qtmp;
	fstream fi,Q,fout;
	vector<string> tmp_string,all_title;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	int data_i=0;
    while(1){
        fi.open(data_dir+to_string(data_i)+string(".txt"), ios::in);
        if(!fi) break;

        // GET TITLENAME
        getline(fi, title_name);
        // GET CONTENT
        // GET TITLENAME WORD ARRAY
        tmp_string = split(title_name, " ");
        all_title.push_back(title_name);
        vector<string> title = word_parse(tmp_string);

        root.push_back(new Root());

        for(auto &word : title){
         	root[data_i]->Tri_insert(word);
        }
        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){

            // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");

            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);

            for(auto &word : content){
                root[data_i]->Tri_insert(word);
                //cout << word << endl;
            }
        }
        // CLOSE FILE
        fi.close();
        data_i++;
    }
    Q.open(query, ios::in);
    fout.open(output, ios::out);
    while(getline(Q, Qtmp)){
        int id=0;
        bool notFound=1;
        vector<string> Querys=split(Qtmp, " ");
        for(auto & curRoot : root){
            bool ans=0;
            int preOP=NONE;
            for(auto & key : Querys){
                if(key=="+"){
                    preOP=AND;
                }
                else if(key=="/"){
                    preOP=OR;
                }
                else if(isalpha(key[0])){
                    if(preOP==NONE) ans=curRoot->Tri_search(key,prefix);
                    else if(preOP==AND) ans=ans&&curRoot->Tri_search(key,prefix);
                    else if(preOP==OR) ans=ans||curRoot->Tri_search(key,prefix);
                    preOP=NONE;
                }
                else if(key[0]=='"'){
                    string newkey=alpha(key);
                    if(preOP==NONE) ans=curRoot->Tri_search(newkey,exact);
                    else if(preOP==AND) ans=ans&&curRoot->Tri_search(newkey,exact);
                    else if(preOP==OR) ans=ans||curRoot->Tri_search(newkey,exact);
                    preOP=NONE;
                }
                else if(key[0]=='*'){
                    string newkey=alpha(key);
                    if(preOP==NONE) ans=curRoot->Tri_search(newkey,suffix);
                    else if(preOP==AND) ans=ans&&curRoot->Tri_search(newkey,suffix);
                    else if(preOP==OR) ans=ans||curRoot->Tri_search(newkey,suffix);
                    preOP=NONE;
                }
            }
            if(ans) {
                fout<<all_title[id]<<"\n";
                notFound=0;
            }
            id++;
        }
        if(notFound) {
            fout<<"Not Found!\n";
        }
    }
    Q.close();
    fout.close();
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////

//g++ -std=c++17 -o essay-search.exe ./main.cpp
//essay-search.exe data query.txt output.txt
//essay-search.exe data query.txt myans.txt

