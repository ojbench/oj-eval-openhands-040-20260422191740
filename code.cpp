#include <iostream>
#include <memory>
#include <optional>

// Implementation of a doubly linked list using std::shared_ptr and std::weak_ptr
// Follow the provided skeleton and only fill in TODO parts.

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(std::move(node)) {}

        // Dereference operator to get value
        T& operator*() {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            return current->value;
        }

        // Post-increment operator
        iterator operator++(int) {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            iterator tmp = *this;
            if (current) current = current->next;
            return tmp;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            // 注：比较属于不同容器的非空迭代器是未定义行为，测试数据中不会出现
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        auto node = std::make_shared<Node>(value);
        node->next = head;
        if (head) {
            head->prev = node;
        } else {
            // was empty, tail becomes this node
            tail = node;
        }
        head = node;
        ++length;
    }

    // Add element to the back
    void push_back(const T& value) {
        auto node = std::make_shared<Node>(value);
        if (auto t = tail.lock()) {
            t->next = node;
            node->prev = t;
            tail = node;
        } else {
            // empty list
            head = node;
            tail = node;
        }
        ++length;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        T val = head->value;
        auto nxt = head->next;
        if (nxt) {
            // disconnect backward link
            nxt->prev.reset();
            head = nxt;
        } else {
            // becomes empty
            head.reset();
            tail.reset();
        }
        --length;
        return val;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        // if only one element
        if (!head->next) {
            T val = head->value;
            head.reset();
            tail.reset();
            --length;
            return val;
        }
        auto t = tail.lock();
        if (t) {
            T val = t->value;
            auto p = t->prev.lock();
            if (p) {
                p->next.reset();
                tail = p;
            } else {
                // fallback: list should have been handled above
                head.reset();
                tail.reset();
            }
            --length;
            return val;
        }
        // inconsistent state, treat as empty
        return std::nullopt;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

// No main function is provided because the evaluation system may include this file in its own test harness.
