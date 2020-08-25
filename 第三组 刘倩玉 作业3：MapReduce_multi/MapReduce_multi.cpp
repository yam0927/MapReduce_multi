#include<bits/stdc++.h>
using namespace std;

void *status1, *status2, *status3;

string contents; //original hamlet txt
vector<string> words; //all legal words

vector<string> words1; //p1's words 
vector<string> words2; //p2's words
vector<string> words3; //p3's words

vector<pair<string, int> > map_rst1; //p1's words <word,1>
vector<pair<string, int> > map_rst2; //p2's words <word,1>
vector<pair<string, int> > map_rst3; //p3's words <word,1>

map<string, vector<int> > shuffle_rst; //all <word, [1,1,1...,1]>
vector<string> m_dict;

vector<pair<string, int> >reduce_rst1; 
vector<pair<string, int> >reduce_rst2;
vector<pair<string, int> >reduce_rst3;


void loadFile(){
    ifstream t("hamlet.txt");
    stringstream buffer;
    buffer << t.rdbuf();
    contents = (buffer.str());
}

void loadWords(){
    string temp = "";
    for(int i = 0; i < contents.length(); ){
        if(contents[i] >= 'a' && contents[i] <= 'z' || contents[i] >= 'A' && contents[i] <= 'Z'){
            temp += contents[i];
            i++;
        }
        else{
           if(temp != "")words.push_back(temp);
           i++;
           temp = "";
        }
    }
    for(int i = 0; i < words.size(); i++){
        transform(words[i].begin(), words[i].end(), words[i].begin(),::tolower);
    }
}

void getHamletWords(){
    loadFile();
    loadWords();
}


void* map1_op(void*){
    int block_size = words.size() / 3;
    for(int i = 0; i < block_size; i++){
        words1.push_back(words[i]);
    }
    for(int i = 0; i < words1.size(); i++){
          pair<string, int> tmp(words1[i], 1);
          map_rst1.push_back(tmp);
    }
}

void* map2_op(void*){
    int block_size = words.size() / 3;
    for(int i = block_size; i < 2 * block_size; i++){
        words2.push_back(words[i]);
    }
    for(int i = 0; i < words2.size(); i++){
          pair<string, int> tmp(words2[i], 1);
          map_rst2.push_back(tmp);
    }
}

void* map3_op(void*){
    int block_size = words.size() / 3;
    for(int i = 2 * block_size; i < words.size(); i++){
        words3.push_back(words[i]);
    }
    for(int i = 0; i < words3.size(); i++){
          pair<string, int> tmp(words3[i], 1);
          map_rst3.push_back(tmp);
    }
}

void shuffle_op(){
    for(int i = 0; i < map_rst1.size(); i++){
        string m_key = map_rst1[i].first;
        shuffle_rst[m_key].push_back(1);
    }
    for(int i = 0; i < map_rst2.size(); i++){
        string m_key = map_rst2[i].first;
        shuffle_rst[m_key].push_back(1);
    }
    for(int i = 0; i < map_rst3.size(); i++){
        string m_key = map_rst3[i].first;
        shuffle_rst[m_key].push_back(1);
    }
    for(map<string, vector<int> >::iterator it = shuffle_rst.begin();it != shuffle_rst.end();it++){
        m_dict.push_back(it->first);
    }
}

/*
void reduce_op(){
    for(map<string, vector<int> >::iterator it = shuffle_rst.begin(); it != shuffle_rst.end(); it++){
        string m_key = it->first;
        int m_cnt = shuffle_rst[m_key].size();
        pair<string, int> tmp(m_key, m_cnt);
        reduce_rst.push_back(tmp);
    } 
}*/

void* reduce1_op(void*){
    int block_size = m_dict.size() / 3;
    for(int i = 0; i < block_size; i++){
        string m_key = m_dict[i];
        int m_cnt = shuffle_rst[m_key].size();
        pair<string, int> tmp(m_key, m_cnt);
        reduce_rst1.push_back(tmp);
    }
}
void* reduce2_op(void*){
    int block_size = m_dict.size() / 3;
    for(int i = block_size; i < 2 * block_size; i++){
        string m_key = m_dict[i];
        int m_cnt = shuffle_rst[m_key].size();
        pair<string, int> tmp(m_key, m_cnt);
        reduce_rst2.push_back(tmp);
    }
}
        
void* reduce3_op(void*){
    int block_size = m_dict.size() / 3;
    for(int i = 2 * block_size; i < m_dict.size(); i++){
        string m_key = m_dict[i];
        int m_cnt = shuffle_rst[m_key].size();
        pair<string, int> tmp(m_key, m_cnt);
        reduce_rst3.push_back(tmp);
    }
}

  
void printMPR(){
    for(int i = 0; i < reduce_rst1.size(); i++){
        cout << "< " << reduce_rst1[i].first << "," << reduce_rst1[i].second << " >" << endl;
    }
    for(int i = 0; i < reduce_rst2.size(); i++){
        cout << "< " << reduce_rst2[i].first << "," << reduce_rst2[i].second << " >" << endl;
    }
    for(int i = 0; i < reduce_rst3.size(); i++){
        cout << "< " << reduce_rst3[i].first << "," << reduce_rst3[i].second << " >" << endl;
    }
}

void outputFile(){
    ofstream ofs; 
    ofs.open("outfile.txt");
    for(int i = 0; i < reduce_rst1.size(); i++){
        ofs << reduce_rst1[i].first << "," << reduce_rst1[i].second << endl;
    }
    for(int i = 0; i < reduce_rst2.size(); i++){
        ofs << reduce_rst2[i].first << "," << reduce_rst2[i].second << endl;
    }
    for(int i = 0; i < reduce_rst3.size(); i++){
        ofs << reduce_rst3[i].first << "," << reduce_rst3[i].second << endl;
    }
    ofs.close();
}

int main(){
     getHamletWords();
     
     pthread_t p1, p2, p3;
     pthread_create(&p1, NULL, map1_op, NULL);
     pthread_create(&p2, NULL, map2_op, NULL);
     pthread_create(&p3, NULL, map3_op, NULL);
     pthread_join(p1, &status1);
     pthread_join(p2, &status2);
     pthread_join(p3, &status3);
     
     shuffle_op();

     pthread_t p4, p5, p6;
     pthread_create(&p4, NULL, reduce1_op, NULL);
     pthread_create(&p5, NULL, reduce2_op, NULL);
     pthread_create(&p6, NULL, reduce3_op, NULL);
     pthread_join(p4, &status1);
     pthread_join(p5, &status2);
     pthread_join(p6, &status3);
     
     printMPR(); 
     outputFile();

     return 0;
}

