//
// Created by lin on 2021/12/26.
//
#include "mini_mlcs.h"

void printMLCSs(vector<vector<struct Point_vec>> D,int k){
    string tmp;
    int s_count = 0;
    for(int i=0;i<D[k].size();i++){
        tmp = D[k][i].s;
        reverse(tmp.begin(),tmp.end());
        s_count++;
        cout << tmp << endl;
    }
    cout << s_count << endl;
}

int computer_level(vector<string> res){
    //定义序列的字符
    char ch[4]{'A', 'C', 'G', 'T'};
    vector<char> sigma(ch, ch + 4);
    vector<vector<vector<int>>> ST;
    contructSuccessorTable(res, ST, sigma);
    return DAG_len(res,0,ST,sigma);
}

void mini_DAG_1_1(vector<string> res, int lower_mlcs, vector<vector<vector<int>>> ST, vector<char> sigma){
    int gl,del,max;
    int level = 0;
    vector<int> tv(res.size(), 0);//匹配点
    vector<vector<struct Point_vec>> D;
    vector<struct Point_vec> nextD;
    vector<int> init(res.size(), 0);
    int k = 0;
    struct Point_vec initpv;
    struct Point_vec tmppv;
    initpv.p = init;
    initpv.s = "";
    nextD.push_back(initpv);
    D.push_back(nextD);
    while (D[k].size() != 0) {
        nextD.clear();
        gl = 0;
        for (int i = 0; i < D[k].size(); i++) {
            if(k>=2){
                D[k-2].clear();
            }
            initpv = D[k][i];
            for (int j = 0; j < sigma.size(); j++){
                del = 0;
                max = INT32_MIN;
                for (int r = 0; r < res.size(); r++){
                    if (ST[r][j][initpv.p[r]] == -1) {
                        del = 1;
                        break;
                    }
                    tv[r] = ST[r][j][initpv.p[r]];

                    if(tv[r] > max){
                        max = tv[r];
                        //Theorem 1
                        if(level + 1 + res[r].size() - max < lower_mlcs){
                            del = 1;
                            gl++;
                            break;
                        }
                    }
                }

                if (del == 1)
                    continue;



                tmppv.p = tv;
                tmppv.s = sigma[j] + initpv.s;
                nextD.push_back(tmppv);
            }
        }
        cout << "第" << k + 1 << "层过滤了" << gl << "个点" << endl;
        D.push_back(nextD);
        if (nextD.size() != 0) {
            level++;
            cout << "level:" << level << endl;
            cout << "num:" << nextD.size() << endl;
        }
        k++;
    }
    if(k>=2){
        D[k-2].clear();
    }
    printMLCSs(D,k-1);
}

void mini_DAG_1(vector<string> res, int lower_mlcs, vector<vector<vector<int>>> ST, vector<char> sigma,int q){

    int gl,del,max;
    int level = 0;
    priority_queue<struct diversity_index, vector<struct diversity_index>, cmp> Q;
    vector<int> index;
    vector<string> lres;
    struct diversity_index dmax;
    vector<int> tv(res.size(), 0);//匹配点
    vector<vector<struct Point_vec>> D;
    vector<struct Point_vec> nextD;
    vector<int> init(res.size(), 0);
    int k = 0;
    struct Point_vec initpv;
    struct Point_vec tmppv;
    initpv.p = init;
    initpv.s = "";
    nextD.push_back(initpv);
    D.push_back(nextD);
    while (D[k].size() != 0) {
        nextD.clear();
        gl = 0;
        for (int i = 0; i < D[k].size(); i++) {
            if(k>=2){
                D[k-2].clear();
            }
            initpv = D[k][i];
            for (int j = 0; j < sigma.size(); j++){
                del = 0;
                max = INT32_MIN;
                lres.clear();
                while(Q.size() > 0){
                    Q.pop();
                }
                for (int r = 0; r < res.size(); r++){
                    if (ST[r][j][initpv.p[r]] == -1) {
                        del = 1;
                        break;
                    }
                    tv[r] = ST[r][j][initpv.p[r]];

                    if(tv[r] > max){
                        max = tv[r];
                        //Theorem 1
                        if(level + 1 + res[r].size() - max < lower_mlcs){
                            del = 1;
                            gl++;
                            break;
                        }
                    }

                    if(r < q){
                        dmax.diversity = tv[r];
                        dmax.index = r;
                        Q.push(dmax);
                    } else{
                        if (Q.top().diversity < tv[r]) {
                            dmax.diversity = tv[r];
                            dmax.index = r;
                            Q.pop();
                            Q.push(dmax);
                        }
                    }
                }

                if (del == 1)
                    continue;


                for (int i = 0; i < q; i++) {
                    lres.push_back(res[Q.top().index].substr(Q.top().diversity,res[Q.top().index].size()));
//                    index.push_back(Q.top().index);
//            cout << Q.top().diversity << endl;
                    Q.pop();
                }

                if(level + 1 + computer_level(lres) < lower_mlcs){
                    gl++;
                    continue;
                }

                tmppv.p = tv;
                tmppv.s = sigma[j] + initpv.s;
                nextD.push_back(tmppv);
            }
        }
        cout << "第" << k + 1 << "层过滤了" << gl << "个点" << endl;
        D.push_back(nextD);
        if (nextD.size() != 0) {
            level++;
            cout << "level:" << level << endl;
            cout << "num:" << nextD.size() << endl;
        }
        k++;
    }
    if(k>=2){
        D[k-2].clear();
    }
    printMLCSs(D,k-1);
}

