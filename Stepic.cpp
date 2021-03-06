﻿#include <algorithm>
#include <iostream>
#include <cassert> 
using namespace std;

namespace Stepic {
	// arrays
	int ** Create2dArray(size_t x, size_t y) {
		// low heap fragmentation method
		int ** arr = new int *[x];
		arr[0] = new int[x*y];
		for (size_t i = 1; i < x; i++)
			arr[i] = arr[i - 1] + y;
		return arr;
	}
	void free2dArray(int ** arr) {
		delete[] arr[0];
		delete[] arr;
	}
	void swap_min(int **mt, size_t m, size_t n) {
		int min = mt[0][0], minI = 0;
		for (size_t i = 0; i < m; i++)
			for (size_t j = 0; j < n; j++)
				if (mt[i][j] < min) {
					min = mt[i][j];
					minI = i;
				}
		if (minI != 0) {
			int * buf = mt[0];
			mt[0] = mt[minI];
			mt[minI] = buf;
		}
	}
	int ** transpose(const int * const * m, size_t x, size_t y) {
		int ** mt = new int *[y];
		mt[0] = new int[x*y];

		for (size_t i = 0; i < y; i++) {
			for (size_t j = 0; j < x; j++)
				mt[i][j] = m[j][i];
			mt[i + 1] = mt[i] + x;
		}
		return mt;
	}
	void printArray(const int * const * m, size_t x, size_t y) {
		cout << "****array****" << endl;
		for (size_t i = 0; i < x; i++) {
			for (size_t j = 0; j < y; j++)
				cout << m[i][j] << " ";
			cout << endl;
		}
	}
	// strings
	class String {
	private:
		char *str_;
	public:
		String(const String &str) {
			this->str_ = new char[str.size() + 1];
			strcpy(this->str_, str.c_str());
		}
		String(const char *str = "") {
			str_ = new char[strlen(str)];
			strcpy(this->str_, str);
		}
		String(size_t n, char c) {
			this->str_ = new char[n];
			for (size_t i = 0; i < n; i++)
				str_[i] = c;
		}
		~String() {
			delete[] this->str_;
		}

		String &operator=(const String &str) {
			if (this != &str) {
				String pattern(str);

				char *buf = this->str_;
				this->str_ = pattern.str_;
				pattern.str_ = buf;
			}
		}
		size_t size() const {
			return strlen(this->str_);
		}
		char &at(size_t idx) const {
			return str_[idx];
		}
		const char *c_str() const {
			return this->str_;
		}
		int compare(String &str) const {
			int thisSize = this->size();
			int thatSize = str.size();
			bool isThisFewer = this->size() < str.size() ? true : false;
			int minlen = isThisFewer ? thisSize : thatSize;

			for (size_t i = 0; minlen; i++) {
				if (this->at(i) == str.at(i))
					continue;

				if (this->at(i) > str.at(i))
					return 1;
				if (this->at(i) < str.at(i))
					return -1;
			}

			if (thisSize == thatSize)
				return 0;
			return isThisFewer ? -1 : 1;
		}
		void append(String &str) {
			char *buf = this->str_;
			this->str_ = new char[this->size() + str.size() + 1];

			// copy
			for (size_t i = 0; buf[i] != '\0'; i++)
				this->str_[i] = buf[i];
			// append
			for (size_t i = 0; str.at(i) != '\0'; i++)
				this->str_[strlen(buf) + i] = str.at(i);
			str_[strlen(buf) + str.size()] = '\0';

			delete[] buf;
		}
	};
	unsigned int strlen(const char *str) {
		size_t i = 0;
		for (; *(str + i) != '\0'; i++);
		return i;
	}
	int strstr(const char *str, const char *pattern) {
		size_t i = 0, j = 0; // i - str index, j - pattern index
		while (str[i] != '\0') {
			for (; str[i] == pattern[j]; i++, j++) {
				if (pattern[j] == '\0')
					return j;
			}
			i -= j + 1; //  +1 means i++
			j = 0;
		}
		return -1;
	}
	void strcat(char *to, const char *from) {
		// counting *to symbols
		int x = 0;
		while (to[x] != '\0')
			x++;
		// concat them
		for (size_t i = 0; from[i] != '\0'; i++)
			to[x++] = from[i];
		to[x] = '\0';
	}
	// I/O
	char *getline() {
		size_t count = 0, memory = 2;
		char next;
		char *result = new char[memory];
		while (cin.get(next)) {
			// end of line
			if (next == '\n')
				break;
			// if overflow        
			if (count + 1 > memory) {
				char *buf = result;
				result = new char[memory *= 2];
				for (size_t i = 0; i < count; i++)
					result[i] = buf[i];
				delete[] buf;
			}
			// add to a string
			result[count] = next;
			count++;
		}
		result[count] = '\0';
		return result;
	}
	void getclearline() {
		char last, next;

		// w8 until first symbols are blanks
		while (cin.get(next) && next == ' ');
		cout << next;
		last = next;

		// logic
		while (cin.get(next)) {
			if (last != ' ' || next != ' ')
				cout << next;
			last = next;
		}
	}
	// Object-oriented programming
	struct Scope {
		virtual ~Scope() { }
		virtual double variableValue(std::string const &name) const = 0;
	};
	struct Expression {
		virtual ~Expression() {}

