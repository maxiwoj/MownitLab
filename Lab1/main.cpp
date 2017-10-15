#include <iostream>
#include <fstream>
#include <cmath>
#include <set>

#define N 10000000

float tab[N];
float approximationError[N / 25000 + 1];
float value = 0.54635;

float recurrentSum(int first, int last);
void writeToFileApproxError(float finalValue, float sum);
float kahanSum();
float iterSum();
void testdzetaRiemanna();
void zad4Float();
void zad4Double();

void zad4C();

void zad4();

using namespace std;

int main() {

    for(int i = 0 ; i < N ; i++){
        tab[i] = value;
    }
    float finalValue = value * N;


    cout<<"-------------------------------------ITER---------------------------------------------"<<endl;
    clock_t iterStart = clock();
    float sum = iterSum();
    double iterDuration = ( clock() - iterStart ) / (double) CLOCKS_PER_SEC;
    cout << "Iter Duration: " << iterDuration << endl;

    cout << "Sum: " <<sum << endl;
    cout << "Approximation Error: " << finalValue - sum << endl;
    cout << "Relative Approximation Error: " << fabs((finalValue - sum) / finalValue) * 100 << "%" <<endl;

    writeToFileApproxError(finalValue, sum);

    cout<<"-----------------------------------RECURSIVE-----------------------------------------"<<endl;
    clock_t recursiveStart = clock();
    sum = recurrentSum(0,N - 1);
    double recursiveDuration = ( clock() - recursiveStart ) / (double) CLOCKS_PER_SEC;
    cout << "Recursive Duration: " << recursiveDuration << endl;
    cout << "Sum: " <<sum << endl;
    cout << "Approximation Error: " << finalValue - sum << endl;
    cout << "Relative Approximation Error: " << fabs((finalValue - sum) / finalValue) * 100 << "%" <<endl;


    cout<<"-------------------------------------KAHAN-----------------------------------------"<<endl;
    clock_t kahanStart = clock();
    sum = kahanSum();
    double kahanDuration = ( clock() - kahanStart ) / (double) CLOCKS_PER_SEC;
    cout << "Kahan Duration: " << kahanDuration << endl;
    cout << "Sum: " <<sum << endl;
    cout << "Approximation Error: " << finalValue - sum << endl;
    cout << "Relative Approximation Error: " << fabs((finalValue - sum) / finalValue) * 100 << "%" <<endl;

    testdzetaRiemanna();

    zad4();
    zad4Float();
    zad4Double();
    zad4C();
    return 0;
}

float iterSum() {
    float sum = 0;

    for(int i = 0 ; i < N ; i++){
        sum += tab[i];
        if(i % 25000 == 0){
            approximationError[i / 25000] = ((i+1) * value - sum) / sum;
        }
    }
    return sum;
}

float recurrentSum(int first, int last) {
    if(first == last) return tab[first];
    return recurrentSum(first, (first + last)/ 2) + recurrentSum((first + last)/2 + 1, last);
}

float kahanSum() {
    float sum = 0.0f;
    float err = 0.0f;
    for (int i = 0; i < N; ++i) {
        float y = tab[i] - err;
        float temp = sum + y;
        err = (temp - sum) - y;
        sum = temp;
    }
    return sum;
}

float partialSumDzetaRiemannaForward(int n, float s) {
    float sum = 0.0f;
    for(int k = 1 ; k < n ; k++){
        sum += (1/(pow(k,s)));
    }
    return sum;
}
float partialSumDzetaRiemannaBackWard(int n, float s) {
    float sum = 0.0f;
    for(int k = n -1 ; k > 0 ; k--){
        sum += 1/(pow(k,s));
    }
    return sum;
}

float partialSumDirichletForward(int n, float s) {
    float sum = 0.0f;
    for(int k = 1 ; k < n ; k++){
        sum += pow(-1, (k-1) % 2) * (1/pow(k,s));
    }
    return sum;
}

