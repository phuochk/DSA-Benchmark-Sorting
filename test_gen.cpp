#include <iostream>
#include <string>
#include <vector>
#include <random>

using namespace std;

void gen_int_test(int test_num) {
    int N = 100000;
    cout << N << "\n";

    if (test_num == 1) {
        mt19937 rng(1337);
        uniform_int_distribution<int> dist(-2147483648, 2147483647);
        for(int i = 0; i < N; i++) cout << dist(rng) << "\n";
    }
    else if (test_num == 2) {
        for(int i = 0; i < N / 2; i++) {
            cout << 2147483647 - i << "\n";
            cout << -2147483648 + i << "\n";
        }
    }
    else if (test_num == 3) {
        mt19937 rng(42);
        uniform_int_distribution<int> dist(0, 2);
        int vals[3] = {-1, 0, 1};
        for(int i = 0; i < N; i++) cout << vals[dist(rng)] << "\n";
    }
    else if (test_num == 4) {
        for(int i = 0; i < N; i++) cout << N - i << "\n";
    }
    else if (test_num == 5) {
        vector<int> v(N);
        for(int i = 0; i < N; i++) v[i] = i;
        mt19937 rng(99);
        uniform_int_distribution<int> dist(0, N - 1);
        for(int i = 0; i < 1000; i++) swap(v[dist(rng)], v[dist(rng)]);
        for(int i = 0; i < N; i++) cout << v[i] << "\n";
    }
}

void gen_strlexi_test(int test_num) {
    int N = 100000;
    cout << N << "\n";
    mt19937 rng(1337);

    if (test_num == 1) {
        string prefix(98, 'a');
        uniform_int_distribution<int> dist(0, 25);
        for(int i = 0; i < N; i++) {
            cout << prefix << (char)('a' + dist(rng)) << (char)('a' + dist(rng)) << "\n";
        }
    }
    else if (test_num == 2) {
        string clone(100, 'x');
        for(int i = 0; i < N; i++) cout << clone << "\n";
    }
    else if (test_num == 3) {
        uniform_int_distribution<int> dist(0, 1);
        for(int i = 0; i < N; i++) {
            string s(100, 'a');
            for(int j = 0; j < 100; j++) s[j] = (dist(rng) == 0 ? 'a' : 'b');
            cout << s << "\n";
        }
    }
    else if (test_num == 4) {
        for(int i = 0; i < N; i++) cout << string(10 + (i % 91), 'a') << "\n";
    }
    else if (test_num == 5) {
        for(int i = 0; i < N; i++) cout << string(100, (char)('z' - (i % 26))) << "\n";
    }
}

void gen_strlenlexi_test(int test_num) {
    int N = 10000;
    cout << N << "\n";
    mt19937 rng(1337);

    if (test_num == 1) {
        for(int i = 0; i < N; i++) {
            if (i % 2 == 0) cout << string(10, 'z') << "\n";
            else cout << string(100, 'a') << "\n";
        }
    }
    else if (test_num == 2) {
        uniform_int_distribution<int> dist(0, 25);
        for(int i = 0; i < N; i++) {
            string s(100, 'a');
            for(int j = 0; j < 100; j++) s[j] = (char)('a' + dist(rng));
            cout << s << "\n";
        }
    }
    else if (test_num == 3) {
        for(int i = 0; i < N; i++) {
            string s(100, 'a');
            s[i % 100] = 'b';
            cout << s << "\n";
        }
    }
    else if (test_num == 4) {
        uniform_int_distribution<int> len_dist(10, 100);
        for(int i = 0; i < N; i++) {
            int len = len_dist(rng);
            string s(len, 'a');
            for(int j = 0; j < len; j++) s[j] = (char)('a' + (j % 3));
            cout << s << "\n";
        }
    }
    else if (test_num == 5) {
        string pool[5] = { string(10, 'x'), string(50, 'y'), string(100, 'z'), string(15, 'a'), string(99, 'b') };
        uniform_int_distribution<int> dist(0, 4);
        for(int i = 0; i < N; i++) cout << pool[dist(rng)] << "\n";
    }
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (argc != 3) return 1;

    string type = argv[1];
    int test_num = stoi(argv[2]);

    if (test_num < 1 || test_num > 5) return 1;

    if (type == "int") {
        gen_int_test(test_num);
    } 
    else if (type == "strlexi") {
        gen_strlexi_test(test_num);
    } 
    else if (type == "strlenlexi") {
        gen_strlenlexi_test(test_num);
    } 
    else {
        return 1;
    }

    return 0;
}