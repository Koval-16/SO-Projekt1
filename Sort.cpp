//
// Created by kuba on 05.04.2025.
//

#include "Sort.h"
#include "BoardGame.h"
#include <future>      // <-- DODANE
#include <functional>  // <-- DODANE
#include <thread>     // <--- WAŻNE
#include <vector>     // <--- WAŻNE
#include <utility>    // <--- WAŻNE (dla std::pair)

// Insertion Sort Algorithm
template <typename T>
void Sort::insertion_sort(Structure<T> &structure) {
    int size = structure.get_size();
    for(int j=size-2; j>=0; j--){
        T element = structure.get_element(j);
        int i = j+1;
        while(i<size && compare(element, structure.get_element(i))){
            structure.get_array()[i-1] = structure.get_element(i);
            i++;
        }
        structure.get_array()[i-1] = element;
    }
}

// Heap Sort Algorithm
template <typename T>
void Sort::heap_sort(Structure<T> &structure) {
    int heapsize = build_heap(structure,(structure.get_size()/2)-1);
    for(int i=structure.get_size()-1; i>0; i--){
        T temp = structure.get_element(0);
        structure.get_array()[0] = structure.get_element(i);
        structure.get_array()[i] = temp;
        heapsize--;
        heapify(structure,0, heapsize);
    }
}

// Heapify, function that repairs the Heap
template<typename T>
void Sort::heapify(Structure<T>& structure, int index, int heapsize) {
    int l = 2*(index+1)-1;
    int r = 2*(index+1);
    int largest;
    if(l<heapsize && compare(structure.get_element(l), structure.get_element(index))) largest=l;
    else largest=index;
    if(r<heapsize && compare(structure.get_element(r),structure.get_element(largest))) largest=r;
    if(largest!=index){
        T temp = structure.get_element(index);
        structure.get_array()[index] = structure.get_element(largest);
        structure.get_array()[largest] = temp;
        heapify(structure,largest,heapsize);
    }
}

// Build Heap - function to build a Heap
template<typename T>
int Sort::build_heap(Structure<T>& structure, int start) {
    int heapsize = structure.get_size();
    for(int i=start; i>=0; i--){
        heapify(structure,i,heapsize);
    }
    return heapsize;
}

// ShellSort
template <typename T>
void Sort::shell_sort(Structure<T> &structure, int modifier) {
    int difference = space(structure.get_size(),modifier);
    while(difference>0){
        for(int j=structure.get_size()-difference-1; j>=0; j--){
            T element = structure.get_element(j);
            int i = j+difference;
            while(i<structure.get_size() && compare(element,structure.get_element(i))){
                structure.get_array()[i-difference] = structure.get_element(i);
                i += difference;
            }
            structure.get_array()[i-difference] = element;
        }
        if(modifier==0)difference = difference/3;
        else difference = difference/2;
    }
}

// Space - function which calculates a space in Shell's Algorithm
int Sort::space(int size, int modifier){
    int diff = 1;
    switch (modifier) {
        case 0:
            while(diff<size) diff = 3*diff+1;
            diff = diff/9;
            break;
        case 1:
            diff = size/2;
            break;
        default:
            break;
    }

    return diff;
}

// Drunk Version of HeapSort
// It calls Heapify for every element, instead of one
template<typename T>
void Sort::drunk_sort(Structure<T> &structure) {
    int heapsize = build_heap(structure, structure.get_size());
    for(int i=structure.get_size()-1; i>0; i--){
        T temp = structure.get_element(0);
        structure.get_array()[0] = structure.get_element(i);
        structure.get_array()[i] = temp;
        heapsize--;
        for(int j=0; j<heapsize; j++) heapify(structure,j, heapsize);
    }
}

// ##################################################################
// ##               NOWA SEKCJA - WIELOWĄTKOWY QUICKSORT           ##
// ##################################################################

