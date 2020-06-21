#ifndef BUW_LIST_HPP
#define BUW_LIST_HPP
#include <cassert>
#include <cstddef>  //ptrdiff_t
#include <iterator> //std::bidirectional_iterator_tag
#include <iostream>

#include <initializer_list>

template <typename T>
class List;

template <typename T>
struct ListNode {
  T         value = T{};
  ListNode* prev = nullptr;
  ListNode* next = nullptr;
};


//TODO: Implementierung der Methoden des Iterators 
//      (nach Vorlesung STL-1 am 09. Juni) (Aufgabe 3.12)
template <typename T>
struct ListIterator {
  using Self              = ListIterator<T>;
  using value_type        = T;
  using pointer           = T*;
  using reference         = T&;
  using difference_type   = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;


  /* DESCRIPTION  operator*() */
  T&  operator*()  const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return node->value;
  }

  /* DESCRIPTION  operator->() */
  T* operator->() const {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    return &node->value;
  }

  /* PREINCREMENT, call: ++it, advances one element forward */
  ListIterator<T>& operator++() {
    if(nullptr == node) {
      throw "Iterator does not point to valid node";
    }
    node = node->next;
    return *this;
  }

  /* POSTINCREMENT (signature distinguishes the iterators), 
                    call:  it++, advances one element forward*/
  ListIterator<T> operator++(int) {
    if(nullptr == node) {
        throw "Iterator does not point to valid node";
    }
    else {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }  
  }

  /* the equal operator checks if an iterater equal to another is,  */
  bool operator==(ListIterator<T> const& x) const {
      return node == x.node;
  }

  /* the opposite of the == operator, it checks if there unequal */
  bool operator!=(ListIterator<T> const& x) const {
      return !(node == x.node);
  }

  ListIterator<T> next() const {
    if (nullptr != node) {
      return ListIterator{node->next};
    } else {
      return ListIterator{nullptr};
    }
  }


  ListNode <T>* node = nullptr;
};



template <typename T>
class List {
  public:

    //friend declarations for testing the members   
    template <typename TEST_TYPE>
    friend size_t get_size(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_first_pointer(List<TEST_TYPE> const& list_to_test);
    template <typename TEST_TYPE>
    friend ListNode<TEST_TYPE>* get_last_pointer(List<TEST_TYPE> const& list_to_test);

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = T const*;
    using reference       = T&;
    using const_reference = T const&;
    using iterator        = ListIterator<T>;

    List() :
        size_{0},
        first_{ nullptr },
        last_{ nullptr } {};

    // test and implement:
    List(List<T> const& list):
        size_{ 0 },
        first_{ nullptr },
        last_{ nullptr } 
        {
        ListNode<T> *tmp = list.last_;
            while (nullptr != tmp) {
                push_front(tmp->value);
                tmp = tmp->prev;
            }
        };

    /* With the move-konstruktor we transfer all elements from one list to another, the first list 
    is now empty*/
    List(List<T>&& move_list) :
        size_(move_list.size_),
        first_(move_list.first_),
        last_(move_list.last_) {
        move_list.size_ = 0;
        move_list.first_ = nullptr;
        move_list.last_ = nullptr;
    }
    //TODO: Initializer-List Konstruktor (3.10 - Teil 1)
    /* ... */
    // test and implement:
    List(std::initializer_list<T> ini_list) {
      //not implemented yet
    }

    /* the swap methode  */
    void swap(List<T> list) {
        std::swap(first_, list.first_);
        std::swap(last_, list.last_);
    }

    List& operator= (List<T> list) {
        swap(list);
        return *this;
    }

    /* this operator checks if one list is identical with another,
    we need to check every single node for this*/
    bool operator==(List const& rhs) const{
        if (empty() && rhs.empty()) {
            return true;
        }
        else if (size() != rhs.size()) {
            return false;
        }
        else {
            auto* tmp_Node1 = first_;
            auto* tmp_Node2 = rhs.first_;
            while(nullptr != tmp_Node1){
                if (tmp_Node1->value != tmp_Node2->value) {
                    return false;
                }
                tmp_Node1 = tmp_Node1->next;
                tmp_Node2 = tmp_Node2->next;
            }
            return true;
        }
    }

    /*checks if one list is unequal than another*/
    bool operator!=(List const& rhs) const
    {
        if (*this == rhs){
            return false;
        }
        else {
            return true;
        }
    }

    /* Destrucktor terminate a list by using the clear method */
    ~List() {
        clear();
    }

    /* the Listiterar gives back the first Iterator node of the list */
    ListIterator<T> begin() {
        ListIterator<T> begin;
        begin.node = first_;
        return begin;
    }

    /* the Listiterar gives back the last Iterator node of the list */
    ListIterator<T> end() {
        ListIterator<T> end;
        end.node = nullptr;
        return end;
    }

    /*removes every element of the list by poping them one by one*/
    void clear() {
        while(!empty()){
            pop_back();
        }
    }

    /* the insert method can insert one node at one axact position, pointed by List iterator.*/
    ListIterator<T> insert(ListIterator<T> const& position, T const& element) {
        if (position == begin()) {
            push_front(element);
            return begin();
        }
        else if (position == end()) {
            push_back(element);
            return end();
        }
        else {
            ListNode<T>* insert_Node = new ListNode<T>{ element, position.node->prev, position.node };
            position.node->prev->next = insert_Node;
            position.node->prev = insert_Node;
            ++size_;
            return ListIterator<T>{insert_Node};
        }
    }

    /* the erase methode works similar to the insert funktion, but instead of insearting it removes
    the node at the given position */
    ListIterator<T> erase(ListIterator<T> const& position) {
        if (empty()){
            throw "List is empty, better Luck next time";
        }
        else if (position == begin()) {
            pop_front();
            return begin();
        }
        else if (position == ListIterator<T>{last_}) {
            pop_back();
            return end();
        }
        else {
            auto tmp{ position };
            ++tmp;
            auto value{ tmp };
            position.node->prev->next = position.node->next;
            position.node->next->prev = position.node->prev;
            assert(nullptr != position.node);
            delete position.node;
            --size_;
            return ListIterator<T>{value};
        }
    }

    /* the reverse methode change the Order of the elements of the list, by swaping the last with node with the first by
    using the swap methode. so we have the reverse version of the methode*/
    void reverse() {
        if (empty() || size_ == 1) {
            return;
        }
        else {
            ListNode<T>* tmp = first_;
            std::swap(first_, last_);
            while(tmp != nullptr){
                std::swap(tmp->next, tmp->prev);
                tmp = tmp->prev;
            }
        }
    }


    /* the push front methode create a new node at the start of the list */
    void push_front(T const& element) {
        ListNode<T>* node = new ListNode<T>;
        node->value = element;
        node->prev = nullptr;
        if (empty()) {
            node->next = nullptr;
            first_ = node;
            last_ = node;
        }
        else if (first_ == last_){
            node->next = last_;
            first_ = node;
            last_->prev = first_;
        }
        else {
            node->next = first_;
            first_->prev = node;
            first_ = node;
        }
        size_++;
    }

    /* same as the push front methode, but instead of at the start it creates a new node at the end */
    void push_back(T const& element) {
        ListNode<T>* node = new ListNode<T>;
        node->value = element;
        node->next = nullptr;
        if (empty()) {
            node->prev = nullptr;
            first_ = node;
            last_ = node;
        }
        else if (first_ == last_) {
            node->prev = first_;
            last_ = node;
            first_->next = last_;
        }
        else {
            node->prev = last_;
            last_->next = node;
            last_ = node;
        }
        size_++;
    }

    /* the pop methode remove the first element of the list */
    void pop_front() {
      if(empty()) {
          throw "List is empty";
      }
      else if(first_ == last_) {
          delete first_;                //delete is used so theres no garbage left in the storrage
          first_ = nullptr;
          last_ = nullptr;
      }
      else {
          first_ = first_->next;
          delete first_->prev;          //delete is used so theres no garbage left in the storrage
          first_->prev = nullptr;
      }
      size_--;                      
    }

    /* removes the last element of the list */
    void pop_back() {
      if(empty()) {
        throw "List is empty";
      }
      else if (first_ == last_) {
          delete last_;             //delete is used so theres no garbage left in the storrage
          last_ = nullptr;
          first_ = nullptr;
      }
      else {
          last_ = last_->prev;
          delete last_->next;       //delete is used so theres no garbage left in the storrage
          last_->next = nullptr;
      }
      size_--;
    }

    /* gives back the value of the first node, important for test cases */
    T& front() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return first_->value;
      }
    }

    /* gives back the value of the last node, important for test cases */
    T& back() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return last_->value;
      }
    }

    /* checks if the list is empty */
    bool empty() const {
        if (first_ == nullptr && last_ == nullptr) {
            return true;
        }
        return false;
    };


    /* gives back how many elements are inside of the list */
    std::size_t size() const{
        return size_;
  };


  // list members
  private: 
    std::size_t size_;
    ListNode<T>* first_;
    ListNode<T>* last_;
};

/* free funktion which uses the same tamplate as the list. it basicly calls the reverse methode for the input list.*/
template<typename T>
void reverse(List<T> &list) {
    list.reverse();
}

/* ... */
template<typename T>
bool has_same_content(List<T> const& list, std::vector<T> const& vec) {
    std::vector<T> same_vec{};
    std::copy(list.begin(), list.end(), std::back_inserter(same_vec));
    if (vec == same_vec){
        return true;
    }
    else {
        return false;
    }
}

#endif // # define BUW_LIST_HPP