void mini_DAG(vector<string> res, int lower_mlcs, vector<vector<vector<int>>> ST, vector<char> sigma, vector<int> index, vector<vector<int>> a,vector<int> m){

    int gl,del,min,index_count;
    int level = 0;
    vector<int> tv(res.size(), 0);//匹配点
    vector<vector<struct Point_vec>> D;
    vector<struct Point_vec> nextD;
    vector<int> init(res.size(), 0);
    int k = 0;
    struct Point_vec initpv;
    struct Point_vec tmppv;
    initpv.p = init;
    initpv.s = "";
    nextD.push_back(initpv);
    D.push_back(nextD);
    while (D[k].size() != 0) {
        nextD.clear();
        gl = 0;
        for (int i = 0; i < D[k].size(); i++) {
            if(k>=2){
                D[k-2].clear();
            }
            initpv = D[k][i];
            for (int j = 0; j < sigma.size(); j++){
                del = 0;
                min = INT32_MAX;
                index_count=0;
                for (int r = 0; r < res.size(); r++){
                    if (ST[r][j][initpv.p[r]] == -1) {
                        del = 1;
                        break;
                    }
                    tv[r] = ST[r][j][initpv.p[r]];

                    //Theorem 1
                    if(level + 1 + res[r].size() - tv[r] < lower_mlcs){
                        del = 1;
                        gl++;
                        break;
                    }

                    //Theorem 3
                    if(r == index[index_count]){
                        if(level + 1 + a[index_count][tv[r]-1] < lower_mlcs){
                            del = 1;
                            gl++;
                            break;
                        }
                        index_count++;
                    }

                    if(tv[r] < min){
                        min = tv[r];
                    }
                }

                if (del == 1)
                    continue;

                //Theorem 2
                if(level + 1 + m[min-1] < lower_mlcs){
                    gl++;
                    continue;
                }

                tmppv.p = tv;
                tmppv.s = sigma[j] + initpv.s;
                nextD.push_back(tmppv);
            }
        }
        cout << "第" << k + 1 << "层过滤了" << gl << "个点" << endl;
        D.push_back(nextD);
        if (nextD.size() != 0) {
            level++;
            cout << "level:" << level << endl;
            cout << "num:" << nextD.size() << endl;
        }
        k++;
    }
    if(k>=2){
        D[k-2].clear();
    }
    printMLCSs(D,k-1);
}


int deal_with_theorem3(vector<string> T,vector<int> index,vector<vector<int>> &a,int i,int t,vector<int> r_index,int r,int true_l_mlcs){
    //定义序列的字符
    char ch[4]{'A', 'C', 'G', 'T'};
    vector<char> sigma(ch, ch + 4);
    vector<vector<vector<int>>> ST;
    vector<string> res;
    int lower_mlcs;
    int r_count = 0;
    for(int j=0;j<r_index.size();j++){
        for(int k=0;k<index.size();k++){
            if(r_index[j]==index[k]){
                res.push_back(T[index[k]].substr(i,T[index[k]].size()));
            }else{
                res.push_back(T[index[k]]);
            }
        }
        //构建ST表
        contructSuccessorTable(res, ST, sigma);
        lower_mlcs = compute_lower_mlcs(ST, sigma, res, 10);

        if(i!=T[0].size()){
            cout << "a" << a[j][i] << endl;
            if(a[j][i] >= true_l_mlcs - 4){
                r_count++;
            }
        }

        a[j][i-1] = DAG_len(res,lower_mlcs,ST, sigma);
        ST.clear();
        res.clear();
    }
    if(r_count == r){
        return 1;
    }else{
        return 0;
    }
}

