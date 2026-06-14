// COMP 2450 — Floor 6 starter
// hero/Chain.h — doubly-linked container template with iterators.
//
// As-of state at the start of Floor 6: everything from Floor 4½ and
// Floor 5 is in place. Doubly-linked storage with the full Rule of
// Three (deep copy ctor + copy-and-swap operator=), push_front /
// push_back / pop_front / pop_back, plus working iterator and
// const_iterator with all five operators, plus the reverse-iterator
// helpers.
//
// You will not edit this file on Floor 6. It is the substrate that
// Stack<T> will adapt over.

#pragma once

#include <cstddef>
#include <iterator>
#include <utility>

namespace dungeon {

namespace detail {
struct NodeStats {
    static inline std::size_t allocations   = 0;
    static inline std::size_t deallocations = 0;
    static void reset() { allocations = 0; deallocations = 0; }
};
}  // namespace detail


template <typename T>
class Chain {
public:
    struct Node {
        T     data;
        Node* prev;
        Node* next;

        explicit Node(const T& v, Node* p = nullptr, Node* n = nullptr)
            : data(v), prev(p), next(n) {
            ++detail::NodeStats::allocations;
        }
        ~Node() { ++detail::NodeStats::deallocations; }
    };

    // The five typedefs let std::iterator_traits<iterator> work — that's
    // how std::find_if, std::reverse, range-based for, and friends
    // discover what your iterator can do.
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator() : p_(nullptr), owner_(nullptr) {}
        iterator(Node* n, Chain* o) : p_(n), owner_(o) {}

        reference operator*()  const { return p_->data; }
        pointer   operator->() const { return &p_->data; }

        iterator& operator++()    { p_ = p_->next; return *this; }
        iterator  operator++(int) { iterator t = *this; ++(*this); return t; }

        // When p_ is null (we're at end()), --it hops to owner_->tail_
        // instead of dereferencing nullptr->prev. That's why iterators
        // carry the owner_ back-pointer.
        iterator& operator--()    { p_ = p_ ? p_->prev : owner_->tail_; return *this; }
        iterator  operator--(int) { iterator t = *this; --(*this); return t; }

        // == compares POSITION (the underlying pointer), not value.
        bool operator==(const iterator& other) const { return p_ == other.p_; }
        bool operator!=(const iterator& other) const { return !(*this == other); }

        Node*  node()  const { return p_; }
        Chain* owner() const { return owner_; }

    private:
        Node*  p_;
        Chain* owner_;
    };

    // const_iterator is a DIFFERENT TYPE that callers can ask for
    // (via cbegin/cend) even from a non-const Chain. The implicit
    // const_iterator(const iterator&) ctor lets non-const code freely
    // convert; the reverse is banned.
    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        const_iterator() : p_(nullptr), owner_(nullptr) {}
        const_iterator(const Node* n, const Chain* o) : p_(n), owner_(o) {}
        const_iterator(const iterator& it) : p_(it.node()), owner_(it.owner()) {}

        reference operator*()  const { return p_->data; }
        pointer   operator->() const { return &p_->data; }

        const_iterator& operator++()    { p_ = p_->next; return *this; }
        const_iterator  operator++(int) { const_iterator t = *this; ++(*this); return t; }

        const_iterator& operator--()    { p_ = p_ ? p_->prev : owner_->tail_; return *this; }
        const_iterator  operator--(int) { const_iterator t = *this; --(*this); return t; }

        bool operator==(const const_iterator& other) const { return p_ == other.p_; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }

    private:
        const Node*  p_;
        const Chain* owner_;
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Chain() = default;
    ~Chain() { clear(); }

    Chain(const Chain& other) {
        for (const Node* p = other.head_; p != nullptr; p = p->next) {
            push_back(p->data);
        }
    }

    Chain& operator=(const Chain& other) {
        Chain tmp(other);
        swap(tmp);
        return *this;
    }

    void swap(Chain& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    std::size_t size()  const { return size_; }
    bool        empty() const { return size_ == 0; }

    const Node* head() const { return head_; }
    Node*       head()       { return head_; }
    const Node* tail() const { return tail_; }
    Node*       tail()       { return tail_; }

    // end() returns iterator(nullptr, this) — the sentinel "one past the
    // last." A null-terminated chain means the tail's `next` IS the end
    // sentinel; no extra allocated node is needed.
    iterator begin() { return iterator(head_,   this); }
    iterator end()   { return iterator(nullptr, this); }

    const_iterator begin()  const { return const_iterator(head_,   this); }
    const_iterator end()    const { return const_iterator(nullptr, this); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }

    reverse_iterator       rbegin()        { return reverse_iterator(end()); }
    reverse_iterator       rend()          { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin()  const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()    const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend()   const { return rend(); }

    void push_front(const T& value) {
        Node* n = new Node(value, nullptr, head_);
        if (head_ != nullptr) head_->prev = n;
        else                  tail_       = n;
        head_ = n;
        ++size_;
    }

    void push_back(const T& value) {
        Node* n = new Node(value, tail_, nullptr);
        if (tail_ != nullptr) tail_->next = n;
        else                  head_       = n;
        tail_ = n;
        ++size_;
    }

    void pop_front() {
        if (head_ == nullptr) return;
        Node* old_head = head_;
        Node* new_head = old_head->next;
        delete old_head;
        head_ = new_head;
        if (new_head != nullptr) new_head->prev = nullptr;
        else                     tail_          = nullptr;
        --size_;
    }

    void pop_back() {
        if (tail_ == nullptr) return;
        Node* old_tail = tail_;
        Node* new_tail = old_tail->prev;
        delete old_tail;
        tail_ = new_tail;
        if (new_tail != nullptr) new_tail->next = nullptr;
        else                     head_          = nullptr;
        --size_;
    }

    void clear() {
        Node* p = head_;
        while (p != nullptr) {
            Node* n = p->next;
            delete p;
            p = n;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

private:
    Node*       head_ = nullptr;
    Node*       tail_ = nullptr;
    std::size_t size_ = 0;
};

template <typename T>
void swap(Chain<T>& a, Chain<T>& b) noexcept { a.swap(b); }

}  // namespace dungeon
