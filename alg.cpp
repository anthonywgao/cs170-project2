#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <math.h>
#include <limits>

using namespace std;
using namespace std::chrono;

float accuracy(vector<vector<float>> data, vector<int> cur_set)
{
}

void forward_sel(vector<vector<float>> data)
{
}
void backward_elim(vector<vector<float>> data)
{
}

int main(int argc, char *argv[])
{
    // get file
    ifstream file;
    if (argc > 1)
    {
        file.open(argv[1]);
        if (!file.is_open())
        {
            exit(1);
        }
    }
    else
    {
        exit(1);
    }
    // parse data
    vector<vector<float>> data;
    string row;
    float col;
    while (getline(file, row))
    {
        istringstream sstream(row);
        vector<float> rowV;
        while (sstream >> col)
        {
            rowV.push_back(col);
        }
        data.push_back(rowV);
    }
    // menu
    cout << "Anthonys feature selection" << endl
         << "Pick 1 for forward, 2 for backward" << endl;
    int choice;
    cin >> choice;
    cout << "This data has " << data.at(1).size() - 1 << " features with " << data.size() << " instances" << endl;
    auto start = high_resolution_clock::now();
    switch (choice)
    {
    case 2:
        forward_sel(data);
        break;
    default:
        backward_elim(data);
        break;
    }

    auto elapsed = duration_cast<seconds>(high_resolution_clock::now() - start);
    cout << "Total time: " << elapsed.count() << " seconds" << endl;
    return 0;
}