int DAG_len(vector<string> res, int lower_mlcs, vector<vector<vector<int>>> ST, vector<char> sigma) {

    vector<int> init(res.size(), 0);//初始点
    int k = 0;//当前处理的level
    int del;//判断是不是有效的匹配点
    int level = 0;
    vector<set<vector<int>>> D;
    set<vector<int>> nextD;
    vector<int> tv(res.size(), 0);//匹配点
    D.push_back(nextD);
    D[k].insert(init);
    vector<int> nowrt;
    set<vector<int>>::iterator it;

    while (D[k].size() != 0) {
        if (k >= 2) {
            D[k - 2].clear();
        }
        nextD.clear();
        for (it = D[k].begin(); it != D[k].end(); it++) {
            nowrt = *(it);
            for (int j = 0; j < sigma.size(); j++) {
                del = 0;
                for (int r = 0; r < res.size(); r++) {
                    if (ST[r][j][nowrt[r]] == -1) {
                        del = 1;
                        break;
                    }
                    tv[r] = ST[r][j][nowrt[r]];
                    if(level + 1 + res[r].size() - tv[r] < lower_mlcs){
                        del = 1;
                        break;
                    }
                }
                if (del == 1)
                    continue;
                nextD.insert(tv);
            }
        }

        D.push_back(nextD);
        if (nextD.size() != 0) {
            level++;
        }
        k++;
    }
    return level;
}

void compute_upper_bound(vector<string> T, int d, vector<int> &m, vector<vector<int>> &a,int t,int r,int e) {
    //定义序列的字符
    char ch[4]{'A', 'C', 'G', 'T'};
    vector<char> sigma(ch, ch + 4);
    vector<vector<vector<int>>> ST;
    int lower_mlcs;
    //从T.size()里面抽取d个序列
    vector<int> index;
    vector<int> r_index;
    priority_queue<struct diversity_index, vector<struct diversity_index>, cmp> Q;
    computer_sequence(T, d, index);
    for(int i=0;i<r-1;i++){
        r_index.push_back(index[i]);
    }
    r_index.push_back(0);
    index.push_back(0);
    sort(index.begin(), index.end());
    sort(r_index.begin(), r_index.end());
    vector<string> min_res;
    vector<string> max_res;
    //开始时间
    struct timeval tvs, tve;
    gettimeofday(&tvs, NULL);
    //构建ST表
    contructSuccessorTable(T, ST, sigma);
    lower_mlcs = compute_lower_mlcs(ST, sigma, T, t);
    if(e==1){
        mini_DAG_1_1(T,lower_mlcs,ST,sigma);
        //结束时间
        gettimeofday(&tve, NULL);
        double span = tve.tv_sec - tvs.tv_sec
                      + (tve.tv_usec - tvs.tv_usec) / 1000000.0;
        cout << "end time is: " << span << endl;
        return;;
    }
    int true_l_mlcs = lower_mlcs;
    ST.clear();
    int flag = 0;
    for (int i = T[0].size(); i >=1; i--) {
        cout << i << endl;
//        cout << m[i] << endl;
//        if(i<T[0].size()-1 && (m[i] == true_l_mlcs - 4 || m[i] == true_l_mlcs)){
//            m[i-1] = true_l_mlcs;
//        }else{
//            for (int j = 0; j < index.size(); j++) {
//                min_res.push_back(T[index[j]].substr(i, T[index[j]].size()));
//            }
////        cout << min_res.size() << endl;
//            //构建ST表
//            contructSuccessorTable(min_res, ST, sigma);
//            lower_mlcs = compute_lower_mlcs(ST, sigma, min_res, t);
//            m[i-1] = DAG_len(min_res,lower_mlcs,ST, sigma);
//        }

        cout << flag << endl;
        if(flag != 1)
            flag = deal_with_theorem3(T,index,a,i,t,r_index,r,true_l_mlcs);

        min_res.clear();
        ST.clear();
    }

    contructSuccessorTable(T, ST, sigma);

    mini_DAG(T,true_l_mlcs,ST,sigma,r_index,a,m);
    //结束时间
    gettimeofday(&tve, NULL);
    double span = tve.tv_sec - tvs.tv_sec
                  + (tve.tv_usec - tvs.tv_usec) / 1000000.0;
    cout << "end time is: " << span << endl;
}

