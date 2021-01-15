#include <iostream>
#include <string>

using namespace std;

class MyClass {
    public:
        int myNum;
        string myString;

        struct testSt {
            int sampleNum;
        };

        testSt testFunc(int num);
        void updateSample(int num);

        int testInt(int num);
};

MyClass::testSt MyClass::testFunc(int num) {
    testSt t;
    t.sampleNum = num;

    return t;
}

int MyClass::testInt(int num) {
    int t;

    t = num;
    return t;
}

void MyClass::updateSample(int num) {
    // testSt t;

    testSt t = testFunc(num);
    cout << "testFunc result: " << t.sampleNum << "\n";

    int ret = testInt(num);
    cout << "testInt result: " << ret << "\n";

}


int main() {
    MyClass myObj;

    myObj.myNum = 15;
    myObj.myString = "Some text";

    cout << myObj.myNum << "\n";
    cout << myObj.myString << "\n";

    myObj.updateSample(3);

    return 0;
}