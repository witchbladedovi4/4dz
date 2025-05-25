#include <initializer_list>
#include <stdexcept>

template <typename Type>
class List {
public:
    List() : start_(nullptr), end_(nullptr), size_(0) {}

    explicit List(size_t size) : List() {
        for (size_t i = 0; i < size; ++i) {
            push_back(Type());
        }
    }

    List(std::initializer_list<Type> obj) : List() {
        for (const auto& item : obj) {
            push_back(item);
        }
    }

    List(const List& other) : List() {
        auto current = other.start_;
        while (current != nullptr) {
            push_back(current->getData());
            current = current->getNext();
        }
    }

    List(List&& other) noexcept
        : start_(other.start_), end_(other.end_), size_(other.size_) {
        other.start_ = nullptr;
        other.end_ = nullptr;
        other.size_ = 0;
    }

    ~List() {
        clear();
    }

    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            auto current = other.start_;
            while (current != nullptr) {
                push_back(current->getData());
                current = current->getNext();
            }
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            start_ = other.start_;
            end_ = other.end_;
            size_ = other.size_;
            other.start_ = nullptr;
            other.end_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    Type& operator[](size_t index) {
        return at(index);
    }

    const Type& operator[](size_t index) const {
        return at(index);
    }

    Type& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = start_;
        for (size_t i = 0; i < index; ++i) {
            current = current->getNext();
        }
        return current->getData();
    }

    const Type& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = start_;
        for (size_t i = 0; i < index; ++i) {
            current = current->getNext();
        }
        return current->getData();
    }

    Type& front() {
        if (empty()) throw std::out_of_range("List is empty");
        return start_->getData();
    }

    const Type& front() const {
        if (empty()) throw std::out_of_range("List is empty");
        return start_->getData();
    }

    Type& back() {
        if (empty()) throw std::out_of_range("List is empty");
        return end_->getData();
    }

    const Type& back() const {
        if (empty()) throw std::out_of_range("List is empty");
        return end_->getData();
    }

    void push_back(const Type& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            start_ = end_ = newNode;
        }
        else {
            newNode->setPrev(end_);
            end_->setNext(newNode);
            end_ = newNode;
        }
        ++size_;
    }

    void push_front(const Type& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            start_ = end_ = newNode;
        }
        else {
            newNode->setNext(start_);
            start_->setPrev(newNode);
            start_ = newNode;
        }
        ++size_;
    }

    void pop_back() {
        if (empty()) return;

        Node* toDelete = end_;
        if (size_ == 1) {
            start_ = end_ = nullptr;
        }
        else {
            end_ = end_->getPrev();
            end_->setNext(nullptr);
        }
        delete toDelete;
        --size_;
    }

    void pop_front() {
        if (empty()) return;

        Node* toDelete = start_;
        if (size_ == 1) {
            start_ = end_ = nullptr;
        }
        else {
            start_ = start_->getNext();
            start_->setPrev(nullptr);
        }
        delete toDelete;
        --size_;
    }

    void insert(const Type& value, size_t index) {
        if (index > size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            push_front(value);
        }
        else if (index == size_) {
            push_back(value);
        }
        else {
            Node* current = start_;
            for (size_t i = 0; i < index - 1; ++i) {
                current = current->getNext();
            }

            Node* newNode = new Node(value, current->getNext(), current);
            current->getNext()->setPrev(newNode);
            current->setNext(newNode);
            ++size_;
        }
    }

    void erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            pop_front();
        }
        else if (index == size_ - 1) {
            pop_back();
        }
        else {
            Node* current = start_;
            for (size_t i = 0; i < index; ++i) {
                current = current->getNext();
            }
            current->getPrev()->setNext(current->getNext());
            current->getNext()->setPrev(current->getPrev());

            delete current;
            --size_;
        }
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

private:
    class Node {
    public:
        Node(const Type& data, Node* next = nullptr, Node* prev = nullptr)
            : data_(data), next_(next), prev_(prev) {
        }

        Type& getData() { return data_; }
        const Type& getData() const { return data_; }
        Node* getNext() const { return next_; }
        Node* getPrev() const { return prev_; }

        void setData(const Type& data) { data_ = data; }
        void setNext(Node* next) { next_ = next; }
        void setPrev(Node* prev) { prev_ = prev; }

    private:
        Type data_;
        Node* next_;
        Node* prev_;
    };

    Node* start_;
    Node* end_;
    size_t size_;
};

using MyType = double;
typedef double Duration;

#include <iostream>

int main() {
    List<int> myList = { 1, 2, 3 };
    myList.push_back(4);
    myList.insert(10, 1);

    for (size_t i = 0; i < myList.size(); ++i) {
        std::cout << myList[i] << " ";
    }

    return 0;
}