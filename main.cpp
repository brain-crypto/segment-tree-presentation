#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono> 
#define LIM 10000000
#define INT_MAX 2069696969
using namespace std;
typedef vector<int> vi; 
struct SegmentTree
{
    vi nodes; // an array of integers
    SegmentTree(int size) : nodes(exp2(ceil(log2(size)) + 1), INT_MAX) {} 
    // construct an array of size 2*(n+1), where 2*n is the smallest power of 2 above the size of the dataset
    // as this tree is for finding the smallest leaf, we set the value of empty leaves to INT_MAX so they don't interfere with the minimum

    int query(int left, int right) // just a shortcut, as we always visit the top node in the first call
    {
        return query(1, 0, nodes.size() / 2, left, right); 
    }

    //[c_left, c_right) is the window that the current node covers, [left, right) is the target range
    int query(int node, int c_left, int c_right, int left, int right)
    {
        if (c_left >= right || c_right <= left) //window lies outside of the range
            return INT_MAX;
        else if (left <= c_left && c_right <= right) //window lies within the range
            return nodes[node];
        else //window lies partly within the range
        {
            int middle = (c_left + c_right) / 2;
            // the minimum in this window = the smaller of the two minimums in the half-windows
            return min(query(node * 2, c_left, middle, left, right), query(node * 2 + 1, middle, c_right, left, right));
        }
    }
    void update(int index, int value)
    {
        update(1, 0, nodes.size()/2, index, value);
    }
    void update(int node, int c_left, int c_right, int index, int value)
    {
        if (c_left > index || c_right <= index) //exit if this half doesn't contain the node we want to update
            return;
        else if (c_left == index && index + 1 == c_right) //the current node is the node we want to update
        {
            nodes[node] = value;
            return;
        }
        else 
        {
            int middle = (c_left + c_right) / 2;
            // call update on both halves, as the half that doesn't contain the node will just exit as soon as it is called
            update(node * 2, c_left, middle, index, value);
            update(node * 2 + 1, middle, c_right, index, value);
            nodes[node] = min(nodes[node * 2], nodes[node * 2 + 1]); // update the value of the current node, in case the minimum has changed
        }
    }
};

int main()
{
    ifstream num("data0103.txt");
    SegmentTree tree(LIM); 
    auto start = chrono::high_resolution_clock::now(); 
    string input; 
    for (int i = 0; i < LIM; i++)
    {
        getline(num, input);
        tree.update(i, stoi(input));
    }
    auto stop = chrono::high_resolution_clock::now(); 
    cout << chrono::duration_cast<chrono::microseconds>(stop-start).count() << "\n"; 
    // for (int i = 0; i < LIM; i++)
    // {
    //     cout << Numbers[i] << " ";
    // }    
    num.close();
    while (true)
    {        
        int left, right;
        cout << "Enter a range: ";
        cin >> left >> right;
        start = chrono::high_resolution_clock::now(); 
        cout << tree.query(left, right) << ", ";
        stop = chrono::high_resolution_clock::now(); 
        cout << chrono::duration_cast<chrono::microseconds>(stop-start).count() << "\n"; 
    }
    return 0;
}

