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
vector<pair<string, int> > map_rst;

map<string, vector<int> > shuffle_rst; //all <word, [1,1,1...,1]>
map<string, vector<int> > shuffle_rst1; //all <word, [1,1,1...,1]>
map<string, vector<int> > shuffle_rst2; //all <word, [1,1,1...,1]>
map<string, vector<int> > shuffle_rst3; //all <word, [1,1,1...,1]>

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

bool cmpPair(const pair<string, int> &a, const pair<string, int> &b){
   return a.first < b.first;
}

void shufflePre(){
    for(int i = 0; i < map_rst1.size(); i++)
        map_rst.push_back(map_rst1[i]);
    for(int i = 0; i < map_rst2.size(); i++)
        map_rst.push_back(map_rst2[i]);
    for(int i = 0; i < map_rst3.size(); i++)
        map_rst.push_back(map_rst3[i]);
    sort(map_rst.begin(), map_rst.end(), cmpPair);
    for(int i = 0; i < map_rst.size(); i++)
        cout << map_rst[i].first << endl;
}

void* shuffle1_op(void*){
    int block_size = map_rst.size()/3;
    for(int i = 0; i < block_size; i++){
        string m_key = map_rst[i].first;
        shuffle_rst1[m_key].push_back(1);
    }
}
void* shuffle2_op(void*){
    int block_size = map_rst.size()/3;
    for(int i = block_size; i < 2*block_size; i++){
        string m_key = map_rst[i].first;
        shuffle_rst2[m_key].push_back(1);
    }
}
void* shuffle3_op(void*){
    int block_size = map_rst.size()/3;
    for(int i = 2*block_size; i < map_rst.size(); i++){
        string m_key = map_rst[i].first;
        shuffle_rst3[m_key].push_back(1);
    }
}

void* reduce1_op(void*){
    for(int i = 0; i < shuffle_rst1.size(); i++){
        string m_key = m_dict[i];
        int m_cnt = shuffle_rst1[m_key].size();
        pair<string, int> tmp(m_key, m_cnt);
        reduce_rst1.push_back(tmp);
    }
}
void* reduce2_op(void*){
    for(int i = 0; i < shuffle_rst2.size(); i++){
        string m_key = m_dict[i];
        int m_cnt = shuffle_rst2[m_key].size();
        pair<string, int> tmp(m_key, m_cnt);
        reduce_rst2.push_back(tmp);
    }
}
        
void* reduce3_op(void*){
    for(int i = 0; i < shuffle_rst3.size(); i++){
        string m_key = m_dict[i];
        int m_cnt = shuffle_rst3[m_key].size();
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
     
     shufflePre();    
     pthread_t p7, p8, p9;
     pthread_create(&p7, NULL, shuffle1_op, NULL);
     pthread_create(&p8, NULL, shuffle2_op, NULL);
     pthread_create(&p9, NULL, shuffle3_op, NULL);
     pthread_join(p7, &status1);
     pthread_join(p8, &status2);
     pthread_join(p9, &status3);

     pthread_t p4, p5, p6;
     pthread_create(&p4, NULL, reduce1_op, NULL);
     pthread_create(&p5, NULL, reduce2_op, NULL);
     pthread_create(&p6, NULL, reduce3_op, NULL);
     pthread_join(p4, &status1);
     pthread_join(p5, &status2);
     pthread_join(p6, &status3);
     
 //    printMPR(); 
     outputFile();

     return 0;
}