		virtual double evaluate(Scope* s) const { return 2; } // it's a plug, forget about it ^)
	};
	struct Number : Expression {
		Number(double value) : value_(value) {}

		double value() const {
			return value_;
		}
		double evaluate(Scope* s) const {
			return value_;
		}

	private:
		double value_;
	};
	struct BinaryOperation : Expression {
		BinaryOperation(Expression const *left, int op, Expression const *right) : left_(left), op_(op), right_(right) {
			assert(left_ && right_);
		}
		~BinaryOperation() {
			delete left_;
			delete right_;
		}

		Expression const *left() const {
			return left_;
		}
		Expression const *right() const {
			return right_;
		}
		int operation() const {
			return op_;
		}
		double evaluate(Scope* s) const {
			double left = left_->evaluate(s);
			double right = right_->evaluate(s);

			switch (op_) {
			case PLUS: return left + right;
			case MINUS: return left - right;
			case DIV: return left / right;
			case MUL: return left * right;
			}
			assert(0);
			return 0.0;
		}

	private:
		enum {
			PLUS = '+',
			MINUS = '-',
			DIV = '/',
			MUL = '*'
		};
		Expression const *left_;
		Expression const *right_;
		int op_;
	};
	struct FunctionCall : Expression {
		FunctionCall(std::string const &name, Expression const *arg) : name_(name), arg_(arg) {
			assert(arg_);
			assert(name_ == "sqrt" || name_ == "abs");
		}
		~FunctionCall() {
			delete arg_;
		}

		std::string const & name() const {
			return name_;
		}
		Expression const *arg() const {
			return arg_;
		}
		double evaluate(Scope* s) const {
			double value = arg_->evaluate(s);
			if (name_ == "sqrt")
				return std::sqrt(value);
			if (name_ == "abs")
				return value < 0 ? -value : value;
		}

	private:
		std::string const name_;
		Expression const *arg_;
	};
	struct Variable : Expression {
		Variable(std::string const &name) : name_(name) { }

		std::string const &name() const { return name_; }
		double evaluate(Scope* s) const {
			return s->variableValue(name_);
		}

	private:
		std::string const name_;
	};
	class Rational {
	public:
		Rational(int numerator = 0, int denominator = 1) {
			this->_numerator = numerator;
			this->_denominator = denominator;
			this->_simplify();
		}
		double to_double() const {
			return (double)_numerator * (double)_denominator;
		}

		Rational inverse() const {
			return Rational(_numerator > 0 ? _denominator : -(int)_denominator, (unsigned)_numerator);
		}
		Rational operator +() const {
			// It's not abs() guys, trust me
			return Rational(_numerator, _denominator);
		}
		Rational operator -() const {
			return Rational(-_numerator, _denominator);
		}
		Rational & operator +=(Rational const & rnl) {
			unsigned sharedD = _lcm(_denominator, rnl._denominator);
			_numerator = (this->_denominator == sharedD ? _numerator : _numerator * sharedD / _denominator) +
				(rnl._denominator == sharedD ? rnl._numerator : rnl._numerator * sharedD / rnl._denominator);
			this->_simplify();

			return *this;
		}
		Rational & operator -=(Rational const & rnl) {
			return *this += -rnl;
		}
		Rational & operator *=(Rational const & rnl) {
			_numerator *= rnl._numerator;
			_denominator *= rnl._denominator;
			this->_simplify();

			return *this;
		}
		Rational & operator /=(Rational const & rnl) {
			*this *= rnl.inverse();
			return *this;
		}

	protected:
		int _numerator;
		unsigned _denominator;

		void _simplify() {
			int devisor = _gcd(_denominator, _numerator);
			do {
				_numerator /= devisor;
				_denominator /= devisor;

				devisor = _gcd(_denominator, _numerator);
			} while (devisor != 1);
		}

