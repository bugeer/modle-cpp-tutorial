#include <algorithm>
#include <cstdio>
#include <utility>

enum class shape_type {
    circle,
    triangle,
    rectangle
};

class shape {
public:
    virtual ~shape() {
        puts("~shape");
    }
};

class circle : public shape {
public:
    ~circle() {
        puts("~circle");
    }
};

class triangle : public shape {
public:
    ~triangle() {
        puts("~triangle");
    }
};

class rectangle : public shape {
public:
    ~rectangle() {
        puts("~rectangle");
    }
};

shape* create_shape(shape_type type) {
    switch (type) {
        case shape_type::circle:
            return new circle();
        case shape_type::triangle:
            return new triangle();
        case shape_type::rectangle:
            return new rectangle();
        default:
            return new circle();
    }
}

class shared_count {
public:
    shared_count()noexcept : count_(1) {
    }
    void add_count() noexcept {
        ++count_;
    }
    long reduce_count() noexcept {
        return --count_;
    }
    long get_count() noexcept {
        return count_;
    }

private:
    long count_;
};

template<typename T>
class smart_ptr {
    template<typename U>
    friend class smart_ptr;

public:
    explicit smart_ptr(T* ptr = nullptr): ptr_(ptr) {
        if(ptr) {
            shared_count_ = new shared_count();
        }
    }
    ~smart_ptr() {
        if(ptr_ && !shared_count_->reduce_count()) {
            delete ptr_;
            delete shared_count_;
        }
    }

    // smart_ptr(const smart_ptr&) = delete;
    // smart_ptr& operator=(const smart_ptr&) = delete;

    T* get() const {
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }
    T& operator->() const {
        return ptr_;
    }
    operator bool() const {
        return ptr_;
    }

    smart_ptr(const smart_ptr& other) {
        ptr_ = other.ptr_;
        if(ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }
    smart_ptr(smart_ptr&& other) {
        ptr_ = other.release();
    }
    template<typename U>
    smart_ptr(const smart_ptr<U>& other) noexcept {
        ptr_ = other.ptr_;
        if(ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }
    template<typename U>
    smart_ptr(const smart_ptr<U>&& other) noexcept {
        ptr_ = other.ptr_;
        if(ptr_) {
            shared_count_ = other.shared_count_;
            other.ptr_ = nullptr;
        }
    }
    template<typename U>
    smart_ptr(const smart_ptr<U>& other, T* ptr) noexcept {
        ptr_ = ptr;
        if(ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }

    smart_ptr& operator=(smart_ptr rhs) noexcept {
        // smart_ptr(rhs).swap(*this);
        // return *this;
        rhs.swap(*this);
        return *this;
    }
    T* release() {
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }
    T* get() {
        return ptr_;
    }
    long use_count() const noexcept {
        if(ptr_) {
            return shared_count_->get_count();
        } else {
            return 0;
        }
    }
    void swap(smart_ptr& rhs) {
        using std::swap;
        swap(ptr_, rhs.ptr_);
        swap(shared_count_, rhs.shared_count_);
    }

private:
    T *ptr_;
    shared_count* shared_count_;
};

class shape_wrapper {
public:
    explicit shape_wrapper(shape* ptr = nullptr): ptr_(ptr) {
    }

    ~shape_wrapper() {
        delete ptr_;
    }

    shape* get() const {
        return ptr_;
    }

private:
    shape* ptr_;
};


template <typename T>
void swap(smart_ptr<T>& lhs, smart_ptr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename T, typename U>
smart_ptr<T> static_pointer_cast(const smart_ptr<U>& other) noexcept {
    T* ptr = static_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> reinterpret_pointer_cast(const smart_ptr<U>& other) noexcept {
    T* ptr = reinterpret_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> const_pointer_cast(const smart_ptr<U>& other) noexcept {
    T* ptr = const_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> dynamic_pointer_cast(const smart_ptr<U>& other) noexcept {
    T* ptr = dynamic_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

int main (int argc, char *argv[]) {
    smart_ptr<shape> ptr1{create_shape(shape_type::circle)};
    // smart_ptr<shape> ptr2{ptr1};
    smart_ptr<shape> ptr3;
    // ptr3 = ptr1;
    ptr3 = std::move(ptr1);
    smart_ptr<shape> ptr4{std::move(ptr3)};

    return 0;
}
