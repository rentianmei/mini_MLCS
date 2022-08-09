//
// Created by lin on 2021/12/26.
//
#include "basic_mlcs.h"
#include "mini_mlcs.h"

int main(int argc, char **argv) {
    char *ref_path;
    char *sav_path;
    int t,d,N,L,r,e;
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'R') {
            ref_path = argv[i + 1];
        }
        if (argv[i][0] == '-' && argv[i][1] == 's') {
            sav_path = argv[i + 1];
        }
        if (argv[i][0] == '-' && argv[i][1] == 'N') {
            N = atoi(argv[i + 1]);
        }
        if (argv[i][0] == '-' && argv[i][1] == 'L') {
            L = atoi(argv[i + 1]);
        }
        if (argv[i][0] == '-' && argv[i][1] == 'g') {
            gernerateData(ref_path,sav_path,N,L);
        }
        if (argv[i][0] == '-' && argv[i][1] == 't') {
            t = atoi(argv[i + 1]); //每一层匹配点最多个数
        }
        if (argv[i][0] == '-' && argv[i][1] == 'd') {
            d = atoi(argv[i + 1]); //抽取的序列个数
        }
        if (argv[i][0] == '-' && argv[i][1] == 'r') {
            r = atoi(argv[i + 1]); //计算r个匹配点的位置
        }
        if (argv[i][0] == '-' && argv[i][1] == 'e') {
            e = atoi(argv[i + 1]); //用哪种方法
        }
        if (argv[i][0] == '-' && argv[i][1] == 'l') {
            //求下界
            //读序列
            vector<string> T;
            readRef(T, ref_path);
            //定义序列的字符
            char ch[4]{'A', 'C', 'G', 'T'};
            vector<char> sigma(ch, ch + 4);
            vector<vector<vector<int>>> ST;
            //构建ST表
            contructSuccessorTable(T, ST, sigma);
            //开始时间
            struct timeval tvs, tve;
            gettimeofday(&tvs, NULL);
            int lower_mlcs = compute_lower_mlcs(ST, sigma, T, t);
            //结束时间
            gettimeofday(&tve, NULL);
            double span = tve.tv_sec - tvs.tv_sec
                          + (tve.tv_usec - tvs.tv_usec) / 1000000.0;
            cout << "end time is: " << span << endl;
            cout << lower_mlcs << endl;
        }
        if (argv[i][0] == '-' && argv[i][1] == 'u'){
            //读序列
            vector<string> T;
            readRef(T, ref_path);
            vector<int> m(T[0].size(),100);
            vector<vector<int>> a(r,vector<int>(T[0].size(),100));
            compute_upper_bound(T,d,m,a ,t,r,e);
        }
    }
}