		// Greater common divisor
		int _gcd(int a, int b) {
			return b == 0 ? a : _gcd(b, a % b);
		}
		// Loweer common multiplier
		int _lcm(int a, int b) {
			return a * b / _gcd(a, b);
		}
	};
	Rational& operator +(Rational const & a, Rational const & b) {
		return Rational(a) += b;
	}
	Rational& operator -(Rational const & a, Rational const & b) {
		return Rational(a) -= b;
	}
	Rational& operator *(Rational const & a, Rational const & b) {
		return Rational(a) *= b;
	}
	Rational& operator /(Rational const & a, Rational const & b) {
		return Rational(a) /= b;
	}

	bool operator ==(Rational const & l, Rational const & r) {
		return ((l - r).to_double() == 0);
	}
	bool operator !=(Rational const & l, Rational const & r) {
		return !(l == r);
	}
	bool operator >(Rational const & l, Rational const & r) {
		return ((l - r).to_double() > 0);
	}
	bool operator <(Rational const & l, Rational const & r) {
		return (r > l);
	}
	bool operator >=(Rational const & l, Rational const & r) {
		return !(l < r);
	}
	bool operator <=(Rational const & l, Rational const & r) {
		return !(l > r);
	}
	class ScopedPtr {
	public:
		explicit ScopedPtr(Expression *ptr = 0) {
			this->ptr_ = ptr;
		}
		~ScopedPtr() {
			delete ptr_;
		}
		Expression* get() const {
			return ptr_;
		}
		Expression* release() {
			Expression* buf = ptr_;
			ptr_ = 0;
			return buf;
		}
		void reset(Expression *ptr = 0) {
			this->~ScopedPtr();
			ptr_ = ptr;
		}
		Expression& operator*() const {
			return *ptr_;
		}
		Expression* operator->() const {
			return ptr_;
		}

	private:
		// запрещаем копирование ScopedPtr
		ScopedPtr(const ScopedPtr&);
		ScopedPtr& operator=(const ScopedPtr&);

		Expression *ptr_;
	};
	class SharedPtr {
	public:
		explicit SharedPtr(Expression *ptr = 0) : links_(0) {
			if (ptr != 0)
				links_ = new short(1);
			ptr_ = ptr;
		}
		~SharedPtr() {
			if (ptr_ != 0 && --(*links_) == 0) {
				delete ptr_;
				delete links_;
			}
		}
		SharedPtr(const SharedPtr & p) : links_(0) {
			ptr_ = p.ptr_;
			if (p.ptr_ != 0) {
				this->links_ = p.links_;
				(*(this->links_))++;
			}
			else
				links_ = 0;
		}
		SharedPtr& operator=(const SharedPtr & k) {
			if (this->ptr_ != k.ptr_) {
				if (ptr_ != 0 && --(*links_) == 0) {
					delete ptr_;
					delete links_;
				}
				ptr_ = k.ptr_;
				if (k.ptr_ != 0) {
					links_ = k.links_;
					(*links_)++;
				}
				else
					links_ = 0;
			}
			return *this;
		}
		Expression* get() const {
			return ptr_;
		}
		void reset(Expression* ptr = 0) {
			if (ptr_ != ptr) {
				if (ptr_ != 0 && --(*links_) == 0) {
					delete ptr_;
					delete links_;
				}
				ptr_ = ptr;
				if (ptr != 0)
					this->links_ = new short(1);
				else
					links_ = 0;
			}
		}
		Expression& operator*() const {
			return *ptr_;
		}
		Expression* operator->() const {
			return ptr_;
		}
	private:
		short* links_;
		Expression* ptr_;
	};
	// templates
	template <typename T>
	class Array {
	public:
		Array(size_t size, const T& value = T()) {
			size_ = size;
			arr_ = (T*)(new char[size_ * sizeof(T)]);
			for (size_t i = 0; i < size_; i++)
				new (arr_ + i) T(value);
		}
		Array(const Array & arr) {
			size_ = arr.size_;
			arr_ = (T*)(new char[size_ * sizeof(T)]);
			for (size_t i = 0; i < size_; i++)
				new (arr_ + i) T(arr.arr_[i]);
		}
		~Array() {
			for (size_t i = 0; i < size_; i++)
				arr_[i].~T();
			delete[](char*)arr_;
		}
		Array& operator=(const Array arr) {
			if (arr_ != arr.arr_) {
				for (size_t i = 0; i < size_; i++)
					arr_[i].~T();
				delete[](char*)arr_;

				size_ = arr.size_;
				arr_ = (T*)(new char[size_ * sizeof(T)]);
				for (size_t i = 0; i < size_; i++)
					new (arr_ + i) T(arr[i]);
			}
		}

		size_t size() const {
			return size_;
		}
		T& operator[](size_t index) {
			return arr_[index];
		}
		const T& operator[](size_t index) const {
			return arr_[index];
		}
	private:
		T* arr_;
		size_t size_;
	};
	template <typename T>
	class Array < T* > {
	public:
		explicit Array(size_t size) : size_(size), data_(new T*[size]) {

		}

