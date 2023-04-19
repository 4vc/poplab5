#include <iostream>
#include <climits>
#include <omp.h>

using namespace std;

const int ROWS = 1000;
const int COLS = 1000;

int arr[ROWS][COLS];

void init_arr() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            arr[i][j] = rand() % 100;
        }
    }
}

int sum_all(int num_threads) {
    int sum = 0;
    double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            sum += arr[i][j];
        }
    }
    double t2 = omp_get_wtime();
    cout << "sum_all with " << num_threads << " threads took " << t2 - t1 << " seconds" << endl;
    return sum;
}

int min_row(int num_threads) {
    int min_sum = INT_MAX;
    int min_row = -1;
    double t1 = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < ROWS; i++) {
        int row_sum = 0;
        for (int j = 0; j < COLS; j++) {
            row_sum += arr[i][j];
        }
#pragma omp critical
        {
            if (row_sum < min_sum) {
                min_sum = row_sum;
                min_row = i;
            }
        }
    }
    double t2 = omp_get_wtime();
    cout << "min_row with " << num_threads << " threads took " << t2 - t1 << " seconds" << endl;
    cout << "min_row = " << min_row << ", sum = " << min_sum << endl;
    return min_row;
}

int main() {
    int num_threads;
    cout << "Enter number of threads: ";
    cin >> num_threads;
    init_arr();
    sum_all(num_threads);
    min_row(num_threads);
    return 0;
}
