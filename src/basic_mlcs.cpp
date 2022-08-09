//
// Created by lin on 2021/12/26.
//
#include "basic_mlcs.h"

void ReadSeq(char **seq1,uint32_t *seq_length,char* p_ref)
{
    uint32_t cursize;
    uint32_t maxsize;
    uint32_t addsize;

    maxsize=pow(2,20);
    addsize=pow(2,20);

    char *seq;
    seq=(char*) malloc (sizeof(char)*maxsize);
    cursize=maxsize;

    uint32_t buffer_size=256;
    char buffer_line[256];
    memset(buffer_line,0,buffer_size);

    FILE *fp;
    fp = fopen(p_ref,"r+");
    if(fp==NULL)
    {
        cout <<"file can not be open!" << endl;
    }

    uint32_t len=0;
    uint32_t cycle=0;
    while (fgets(buffer_line,buffer_size-1,fp)!=NULL)
    {
        if(buffer_line[0]=='>')
            continue;
        else
        {
            if(len+buffer_size<cursize)
            {
                for(uint32_t i=0;i<buffer_size;i++)
                {
                    if(buffer_line[i]=='\n'||buffer_line[i]=='\0')
                    {
                        break;
                    }
                    if(buffer_line[i]>='a')
                    {
                        buffer_line[i]-=32;
                    }
                    if(buffer_line[i]!='A'&&buffer_line[i]!='C'&&buffer_line[i]!='G'&&buffer_line[i]!='T')
                    {
                        continue;
                    }
                    seq[len]=buffer_line[i];
                    len++;
                }
            }
            else
            {
                seq=(char*) realloc (seq,sizeof(char)*(cursize+addsize));
                cursize=cursize+addsize;
                for(uint32_t i=0;i<buffer_size;i++)
                {
                    if(buffer_line[i]=='\n'||buffer_line[i]=='\0')
                    {
                        break;
                    }
                    if(buffer_line[i]>='a')
                    {
                        buffer_line[i]-=32;
                    }
                    if(buffer_line[i]!='A'&&buffer_line[i]!='C'&&buffer_line[i]!='G'&&buffer_line[i]!='T')
                    {
                        buffer_line[i]='A';
                    }
                    seq[len]=buffer_line[i];
                    len++;
                }
//				cout <<"add 1 MB for seq: " << cycle++ <<endl;
            }
        }
        memset(buffer_line,0,buffer_size);
    }
    *seq_length=len;
    *seq1=seq;
    cout << "the length of seq is: " << len << endl;
}

void gernerateData(char *ref_path,char *sav_path,uint32_t n, uint32_t data_len){

    char * ref;
    uint32_t len;
    ReadSeq(&ref,&len,ref_path);
    ofstream fout(sav_path);
    fout << n << endl;
    char *a;
    a = (char *)malloc(sizeof(char) * data_len);
    uint32_t j=0;
    uint32_t n_count=0;
    for(uint32_t i=0;i<n*data_len;i++){

        a[j]=ref[i];
        if(j==data_len-1){
            fout << a << endl;
            j=0;
            n_count++;
            continue;
        }
        j++;
    }

    fout.close();
}


void readRef(vector<string> &T, char *ref_path) {
    ifstream fin(ref_path);
    if (!fin) {
        cout << "can not find file" << endl;
        return;
    }
    string input_s;
    int n;
    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> input_s;
        T.push_back(input_s);
    }
    fin.close();
}

void contructSuccessorTable(vector<string> T, vector<vector<vector<int>>> &ST, vector<char> sigma) {
    string s;//当前序列
    queue<int> Q[36];
    //序列的个数
    for (int i = 0; i < T.size(); i++) {
        vector<vector<int>> v2d;
        v2d.clear();
        s = " " + T[i];//保证ST表对应的序列位置从1开始
        //将对应字符在序列中的位置放入Q中
        for (int j = 1; j < s.size(); j++) {
            Q[s[j] - 'A'].push(j);
        }
        //对应字符表的每一个字符 也就是ST表的行
        for (int j = 0; j < sigma.size(); j++) {
            vector<int> v1d;
            v1d.clear();
            //对应ST表的列
            for (int k = 0; k < s.size(); k++) {
                if (s[k] != sigma[j] && !Q[sigma[j] - 'A'].empty()) {
                    v1d.push_back(Q[sigma[j] - 'A'].front());
                    continue;
                } else if (s[k] == sigma[j] && !Q[sigma[j] - 'A'].empty()) {
                    Q[sigma[j] - 'A'].pop();
                    if (!Q[sigma[j] - 'A'].empty()) {
                        v1d.push_back(Q[sigma[j] - 'A'].front());
                    } else {
                        v1d.push_back(-1);
                    }
                } else {
                    while (k < s.size()) {
                        v1d.push_back(-1);
                        k++;
                    }
                }
            }
            v2d.push_back(v1d);
        }
        ST.push_back(v2d);
    }
}