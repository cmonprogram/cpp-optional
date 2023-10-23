#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
	using exception::exception;

	virtual const char* what() const noexcept override {
		return "Bad optional access";
	}
};

template <typename T>
class Optional {
public:
	Optional() = default;
	Optional(const T& value) {
		data_value_ = new (&data_[0]) T(value);
		is_initialized_ = true;
	}
	Optional(T&& value) {
		data_value_ = new (&data_[0]) T(std::move(value));
		is_initialized_ = true;
	}
	Optional(const Optional& other) {
		if (!is_initialized_ && !other.is_initialized_) {
		}
		else if (!is_initialized_ && other.is_initialized_) {
			data_value_ = new (&data_[0]) T(other.Value());
			is_initialized_ = true;
		}
		else if (is_initialized_ && !other.is_initialized_) {
			Reset();
		}
		else {
			*data_value_ = other.Value();
			is_initialized_ = true;
		}
	}
	Optional(Optional&& other) {
		if (!is_initialized_ && !other.is_initialized_) {
		}
		else if (!is_initialized_ && other.is_initialized_) {
			data_value_ = new (&data_[0]) T(std::move(other.Value()));
			is_initialized_ = true;
		}
		else if (is_initialized_ && !other.is_initialized_) {
			Reset();
		}
		else {
			*data_value_ = std::move(other.Value());
			is_initialized_ = true;
		}
	}
	Optional& operator=(const T& value) {
		if (!is_initialized_) {
			data_value_ = new (&data_[0]) T(value);
			is_initialized_ = true;
		}
		else {
			*data_value_ = value;
			is_initialized_ = true;
		}
		return *this;
	}
	Optional& operator=(T&& value) {
		if (!is_initialized_) {
			data_value_ = new (&data_[0]) T(std::move(value));
			is_initialized_ = true;
		}
		else {
			*data_value_ = std::move(value);
			is_initialized_ = true;
		}
		return *this;
	}
	Optional& operator=(const Optional& rhs) {
		if (!is_initialized_ && !rhs.is_initialized_) {
		}
		else if (!is_initialized_ && rhs.is_initialized_) {
			data_value_ = new (&data_[0]) T(rhs.Value());
			is_initialized_ = true;
		}
		else if (is_initialized_ && !rhs.is_initialized_) {
			Reset();
		}
		else {
			*data_value_ = rhs.Value();
			is_initialized_ = true;
		}
		return *this;
	}
	Optional& operator=(Optional&& rhs) {
		if (!is_initialized_ && !rhs.is_initialized_) {
		}
		else if (!is_initialized_ && rhs.is_initialized_) {
			data_value_ = new (&data_[0]) T(std::move(rhs.Value()));
			is_initialized_ = true;
		}
		else if (is_initialized_ && !rhs.is_initialized_) {
			Reset();
		}
		else {
			*data_value_ = std::move(rhs.Value());
			is_initialized_ = true;
		}
		return *this;
	}

	~Optional() {
		if (is_initialized_) {
			//operator delete[](&data_[0]);
			data_value_->~T();
			data_value_ = nullptr;
			is_initialized_ = false;
		}
	}

	bool HasValue() const {
		return is_initialized_;
	}

	// Операторы * и -> не должны делать никаких проверок на пустоту Optional.
	// Эти проверки остаются на совести программиста
	T& operator*() {
		return *data_value_;
	}
	const T& operator*() const {
		return *data_value_;
	}
	T* operator->() {
		return data_value_;
	}
	const T* operator->() const {
		return data_value_;
	}

	// Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
	T& Value() {
		if (!is_initialized_) throw BadOptionalAccess();
		return *data_value_;
	}
	const T& Value() const {
		if (!is_initialized_) throw BadOptionalAccess();
		return *data_value_;
	}

	void Reset() {
		if (is_initialized_) {
			data_value_->~T();
			data_value_ = nullptr;
			is_initialized_ = false;
		}
	}

private:
	// alignas нужен для правильного выравнивания блока памяти
	alignas(T) char data_[sizeof(T)];
	bool is_initialized_ = false;
	T* data_value_ = nullptr;

};