		T& operator[](size_t i) {
			return *data_[i];
		}

	private:
		size_t size_;
		T** data_;
	};

	struct ICloneable {
		virtual ICloneable* clone() const = 0;
		virtual ~ICloneable() { }
	};

	template <typename T>
	struct ValueHolder : ICloneable {
		ValueHolder(T data) :data_(data)  { }
		ICloneable* clone() const {
			return new ValueHolder(*this);
		}
		T data_;
	};

	class Any {
	public:
		// В классе Any должен быть конструктор,
		// который можно вызвать без параметров,
		// чтобы работал следующий код:
		//    Any empty; // empty ничего не хранит
		Any() {
			holder_ = 0;
		}

		// В классе Any должен быть шаблонный
		// конструктор от одного параметра, чтобы
		// можно было создавать объекты типа Any,
		// например, следующим образом:
		//    Any i(10); // i хранит значение 10
		template<typename T>
		Any(T obj) {
			holder_ = new ValueHolder<T>(obj);
		}

		// Не забудьте про деструктор. Все выделенные
		// ресурсы нужно освободить.
		~Any() {
			delete holder_;
		}

		// В классе Any также должен быть конструктор
		// копирования (вам поможет метод clone
		// интерфейса ICloneable)
		Any(const Any& other) {
			holder_ = other.holder_->clone();
		}

		// В классе должен быть оператор присваивания и/или
		// шаблонный оператор присваивания, чтобы работал
		// следующий код:
		//    Any copy(i); // copy хранит 10, как и i
		//    empty = copy; // empty хранит 10, как и copy
		//    empty = 0; // а теперь empty хранит 0
		Any& operator=(const Any& other) {
			if (this->holder_ != other.holder_) {
				delete holder_;
				if (other.holder_ != 0)
					holder_ = other.holder_->clone();
				else holder_ = 0;
			}
			return *this;
		}
		template<typename T>
		Any& operator=(const ValueHolder<T>& other) {
			if (*(this->holder_) != other) {
				delete holder_;
				if (other != 0)
					holder_ = other->clone();
				else holder_ = 0;
			}
			return *this;
		}

		// Ну и наконец, мы хотим уметь получать хранимое
		// значение, для этого определите в классе Any
		// шаблонный метод cast, который возвращает
		// указатель на хранимое значение, или нулевой
		// указатель в случае несоответствия типов или
		// если объект Any ничего не хранит:
		//    int *iptr = i.cast<int>(); // *iptr == 10
		//    char *cptr = i.cast<char>(); // cptr == 0,
		//        // потому что i хранит int, а не char
		//    Any empty2;
		//    int *p = empty2.cast<int>(); // p == 0
		// При реализации используйте dynamic_cast,
		// который мы уже обсуждали ранее.
		template<typename T> T* cast() {
			if (holder_ == 0)
				return 0;
			return &((dynamic_cast<ValueHolder<T>*>(holder_))->data_);
		}

	private:
		ICloneable* holder_;
	};

	template<typename U, typename T>
	void copy_n(U* out, T* in, int count) {
		for (size_t i = 0; i < count; i++) {
			out[i] = (U)in[i];
		}
	}
	template<typename T, typename Comparer>
	T minimum(Array<T> arr, Comparer compare) {
		T min = arr[0];
		for (size_t i = 1; i < arr.size(); i++)
			if (compare(arr[i], min))
				min = arr[i];
		return min;
	}

	// template overrides
	template <typename T>
	void flatten(const Array<T>& arr, std::ostream& out) {
		for (size_t i = 0; i < arr.size(); i++)
			out << arr[i] << " ";
	}
	template <typename T>
	void flatten(const Array< Array<T> >& arr, std::ostream& out) {
		for (size_t i = 0; i < arr.size(); i++)
			flatten(arr[i], out);
	}

	template<typename T, typename U>
	struct SameType {
		static const bool value = false;
	};
	template<typename T>
	struct SameType < T, T > {
		static const bool value = true;
	};

	//templated template
	string toString(int i){
		return string{ static_cast<char>(i) };
	};
	template<template <class> class Container>
	Container<string> toStrings(Container<int> const& c) {
		Container<string> result(c.size());
		for (size_t i = 0; i !- ar.size(); i++)
			result.get(i) = toString(c.get(i));
		return result;
	}
	// Some hacky things
	template<typename T, size_t N>
	size_t getSize(T(&arr)[N]) {
		return N;
	}
	bool isEqualType(Expression const *left, Expression const *right) {
		// check for type equal
		int vptrLeft = *(int*)left;
		int vptrRight = *(int*)right;

		if (vptrLeft == vptrRight)
			return true;
		else
			return false;
	}
}