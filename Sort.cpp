//
// Created by kuba on 05.04.2025.
//

#include "Sort.h"
#include <future>      // <-- DODANE
#include <functional>  // <-- DODANE
#include <thread>     // <--- WAŻNE
#include <vector>     // <--- WAŻNE
#include <utility>    // <--- WAŻNE (dla std::pair)

// QuickSort Multi-Threaded (using std::async)
template <typename T>
void Sort::quick_sort_multithreaded(Structure<T> &structure, int left, int right, int modifier) {
    if (left >= right) return;

    unsigned int num_threads = std::thread::hardware_concurrency();

    if (num_threads <= 1) {
        quick_sort(structure, left, right, modifier);
        return;
    }

    const int MIN_JOB_PER_THREAD = 1000;

    std::vector<std::pair<int, int>> chunks;
    chunks.push_back({left, right});

    for (unsigned int i = 0; i < num_threads - 1; ++i) {

        int largest_chunk_index = -1;
        int max_size = -1;
        for (int j = 0; j < chunks.size(); ++j) {
            int current_size = chunks[j].second - chunks[j].first + 1;
            if (current_size > max_size) {
                max_size = current_size;
                largest_chunk_index = j;
            }
        }

        if (max_size < (MIN_JOB_PER_THREAD * 2)) break;

        std::pair<int, int> chunk_to_split = chunks[largest_chunk_index];
        std::swap(chunks[largest_chunk_index], chunks.back());
        chunks.pop_back();

        int l = chunk_to_split.first;
        int r = chunk_to_split.second;
        int q = partition(structure, l, r, modifier);

        if (l <= q) chunks.push_back({l, q});
        if (q + 1 <= r) chunks.push_back({q + 1, r});
    }


    std::vector<std::future<void>> futures;

    while (chunks.size() > 1) {
        std::pair<int, int> chunk = chunks.back();
        chunks.pop_back();
        futures.push_back(std::async(std::launch::async,&Sort::quick_sort<T>, this,std::ref(structure), chunk.first, chunk.second, modifier));
    }

    if (!chunks.empty()) quick_sort(structure, chunks[0].first, chunks[0].second, modifier);
    for (auto& f : futures) f.get();
}

// QuickSort
template <typename T>
void Sort::quick_sort(Structure<T> &structure, int left, int right, int modifier) {
    if(left<right){
        int q = partition(structure,left,right, modifier);
        quick_sort(structure,left,q, modifier);
        quick_sort(structure,q+1, right, modifier);
    }
}

// Partition - function used in QuickSort to divide a Structure
template <typename T>
int Sort::partition(Structure<T> &structure, int left, int right, int modifier){
    int pivot_id;
    switch (modifier) {
        case 0:
            pivot_id = (right+left)/2;
            break;
        case 1:
            pivot_id = left;
            break;
        case 2:
            pivot_id = right;
            break;
        case 3:
            pivot_id = left + rand() % (right - left + 1);
            break;
        default:
            pivot_id = (right+left)/2;
            break;
    }
    T pivot = structure.get_element(pivot_id);
    int i = left-1;
    int j = right+1;
    while (true){
        do{
            i++;
        } while (compare(pivot,structure.get_element(i)));
        do{
            j--;
        } while(compare(structure.get_element(j),pivot));
        if(i<j){
            T temp = structure.get_element(i);
            structure.get_array()[i] = structure.get_element(j);
            structure.get_array()[j] = temp;
        }
        else {
            if (j==right) j--;
            return j;
        }
    }
}

template<typename T>
bool Sort::compare(T first, T second) {
    return true;
}

template<>
bool Sort::compare<int>(int first, int second) {
    if(first>second) return true;
    else return false;
}

template<>
bool Sort::compare<float>(float first, float second) {
    if(first>second) return true;
    else return false;
}

template<>
bool Sort::compare<char*>(char* first, char* second) {
    int id = 0;
    while(first[id]!='\0' && second[id]!='\0'){
        if(first[id]>second[id]) return true;
        else if(first[id]<second[id]) return false;
        id++;
    }
    if(first[id]=='\0') return false;
    else return true;
}


// Verifies if Structure is sorted correctly
template <typename T>
bool Sort::verify(Structure<T> &structure) {
    for(int i=0; i<structure.get_size(); i++){
        if(!compare(structure.get_element(i), structure.get_element(i+1))) return true;
    }
    return false;
}


template void Sort::quick_sort<int>(Structure<int>&,int left,int right, int modifier);
template void Sort::quick_sort<float>(Structure<float>&,int left,int right, int modifier);
template void Sort::quick_sort<char*>(Structure<char*>&,int left,int right, int modifier);

template void Sort::quick_sort_multithreaded<int>(Structure<int>&,int left,int right, int modifier);
template void Sort::quick_sort_multithreaded<float>(Structure<float>&,int left,int right, int modifier);
template void Sort::quick_sort_multithreaded<char*>(Structure<char*>&,int left,int right, int modifier);

template int Sort::partition<int>(Structure<int> &structure, int left, int right, int modifier);
template int Sort::partition<float>(Structure<float> &structure, int left, int right, int modifier);
template int Sort::partition<char*>(Structure<char*> &structure, int left, int right, int modifier);

template bool Sort::verify<int>(Structure<int> &structure);
template bool Sort::verify<float>(Structure<float> &structure);
template bool Sort::verify<char*>(Structure<char*> &structure);