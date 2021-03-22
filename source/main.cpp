/*

  Name: Ryan Kilkenney
  Date: 11/12/2020
  Purpose: It's the main file for project 4. It takes in n sublists of m numbers, sorts the sublists, and merges it into one big list 

*/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

//I use this struct for making the heap in the merge function
struct element {
  int value; //This is a number value from a sublist 
  int listID; //this is the list number that the value is from 
  int place; //this is the values place in the list

  //this is a functor that is used to compare elements so it can be made into a min heap, see README
  bool operator>(const element & f) const {
    return value > f.value;
  }
} ;

//This function is used to find the median of 3 parition from a sublist and does most of sorting work for the quicksort algoritm 
//I took the pseudocode from the chapter 10 notes and turned it into actual code 
int partition(vector<int>& list, int first, int last) {
  // The pivot should be the median of the
  // first, middle, and last elements.
  
  int pivot = 0; //it's the element that the vector is be sorted around 
  int index = 0; //it's the index of the unsorted part of the vector 
  int smallIndex = 0; //it's the index of the lower vector 
  int mid = (first + last)/2; //its the index of the middle element of the vector
  int med = 0; //its the median of the vecor 

  //this checks the values at first, middle, and last and finds the median of the 3
  if(((list[first] > list[last]) && (list[first] < list[mid])) || ((list[first] < list[last]) && (list[first] > list[mid]))) {
    med = first;
  } else if(((list[mid] > list[last]) && (list[mid] < list[first])) || ((list[mid] < list[last]) && (list[mid] > list[first]))) {
    med = mid;
  } else {
    med = last;
  }
  
  //the median element is moved to the front
  swap(list[first], list[med]);

  //the pivot is set to the element at the front 
  pivot = list[first];
  
  //small index is set equal to the index of the first element
  smallIndex = first;

  //this is where the actual sorting happens 
  for(index = first + 1; index <= last; index++) { //case 1 part, nothing needs to happen 
    
    //case 2 part, element gets moved into lower vector and smallIndex gets incremented
    if(list[index] <= pivot) { 
      smallIndex++;
      swap(list[smallIndex], list[index]); 
    }
  }

  //piviot gets settled into it's spot in the vector 
  swap(list[first], list[smallIndex]);
  
  return smallIndex; 
}

//This also just the pseudocode from the notes but adapted to fit the program 
//I noticed that the if(left == right) from the notes wasn't actually doing anything and gave me an infinte loop so 
//I simplified and corrected it to what you see here
void quicksort(vector<int>& list, int first, int last) {
  if(first < last) {
    int i = partition(list, first, last); //partitions the vector into 2 parts
    quicksort(list, first, i-1); //sorts left subvector
    quicksort(list, i+1, last); //sorts right subvectors
  }
}

void multiway_merge(vector<vector<int> >& input_lists, vector<int>& output_list) {
  int total_elements = output_list.size(); //it's the total number of elements to be outputed 
  int total_lists = input_lists.size(); //it's the total number of lists
  element e, f; //this are used to make up the min heap
  priority_queue <element, vector<element>, greater <element> > pq; //it's a min heap of element structs
  int min = 0; //it's used to hold the min value of the heap
  int tempID = 0; //it's used to hold the list number that that the min value came from 
  int pos = 0; //since output_list starts as a list of 0's, this just holds the index of the postion that needs to be updated 

  //this creates the inital heap of the min value of each sub vector 
  for(int i = 0; i < total_lists; i++) { 
    e.value = input_lists[i][0];
    e.listID = i;
    e.place = 0;
    pq.push(e);
  }

  //this loop takes the min value out of the heap, puts it into output_lists, and replaces the removed value from the heap with its successor 
  while(total_elements > 0) {
    min = pq.top().value;
    output_list[pos] = min;
    tempID = pq.top().listID;
    f = pq.top();
    pq.pop();

    //this part is where the replacing the min value with it's successor happenes 
    //it checks if there are more numbers from the element's list
    //grabs the successor value and its place in the list
    //pushes the updated element back into the list
    if(f.place < input_lists[f.listID].size()-1) { 
      f.value = input_lists[f.listID][f.place+1];
      f.place++;
      pq.push(f);
    }

    total_elements--;
    pos++;
  } 
} //the loop has to run for each element so it runs while total_elements > 0

int main(int argc, char** argv) {
  int n, m;
  cin >> n >> m;

  vector<vector<int> > input_lists(n, vector<int>(m));

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cin >> input_lists[i][j];
    }
  }

  // Quicksort k sublists
  for (int i = 0; i < input_lists.size(); ++i)
    quicksort(input_lists[i], 0, m-1);

  //loop to check result of quicksort
  /*for (int i = 0; i < input_lists.size(); i++) {
    for(int j = 0; j < input_lists[i].size(); j++) {
      cout << input_lists[i][j] << " ";
    }
    cout << endl;
  }*/

  // Merge n input sublists into one sorted list
  vector<int> output_list(n * m);
  multiway_merge(input_lists, output_list);

  for (int i = 0; i < output_list.size(); ++i)
    cout << output_list[i] << " ";
  cout << endl;
}