void computer_sequence(vector<string> T, int d, vector<int> &index) {
    vector<vector<double>> c(T.size(), vector<double>(4, 0));
    for (int i = 0; i < T.size(); i++) {
        for (int j = 0; j < T[i].size(); j++) {
            switch (T[i][j]) {
                case 'A':
                    c[i][0]++;
                    break;
                case 'C':
                    c[i][1]++;
                    break;
                case 'G':
                    c[i][2]++;
                    break;
                case 'T':
                    c[i][3]++;
                    break;
                default:
                    break;
            }
        }
    }
    //寻找和第一条序列差异性最大的序列
    //计算第一个序列和其他序列的差异
    struct diversity_index di;
    double sum = c[0][0] + c[0][1] + c[0][2] + c[0][3];
    double diversity;
    priority_queue<struct diversity_index, vector<struct diversity_index>, cmp> Q;
    for (int i = 0; i < d; i++) {
        diversity = (c[0][0] / sum) * abs(c[0][0] - c[i][0]) + (c[0][1] / sum) * abs(c[0][1] - c[i][1]) +
                    (c[0][2] / sum) * abs(c[0][2] - c[i][2]) + (c[0][3] / sum) * abs(c[0][3] - c[i][3]);
//            cout << diversity << endl;
        di.diversity = diversity;
        di.index = i;
        Q.push(di);
    }

    for (int i = d; i < T.size(); i++) {
        diversity = (c[0][0] / sum) * abs(c[0][0] - c[i][0]) + (c[0][1] / sum) * abs(c[0][1] - c[i][1]) +
                    (c[0][2] / sum) * abs(c[0][2] - c[i][2]) + (c[0][3] / sum) * abs(c[0][3] - c[i][3]);
//        cout << diversity << endl;
        di.diversity = diversity;
        di.index = i;
        if (Q.top().diversity < diversity) {
            Q.pop();
            Q.push(di);
        }
    }
    for (int i = 0; i < d; i++) {
        index.push_back(Q.top().index);
//            cout << Q.top().diversity << endl;
        Q.pop();
    }
}

int compute_lower_mlcs(vector<vector<vector<int>>> ST, vector<char> sigma, vector<string> T, int t) {
    vector<set<vector<int>>> D;//存放每层的结果
    multimap<int, vector<int>> m;
    int m_count;
    set<vector<int>> init;//初始层
    vector<int> initPoint(T.size(), 0);
    init.insert(initPoint);
    D.push_back(init);//初始层
    int k = 0;
    vector<int> tmp;
    int del;//判断当前后继点是不是有效点
    vector<int> tv(T.size(), 0);
    vector<int> cp(T.size(), 0);
    int max, min;
    vector<int> minPoint;
    set<vector<int>> nextD;
    int lmlcs = 0;
    set<vector<int>>::iterator it;
    while (D[k].size() != 0) {
        nextD.clear();
        it = nextD.begin();
        for (it = D[k].begin(); it != D[k].end(); it++) {
            tmp = *(it);//当前点
            minPoint.clear();
            //根据当前点计算下一层的点
            for (int j = 0; j < sigma.size(); j++) {
                del = 0;
                max = INT32_MIN;
                min = INT32_MAX;
                for (int k = 0; k < T.size(); k++) {
                    if (ST[k][j][tmp[k]] == -1) {
                        del = 1;
                        break;
                    }
                    tv[k] = ST[k][j][tmp[k]];
                    if (tv[k] > max) {
                        if (max != INT32_MIN && min == INT32_MAX) {
                            min = max;
                        }
                        max = tv[k];
                    } else if (tv[k] < min) {
                        min = tv[k];
                    }

                }

                if (del == 1)
                    continue;
                m.insert(pair<int, vector<int>>(max - min, tv));
                nextD.insert(tv);
            }
        }
        if (nextD.size() != 0) {
            lmlcs++;
//            cout << lmlcs << endl;
        }
        if (m.size() >= t) {
            nextD.clear();
            m_count = 0;
            multimap<int, vector<int>>::iterator it;
            for (it = m.begin(); it != m.end(); it++) {
                if (m_count == t) {
                    break;
                }
                nextD.insert((*it).second);
                m_count++;
            }
        }
        //非支配排序
//        nextD = normalMethod1(nextD);
        D.push_back(nextD);
        m.clear();
        k++;
    }
    D.push_back(init);
    if (lmlcs == 0)
        return INT32_MAX;
    return lmlcs;
}