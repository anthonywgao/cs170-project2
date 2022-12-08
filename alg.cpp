#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <math.h>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace std::chrono;

float calc_distance(vector<float> a, vector<float> b)
{
    float sum = 0;
    for (int i = 1; i < a.size(); ++i)
    {
        sum += pow((a[i] - b[i]), 2);
    }
    return sqrt(sum);
}

float accuracy(vector<vector<float>> data, vector<int> cur_set)
{
    int num_cor = 0;
    // 0 out other features
    for (int i = 0; i < data.size(); ++i)
    {
        for (int j = 1; j < data[0].size(); ++j)
        {
            if (find(cur_set.begin(), cur_set.end(), j) == cur_set.end())
                data[i][j] = 0;
        }
    }
    for (int i = 0; i < data.size(); i++)
    {
        vector<float> object_to_classify = data.at(i);
        int label = data[i][0];
        float nn_dist = numeric_limits<float>::max();
        float nn_loc = numeric_limits<float>::max();
        int nn_label;

        for (int j = 0; j < data.size(); j++)
        {
            if (i != j)
            {
                float distance = calc_distance(object_to_classify, data.at(j));
                if (distance < nn_dist)
                {
                    nn_dist = distance;
                    nn_loc = j;
                    nn_label = data[nn_loc][0];
                }
            }
        }
        if (label == nn_label)
            num_cor++;
    }
    return (float)num_cor / data.size();
}

void forward_sel(vector<vector<float>> data)
{
    vector<int> cur;
    vector<int> sol;

    float global_acc = accuracy(data, cur);
    cout << "Default accuracy: " << global_acc << endl;
    for (int i = 1; i < data.at(0).size(); ++i)
    {
        int f_add;
        float local_acc = 0;
        for (int j = 1; j < data.at(0).size(); ++j)
        {
            if (find(cur.begin(), cur.end(), j) == cur.end())
            {
                vector<int> temp = cur;
                temp.push_back(j);
                float acc = accuracy(data, temp);
                cout << "Using feature(s) {";
                for (int f : temp)
                {
                    cout << f << ',';
                }
                cout << "} accuracy is " << acc << endl;

                if (acc > local_acc)
                {
                    local_acc = acc;
                    f_add = j;
                }
            }
        }
        cur.push_back(f_add);
        cout << "Added feature " << f_add;
        cout << ". Using feature(s) {";
        for (int f : cur)
        {
            cout << f << ',';
        }
        cout << "} accuracy is " << local_acc << endl;
        if (local_acc > global_acc)
        {
            global_acc = local_acc;
            sol = cur;
        }
    }
    cout << endl
         << "Finished" << endl;
    cout << "Best accuracy: " << global_acc << " using feature(s) {";
    for (int f : sol)
    {
        cout << f << ',';
    }
    cout << '}' << endl;
}
void backward_elim(vector<vector<float>> data)
{
    vector<int> cur(data[0].size() - 1);
    iota(cur.begin(), cur.end(), 1);
    vector<int> sol = cur;
    float global_acc = accuracy(data, cur);
    cout << "Default accuracy: " << global_acc << endl;
    for (int i = 1; i < data.at(0).size(); ++i)
    {
        int f_rem;
        float local_acc = 0;
        for (int j = 1; j < data.at(0).size(); ++j)
        {
            if (find(cur.begin(), cur.end(), j) != cur.end())
            {
                vector<int> temp = cur;
                vector<int>::iterator it = find(temp.begin(), temp.end(), j);
                temp.erase(temp.begin() + (it - temp.begin()));
                float acc = accuracy(data, temp);
                cout << "Using feature(s) {";
                for (int f : temp)
                {
                    cout << f << ',';
                }
                cout << "} accuracy is " << acc << endl;

                if (acc > local_acc)
                {
                    local_acc = acc;
                    f_rem = j;
                }
            }
        }
        vector<int>::iterator it = find(cur.begin(), cur.end(), f_rem);
        cur.erase(it);
        cout << "Removed feature " << f_rem;
        cout << ". Using feature(s) {";
        for (int f : cur)
        {
            cout << f << ',';
        }
        cout << "} accuracy is " << local_acc << endl;
        if (local_acc > global_acc)
        {
            global_acc = local_acc;
            sol = cur;
        }
    }
    cout << endl
         << "Finished" << endl;
    cout << "Best accuracy: " << global_acc << " using feature(s) {";
    for (int f : sol)
    {
        cout << f << ',';
    }
    cout << '}' << endl;
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
    case 1:
        forward_sel(data);
        break;
    case 2:
        backward_elim(data);
        break;
    }

    auto elapsed = duration_cast<seconds>(high_resolution_clock::now() - start);
    cout << "Total time: " << elapsed.count() << " seconds" << endl;
    return 0;
}