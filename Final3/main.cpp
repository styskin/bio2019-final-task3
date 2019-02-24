//
//  main.cpp
//  Final3
//
//  Created by Andrey Styskin on 23.02.2019.
//  Copyright © 2019 Andrey Styskin. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <math.h>
using namespace std;

typedef pair<int, string> pis;
typedef vector<string> TVS;


void split(const string& o, TVS& v) {
    istringstream iss(o);
    string s;
    while ( getline( iss, s, ' ' ) ) {
        v.push_back(s);
    }
}

int calc_diff(const string& orig, const string& dst) {
    int diff = 0;
    for (int j = 0; j < orig.length(); ++j) {
        if (orig[j] != dst[j]) {
            ++diff;
        }
    }
    return diff;
}


int find_min(const string& orig, const vector<pis>& top, int start, int end) {
    int mx = 1000000;
    int ind = start;
    for (int i = start; i < end; ++i) {
        int diff = calc_diff(orig, top[i].second);
        if (diff < mx) {
            mx = diff;
            ind = i;
        }
    }
    return ind;
}


void order(ostream& out, const vector<TVS>& M) {
    map<string, int> count;
    map<string, int> distinct_count;

    const size_t FEATURES_COUNT = 10;
    
    
    
    vector<vector<double> > A(M.size(), vector<double>(FEATURES_COUNT) );
    
    map<string, double> epoh;
    vector<pis> top_backeries;

    for (int i = 0; i < M.size(); ++i) {
        map<string, int> cc;
        for (int j = 0; j < M[i].size(); ++j) {
            ++count[M[i][j]];
            ++cc[M[i][j]];
        }
        vector<pis> local;
        for (const auto& a : cc) {
            local.push_back(pis(-a.second, a.first));
        }
        sort(local.begin(), local.end());
//        cout << local[0].first << " " << local[0].second << endl;
        top_backeries.push_back(local[0]);
        A[i][0] = 1.0 * cc.size() / M[i].size();
    }

//    for (const auto& a : count) {
//        top_backeries.push_back(pis(-a.second, a.first));
//    }
    sort(top_backeries.begin(), top_backeries.end());

    int E = top_backeries.size();

    string origin = top_backeries[0].second;
    // Calculate distances from origin
    for (int st = 1; st < E; ++st) {
        epoh[top_backeries[st-1].second] = st;
        cout << top_backeries[st - 1].second << " " << top_backeries[st-1].first << endl;
        int ind = find_min(top_backeries[st - 1].second, top_backeries, st, E);
        swap(top_backeries[st], top_backeries[ind]);
    }
    
//    for (int i = 0; i < M.size(); ++i) {
//        for (int j = 0; j < M[i].size(); ++j) {
//            for (auto a : epoh) {
//                if (calc_diff(a.first, M[i][j]) < 2) {
//                    epoh[M[i][j]] = a.second + 1;
//                }
//            }
//        }
//    }
    vector< vector<int> > X(M.size());
    for (int i = 0; i < M.size(); ++i) {
        for (int j = 0; j < M[i].size(); ++j) {
            A[i][3] += 1.0 * calc_diff(origin, M[i][j]) / M[i].size();
            if (epoh.find(M[i][j]) != epoh.end()) {
                A[i][2] = max(A[i][2], epoh[M[i][j]]);
                X[i].push_back(epoh[M[i][j]]);
                A[i][1] += epoh[M[i][j]];
            }
        }
        sort(X[i].begin(), X[i].end());
//        for (int j = 0; j < X[i].size(); ++j) {
//            if (j > 0) {
//                out << " ";
//            }
//            out << X[i][j];
//        }
//        out << endl;
    }
    vector<pair<double, int> > r;
    for (int i = 0; i < A.size(); ++i) {
        for (int f = 0; f < 3; ++f) {
            if (f > 0) {
                cout << " ";
            }
            cout << A[i][f];
        }
        cout << endl;
//        r.push_back(pair<double, int> (-(1.0 / A[i][0] + 20. / max(20., A[i][2]) ), i) );
//        r.push_back(pair<double, int> (-(100. / (A[i][2] < 1 ? 200 : A[i][2]) ), i) );
        r.push_back(pair<double, int> (A[i][3], i) );
    }
    sort(r.begin(), r.end());
//    for (int i = 0; i < r.size(); ++i)  {
//        out << r[i].first << " : " << r[i].second << endl;
//        int ind = r[i].second;
//        for (int j = 0; j < X[ind].size(); ++j) {
//            if (j > 0) {
//                out << " ";
//            }
//            out << X[ind][j];
//        }
//        out << endl;
//    }

    cout << "ANSWER" << endl;
    for (int i = 0; i < r.size(); ++i)  {
        if (i > 0) {
            out << " ";
        }
        out << r[i].second;
        
        for (int f = 0; f < 4; ++f) {
            if (f > 0) {
                cout << " ";
            }
            cout << A[r[i].second][f];
        }
        cout << endl;

        
    }
    out << endl;
}

int main(int argc, const char * argv[]) {
    for (int TT = 2; TT <= 8; ++TT) {
        ifstream in;
        string path("/Users/styskin/bio2019/Final3/");
        in.open(path + to_string(TT) + ".txt");
        ofstream out;
        out.open(path + to_string(TT) + ".out");
        int T;
        in >> T;
        for (int t = 0; t < T; ++t) {
            int N;
            in >> N;
            vector<TVS> M;
            
            string str;
            getline(in, str);
            for (int j = 0; j < N; ++j) {
                getline(in, str);
                TVS v;
                split(str, v);
                M.push_back(v);
            }
            order(out, M);
        }
    }
    return 0;
}
