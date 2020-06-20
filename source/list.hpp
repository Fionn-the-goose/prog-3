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
  }  //call it->method() or it->member


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


  /* ... */
  bool operator==(ListIterator<T> const& x) const {
      return node == x.node;
  } // call it: == it

  /* ... */
  bool operator!=(ListIterator<T> const& x) const {
      return !(node == x.node);
  } // call it: != it

  /* Advances Iterator */
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

    // test and implement:
    // TODO: Move-Konstruktor (Aufgabe 3.9)

    //TODO: Initializer-List Konstruktor (3.10 - Teil 1)
    /* ... */
    // test and implement:
    List(std::initializer_list<T> ini_list) {
      //not implemented yet
    }

    /* ... */
    //(unifying) Assignment operator
    void swap(List<T> list) {
        std::swap(first_, list.first_);
        std::swap(last_, list.last_);
    }

    List& operator= (List<T> list) {
        swap(list);
        return *this;
    }

    /* ... */
    // test and implement:
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

    bool operator!=(List const& rhs) const
    {
        if (*this == rhs){
            return false;
        }
        else {
            return true;
        }
    }

    /* ... */
    ~List() {
        clear();
    }

    /* ... */
    ListIterator<T> begin() {
        ListIterator<T> begin;
        begin.node = first_;
        return begin;
    }

    /* ... */
    ListIterator<T> end() {
        ListIterator<T> end;
        end.node = nullptr;
        return end;
    }

    /* ... */ 
    void clear() {
        while(!empty()){
            pop_back();
        }
    }
    /* ... */
    //TODO: member function insert (Aufgabe 3.13)

    /* ... */
    //TODO: member function insert (Aufgabe 3.14)

    /* ... */
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


    /*  */
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

    /* ... */
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

    /* ... */
    void pop_front() {
      if(empty()) {
          throw "List is empty";
      }
      else if(first_ == last_) {
          delete first_;
          first_ = nullptr;
          last_ = nullptr;
      }
      else {
          first_ = first_->next;
          delete first_->prev;
          first_->prev = nullptr;
      }
      size_--;
    }

    /* ... */
    void pop_back() {
      if(empty()) {
        throw "List is empty";
      }
      else if (first_ == last_) {
          delete last_;
          last_ = nullptr;
          first_ = nullptr;
      }
      else {
          last_ = last_->prev;
          delete last_->next;
          last_->next = nullptr;
      }
      size_--;
    }

    /* ... */
    T& front() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return first_->value;
      }
    }

    /* ... */
    T& back() {
      if(empty()) {
        throw "List is empty";
      }
      else {
          return last_->value;
      }
    }

    /* ... */
    bool empty() const {
        if (first_ == nullptr && last_ == nullptr) {
            return true;
        }
        return false;
    };


    /* ... */
    std::size_t size() const{
        return size_;
  };


  // list members
  private: 
    std::size_t size_;
    ListNode<T>* first_;
    ListNode<T>* last_;
};

/* ... */
template<typename T>
void reverse(List<T> &list) {
    list.reverse();
}

/* ... */
//TODO: Freie Funktion operator+ (3.10 - Teil 2)


#endif // # define BUW_LIST_HPP
