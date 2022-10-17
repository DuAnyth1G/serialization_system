#pragma once

#include <QByteArray>
#include "foundation/tackle/tackle.h"


class Cargo
{

	QByteArray byteTabel_;

public:
	// member typedefs provided through inheriting from std::iterator
	class iterator : public std::iterator <
		std::input_iterator_tag,   // iterator_category
		char,                      // value_type
		char,                      // difference_type
		char*,                     // pointer
		char&                      // reference
	> 
	{
	private:
		pointer ptr_ = nullptr;

	public:
		explicit iterator(pointer ptr);
		reference operator*()  const { return *ptr_; }
		pointer   operator->() const { return  ptr_; }
		pointer ptr() const { return  ptr_; };
		pointer getLot_ptr();
		pointer getId_ptr();
		iterator& operator++();   // Prefix increment
		iterator operator++(int); // Postfix increment

		bool operator==(iterator other) const { return ptr_ == other.ptr_; }
		bool operator!=(iterator other) const { return !(*this == other); }

	};

	iterator begin();
	iterator end();

public:
	Cargo();
	Cargo(const QByteArray& _byteTabel);
	~Cargo();
	uchar occupied() const;
	const QByteArray& get() const; 
	void fill(const QByteArray&);
	void clear();
	void reserve(qsizetype size);
	void squeeze();
	
	iterator::pointer findLot(uchar _id);
	bool chekLot(uchar _id);
	iterator::pointer book(uchar _id, uchar _size);

	template<typename Type>
	void setLot(uchar _id, Type _value);

	template<typename Type>
	void checkIn(uchar _id, Type _value);

private:
	void init();
};

template<typename Type>
inline void Cargo::setLot(uchar _id, Type _value) {
	auto* ptr = findLot(_id);
	if (ptr)
		memcpy(ptr, &_value, sizeof(Type));
	else
		qDebug() << "Error setLot";
}

template<typename Type>
inline void Cargo::checkIn(uchar _id, Type _value) {
	// расширение памяти под регестрируемый объект
	auto* ptr = book(_id, sizeof(Type));

	// заполение выделенной памяти
	memcpy(ptr + 1, &_value, sizeof(Type));
}