// QuickSort Multi-Threaded (using std::async)
template <typename T>
void Sort::quick_sort_multithreaded(Structure<T> &structure, int left, int right, int modifier) {
    if (left >= right) {
        return;
    }

    unsigned int num_threads = std::thread::hardware_concurrency();

    // Jeśli mamy 1 rdzeń, nie ma sensu tworzyć wątków
    if (num_threads <= 1) {
        quick_sort(structure, left, right, modifier);
        return;
    }

    // Minimalna wielkość zadania, aby opłacało się je dzielić
    const int MIN_JOB_PER_THREAD = 1000;

    // --- 2. Stwórz 'num_threads' kawałków pracy ---
    // (Jeśli num_threads = 2, ta pętla zrobi 1 podział - jak Twój kod)
    // (Jeśli num_threads = 12, ta pętla zrobi 11 podziałów)

    std::vector<std::pair<int, int>> chunks;
    chunks.push_back({left, right}); // Zaczynamy z 1 kawałkiem (cała tablica)

    // Chcemy mieć 'num_threads' kawałków, więc wykonujemy 'num_threads - 1' podziałów
    for (unsigned int i = 0; i < num_threads - 1; ++i) {

        // Znajdź największy kawałek na liście, aby go podzielić (lepsze równoważenie)
        int largest_chunk_index = -1;
        int max_size = -1;
        for (int j = 0; j < chunks.size(); ++j) {
            int current_size = chunks[j].second - chunks[j].first + 1;
            if (current_size > max_size) {
                max_size = current_size;
                largest_chunk_index = j;
            }
        }

        // Jeśli największy kawałek jest za mały, nie dzielmy go już.
        if (max_size < (MIN_JOB_PER_THREAD * 2)) break;

        // Mamy największy kawałek, wyjmij go z listy
        std::pair<int, int> chunk_to_split = chunks[largest_chunk_index];
        std::swap(chunks[largest_chunk_index], chunks.back());
        chunks.pop_back();

        // Podziel go
        int l = chunk_to_split.first;
        int r = chunk_to_split.second;
        int q = partition(structure, l, r, modifier);

        // Wrzuć dwa nowe, mniejsze kawałki na listę
        if (l <= q) {
            chunks.push_back({l, q});
        }
        if (q + 1 <= r) {
            chunks.push_back({q + 1, r});
        }
    }

    // --- 3. Uruchom wątki ---
    // Mamy teraz `chunks.size()` kawałków pracy

    std::vector<std::future<void>> futures;

    // Uruchom N-1 wątków, zostawiając ostatni kawałek dla wątku głównego
    while (chunks.size() > 1) {
        std::pair<int, int> chunk = chunks.back();
        chunks.pop_back();

        // Uruchamiamy std::async na JEDNOWĄTKOWYM quick_sorcie
        // Dokładnie tak, jak w Twojej 2-wątkowej funkcji
        futures.push_back(
                std::async(std::launch::async,
                           &Sort::quick_sort<T>, this,
                           std::ref(structure), chunk.first, chunk.second, modifier)
        );
    }

    // --- 4. Sortuj ostatni kawałek w wątku głównym ---
    if (!chunks.empty()) {
        quick_sort(structure, chunks[0].first, chunks[0].second, modifier);
    }

    // --- 5. Czekaj na resztę (tak jak future_left.get()) ---
    for (auto& f : futures) {
        f.get();
    }
}

// ##################################################################
// ##                     KONIEC NOWEJ SEKCJI                      ##
// ##################################################################


// ##################################################################
// ##     NOWA SEKCJA - POPRAWIONY WIELOWĄTKOWY QUICKSORT (HYBRYDOWY)
// ##################################################################

// QuickSort Multi-Threaded (Hybrid Version)
template <typename T>
void Sort::multi_quick_sort(Structure<T> &structure, int left, int right, int modifier) {
    if (left >= right) {
        return; // Przypadek bazowy
    }

    // Ten próg jest nadal przydatny.
    // Jeśli cała tablica jest mała, nie ma sensu w ogóle tworzyć nowego wątku.
    const int THRESHOLD = 10000; // Zwiększyłem próg, dla 1000 nie warto odpalać wątku

    if ((right - left) < THRESHOLD) {
        // Tablica jest za mała na wielowątkowość, użyj oryginalnego jednowątkowego quick_sort
        quick_sort(structure, left, right, modifier);
    } else {
        // Tablica jest duża - dokonaj JEDNEGO podziału
        int q = partition(structure, left, right, modifier);

        // Uruchom sortowanie lewej partycji asynchronicznie (w nowym wątku),
        // ALE ten wątek będzie sortował już do końca JEDNOWĄTKOWO
        // (dlatego wywołujemy &Sort::quick_sort, a NIE &Sort::quick_sort_multithreaded)
        auto future_left = std::async(std::launch::async,
                                      &Sort::quick_sort<T>, this, // <-- KLUCZOWA POPRAWKA
                                      std::ref(structure), left, q, modifier);

        // Sortuj prawą partycję w *bieżącym* wątku,
        // RÓWNIEŻ jednowątkowo do końca.
        quick_sort(structure, q + 1, right, modifier); // <-- KLUCZOWA POPRAWKA

        // Zaczekaj na zakończenie pracy drugiego wątku
        future_left.get();
    }
}
// #############################################

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