float partialSumDirichletBackward(int n, float s) {
    float sum = 0.0f;
    for(int k = n -1 ; k > 0 ; k--){
        sum += pow(-1, (k-1) % 2) * (1/pow(k,s));
    }
    return sum;
}

void zad4(){
    ofstream myfile;
    myfile.open("zad4.txt");
    set<tuple<float, float>> values;
    for(float r = 2.5 ; r < 4 ; r+=0.0001) {
        float prev = 0.3f;
        for(int i = 0 ; i < 10000 ; i++){
            prev = r * prev * (1 - prev);
            if(i > 9900) {
                values.insert({r, prev});
            }
        }
    }
    for(tuple<float, float> f : values) {
        myfile << get<0>(f) << " " << get<1>(f) << endl;
    }
}

void zad4Float(){
    ofstream myfile;
    myfile.open("zad4Float.txt");
    set<tuple<float, float>> values;
    for(float r = 3.75 ; r <= 3.8 ; r+=0.0001) {
            float prev = 0.3f;
            for(int i = 0 ; i < 10000 ; i++){
                prev = r * prev * (1 - prev);
                if(i > 9900) {
                    values.insert({r, prev});
                }
            }
    }
    for(tuple<float, float> f : values) {
        myfile << get<0>(f) << " " << get<1>(f) << endl;
    }
}

void zad4Double(){
    ofstream myfile;
    myfile.open("zad4Double.txt");
    set<tuple<double, double>> values;
    for(double r = 3.75 ; r <= 3.8 ; r+=0.0001) {
        double prev = 0.3f;
        for(int i = 0 ; i < 10000 ; i++){
            prev = r * prev * (1 - prev);
            if(i > 9900) {
                values.insert({r, prev});
            }
        }
    }
    for(tuple<double, double> f : values) {
        myfile << get<0>(f) << " " << get<1>(f) << endl;
    }
}

void zad4C(){
    ofstream myfile;
    myfile.open("zad4C.txt");

    for(float x = 0.01 ; x < 1 ; x += 0.01){
        float prev = x;
        bool toPrint = true;
        int counter;
        for(counter = 0 ; prev > 0 ; counter ++) {
            prev = 4 * prev * (1 - prev);
            if(counter > 10000000) {
                toPrint = false;
                break;
            }
        }
        if(toPrint) myfile << x << " " << counter << endl;

    }
}

//----------------------------------------------------------

void writeToFileApproxError(float finalValue, float sum) {
    ofstream approxErrorFile;
    approxErrorFile.open("gnuPlot.txt");
    for(int i = 0 ; i < N / 25000 ; i++){
        approxErrorFile << i << " "<< approximationError[i] << endl;
    }
    approxErrorFile << N/25000 << " " << ((finalValue - sum) / finalValue);
    approxErrorFile.close();


}

void testdzetaRiemanna(){
    float s[] = {2, 3.6667, 5, 7.2, 10};
    int n[] = { 50, 100, 200, 500, 1000};
    cout<<"----------------------------- DZETA - RIEMANNA ------------------------------"<< endl;
    cout<<"n\t\t\t\ts\t\t\t\tdifference"<<endl;
    for(int i = 0 ; i < 5 ; i++){
        for(int j = 0 ; j < 5 ; j++){
            cout<< n[i] << "  \t\t\t" << s[j] << "\t\t\t" << partialSumDzetaRiemannaBackWard(n[i], s[j]) - partialSumDzetaRiemannaForward(n[i], s[j])<<endl;
        }
    }
    cout<<"---------------------------------- DIRICHLETA ------------------------------"<< endl;
    cout<<"n\t\t\t\ts\t\t\t\tdifference"<<endl;
    for(int i = 0 ; i < 5 ; i++){
        for(int j = 0 ; j < 5 ; j++){
            cout<<n[i] << "  \t\t\t" << s[j] << "\t\t\t" << partialSumDirichletBackward(n[i], s[j]) -
                    partialSumDirichletForward(n[i], s[j])<<endl;
        }
    }
}