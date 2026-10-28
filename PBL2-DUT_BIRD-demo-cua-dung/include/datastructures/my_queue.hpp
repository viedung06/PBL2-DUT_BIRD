#ifndef MY_QUEUE_HPP
#define MY_QUEUE_HPP

#include <cstddef>
#include <utility>

namespace datastructures {

/**
 * - Hoàn toàn độc lập, không dùng thư viện ngoài.
 * - Hỗ trợ Move Semantics hoàn hảo cho std::unique_ptr (push(T&&)).
 * - Thao tác push (tail), pop (head), front (head) đều đạt độ phức tạp O(1).
 * - Giải phóng bộ nhớ sạch sẽ, RAII an toàn (Zero Memory Leak).
 */
template <typename T>
class my_queue {
private:
    struct Node {
        T data;
        Node* next{nullptr};
        explicit Node(T&& val) : data(std::move(val)), next(nullptr) {}
        explicit Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    size_t m_size{0};

public:
    my_queue() = default;

    ~my_queue() {
        clear();
    }

    // Không cho phép Copy vì T có thể là std::unique_ptr
    my_queue(const my_queue&) = delete;
    my_queue& operator=(const my_queue&) = delete;

    // Hỗ trợ Move Semantics
    my_queue(my_queue&& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }

    my_queue& operator=(my_queue&& other) noexcept {
        if (this != &other) {
            clear();
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // Đẩy phần tử vào đuôi hàng đợi (Move) - O(1)
    void push(T&& value) {
        Node* newNode = new Node(std::move(value));
        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
        }
        ++m_size;
    }

    // Đẩy phần tử vào đuôi hàng đợi (Copy) - O(1)
    void push(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
        }
        ++m_size;
    }

    // Hủy phần tử ở đầu hàng đợi - O(1)
    void pop() {
        if (empty()) {
            return;
        }
        Node* temp = m_head;
        m_head = m_head->next;
        if (m_head == nullptr) {
            m_tail = nullptr;
        }
        delete temp; // Destructor của T tự động được gọi
        --m_size;
    }

    // Truy cập phần tử đầu hàng đợi - O(1)
    T& front() {
        return m_head->data;
    }

    const T& front() const {
        return m_head->data;
    }

    bool empty() const noexcept {
        return m_size == 0;
    }

    size_t size() const noexcept {
        return m_size;
    }

    // Xóa sạch toàn bộ hàng đợi - O(N)
    void clear() noexcept {
        while (!empty()) {
            pop();
        }
    }

    // Hàm duyệt phần tử (hỗ trợ update/render)
    template <typename Func>
    void forEach(Func&& func) {
        Node* curr = m_head;
        while (curr != nullptr) {
            func(curr->data);
            curr = curr->next;
        }
    }

    template <typename Func>
    void forEach(Func&& func) const {
        const Node* curr = m_head;
        while (curr != nullptr) {
            func(curr->data);
            curr = curr->next;
        }
    }
};

}

#endif