template<>
bool Sort::compare<BoardGame>(BoardGame first, BoardGame second){
    if(first.get_ratio()>second.get_ratio()) return true;
    else return false;
}

// Verifies if Structure is sorted correctly
template <typename T>
bool Sort::verify(Structure<T> &structure) {
    for(int i=0; i<structure.get_size(); i++){
        if(!compare(structure.get_element(i), structure.get_element(i+1))) return true;
    }
    return false;
}

template void Sort::insertion_sort<int>(Structure<int>&);
template void Sort::heap_sort<int>(Structure<int>&);
template void Sort::shell_sort<int>(Structure<int>&, int modifier);
template void Sort::quick_sort<int>(Structure<int>&,int left,int right, int modifier);
template void Sort::drunk_sort<int>(Structure<int>&);

template void Sort::insertion_sort<float>(Structure<float>&);
template void Sort::heap_sort<float>(Structure<float>&);
template void Sort::shell_sort<float>(Structure<float>&, int modifier);
template void Sort::quick_sort<float>(Structure<float>&,int left,int right, int modifier);
template void Sort::drunk_sort<float>(Structure<float>&);

template void Sort::insertion_sort<char*>(Structure<char*>&);
template void Sort::heap_sort<char*>(Structure<char*>&);
template void Sort::shell_sort<char*>(Structure<char*>&, int modifier);
template void Sort::quick_sort<char*>(Structure<char*>&,int left,int right, int modifier);
template void Sort::drunk_sort<char*>(Structure<char*>&);

template void Sort::insertion_sort<BoardGame>(Structure<BoardGame>&);
template void Sort::heap_sort<BoardGame>(Structure<BoardGame>&);
template void Sort::shell_sort<BoardGame>(Structure<BoardGame>&, int modifier);
template void Sort::quick_sort<BoardGame>(Structure<BoardGame>&,int left,int right, int modifier);
template void Sort::drunk_sort<BoardGame>(Structure<BoardGame>&);

// Instancje dla nowej funkcji wielowątkowej
template void Sort::quick_sort_multithreaded<int>(Structure<int>&,int left,int right, int modifier);
template void Sort::quick_sort_multithreaded<float>(Structure<float>&,int left,int right, int modifier);
template void Sort::quick_sort_multithreaded<char*>(Structure<char*>&,int left,int right, int modifier);
template void Sort::quick_sort_multithreaded<BoardGame>(Structure<BoardGame>&,int left,int right, int modifier);

// Instancje dla nowej funkcji wielowątkowej
template void Sort::multi_quick_sort<int>(Structure<int>&,int left,int right, int modifier);
template void Sort::multi_quick_sort<float>(Structure<float>&,int left,int right, int modifier);
template void Sort::multi_quick_sort<char*>(Structure<char*>&,int left,int right, int modifier);
template void Sort::multi_quick_sort<BoardGame>(Structure<BoardGame>&,int left,int right, int modifier);


template int Sort::partition<int>(Structure<int> &structure, int left, int right, int modifier);
template int Sort::partition<float>(Structure<float> &structure, int left, int right, int modifier);
template int Sort::partition<char*>(Structure<char*> &structure, int left, int right, int modifier);
template int Sort::partition<BoardGame>(Structure<BoardGame> &structure, int left, int right, int modifier);

template void Sort::heapify<int>(Structure<int> &structure, int index, int heapsize);
template void Sort::heapify<float>(Structure<float> &structure, int index, int heapsize);
template void Sort::heapify<char*>(Structure<char*> &structure, int index, int heapsize);
template void Sort::heapify<BoardGame>(Structure<BoardGame> &structure, int index, int heapsize);

template int Sort::build_heap<int>(Structure<int> &structure, int start);
template int Sort::build_heap<float>(Structure<float> &structure, int start);
template int Sort::build_heap<char*>(Structure<char*> &structure, int start);
template int Sort::build_heap<BoardGame>(Structure<BoardGame> &structure, int start);

template bool Sort::verify<int>(Structure<int> &structure);
template bool Sort::verify<float>(Structure<float> &structure);
template bool Sort::verify<char*>(Structure<char*> &structure);
template bool Sort::verify<BoardGame>(Structure<BoardGame> &structure);