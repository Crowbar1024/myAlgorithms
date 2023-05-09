#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <time.h>
#include "MyList.h"

using namespace std;


int main() {
    vector<int> arr({1,2,3,6,3,2,1});
    MyList* obj = new MyList(arr);
    
    obj->printList();
    // cout << obj->getSize() << endl;
    system("pause");
    return 0;
}