#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include "priority_queue.h"
#include <list>
// OVERVIEW: A specialized version of the 'heap' ADT implemented as a
//           Fibonacci heap.
template<typename TYPE, typename COMP = std::less<TYPE> >
class fib_heap: public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;

    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    fib_heap(COMP comp = COMP());

    // EFFECTS: Deconstruct the heap with no memory leak.
    // MODIFIES: this
    // RUNTIME: O(n)
    ~fib_heap();

    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    virtual void enqueue(const TYPE &val);

    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: Amortized O(log(n))
    virtual TYPE dequeue_min();

    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(1)
    virtual const TYPE &get_min() const;

    // EFFECTS: Get the number of elements in the heap.
    // RUNTIME: O(1)
    virtual size_type size() const;

    // EFFECTS: Return true if the heap is empty.
    // RUNTIME: O(1)
    virtual bool empty() const;

private:
    // Note: compare is a functor object
    COMP compare;

private:
    // Add any additional member functions or data you require here.
    // You may want to define a strcut/class to represent nodes in the heap and a
    // pointer to the min node in the heap.
    struct fib_node
    {
        TYPE key;
        std::list<fib_node> child;
        int degree=0;
    };
    typename std::list<fib_node> root;
    typename std::list<fib_node>::iterator min;
    int number=0;
    virtual void consolidate();
    TYPE empty_element=TYPE();

};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

#endif //FIB_HEAP_H
template<typename TYPE,typename COMP>
fib_heap<TYPE,COMP>::fib_heap(COMP comp){
    number=0;
    compare=comp;
    min=root.end();
}
template<typename TYPE,typename COMP>
void fib_heap<TYPE,COMP>::enqueue(const TYPE&val)
{
    fib_node new_fib_node;
    new_fib_node.key=val;
    if(root.size()==1)
    {
        root.push_back(new_fib_node);
        min=root.begin();
    }
    else
    {
        if(compare(val,(*min).key))
        {
            root.push_back(new_fib_node);
            min=root.end();
            min--;
        }
        else
        {
            root.push_back(new_fib_node);
        }
    }
    number++;
}

template<typename TYPE, typename COMP>
bool fib_heap<TYPE, COMP>::empty() const {
    return this->size() == 0;
}

template<typename TYPE, typename COMP>
unsigned fib_heap<TYPE, COMP>::size() const {
    return this->number;
}

template<typename TYPE,typename COMP>
const TYPE&fib_heap<TYPE,COMP>::get_min() const
{
    if(this->empty())
    {
        return empty_element;
    }
    else
    {
        return (*min).key;
    }
}
template<typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP>::dequeue_min() {
     auto temp= *min;
     if(min != root.end()){
         typename std::list<fib_node>::iterator min_child;
         for(min_child=temp.child.begin();min_child!=temp.child.end();){
             root.push_back(*min_child);
             min_child=temp.child.erase(min_child);
         }
     }
    min=root.erase(min);
     number--;
     if(number==0){
         min=root.end();
     }else{
         consolidate();
     }
    return temp.key;
}
template<typename TYPE, typename COMP>
void fib_heap<TYPE,COMP>:: consolidate(){
    int array_size;
    double d=log(number)/log((1+sqrt(5))/2);
    array_size=int(d+1);
    typename std::list<fib_node>::iterator A[array_size];
    for(int i=0;i<array_size;++i)
    {
        A[i]=root.end();
    }
    int deg=0;
    typename std::list<fib_node>::iterator temp_y = root.begin();
    typename std::list<fib_node>::iterator iterator_fib;
    for(iterator_fib=root.begin();iterator_fib!=root.end();iterator_fib++){
        deg=(*iterator_fib).degree;
        while(A[deg]!=root.end()){
            temp_y=A[deg];
            if(compare((*temp_y).key,(*iterator_fib).key)){
                root.insert(temp_y,*iterator_fib);
                root.insert(iterator_fib,*temp_y);
                temp_y=root.erase(temp_y);
                iterator_fib=root.erase(iterator_fib);
                /*for(typename std::list<fib_node>::iterator temp_x = root.begin(); temp_x!=root.end(); temp_x++){
                    std::cout << (*temp_x).key << " ";
                }*/
                //std::cout<<std::endl;
               // std::cout<<"temp_y"<<(*temp_y).key<<"iterator_fib"<<(*iterator_fib).key<<std::endl;
                temp_y--;
                iterator_fib--;
                //std::cout<<"temp_y"<<(*temp_y).key<<"iterator_fib"<<(*iterator_fib).key<<std::endl;
                (*iterator_fib).child.push_back(*(temp_y));
                (*iterator_fib).degree++;
                 root.erase(temp_y);
            }else{
                (*iterator_fib).degree++;
                (*iterator_fib).child.push_back((*temp_y));
                root.erase(temp_y);
            }
            A[deg]=root.end();
            deg++;
        }
        A[deg]=iterator_fib;
    }
    this->min=root.end();
    for(int i=0;i<array_size;i++){
        if(A[i]!=root.end()){
            if(min==root.end()){
                min=A[i];
            }else
            {
                if(compare((*A[i]).key,(*min).key)){
                    min=A[i];
                }
            }
        }
    }
}
template<typename TYPE, typename COMP>
fib_heap<TYPE,COMP>::~fib_heap(){
}
