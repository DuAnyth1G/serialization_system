#include "stdafx.h"
#include "cargo.h"

Cargo::Cargo() : byteTabel_(1, 0)
{
	reserve(256);
}

Cargo::Cargo(const QByteArray& _byteTabel) : byteTabel_(_byteTabel)
{}

Cargo::~Cargo()
{}

void Cargo::init() {
}

uchar Cargo::occupied() const {
	return byteTabel_.size();
}

void Cargo::fill(const QByteArray& _byteTabel) {
	byteTabel_ = _byteTabel;
}

const QByteArray& Cargo::get() const {
	return byteTabel_;
}

void Cargo::clear() {
	byteTabel_.clear();
}

void Cargo::reserve(qsizetype _size) {
	byteTabel_.reserve(_size);
}

void Cargo::squeeze() {
	byteTabel_.squeeze();
}

Cargo::iterator::pointer Cargo::findLot(uchar _id) {
	for (auto it = this->begin(), end = this->end(); it != end; ++it)
		if (_id == Tackle::lotIdValidator(*it))
			return it.getLot_ptr();
	return nullptr;
}

bool Cargo::chekLot(uchar _id) {
	for (auto&& it : *this)
		if (_id == Tackle::lotIdValidator(it))
			return true;
	return false;
}

Cargo::iterator::pointer Cargo::book(uchar _id, uchar _size) {
	// расширение памяти под регестрируемый объект 
	auto const tabelSize = occupied();
	byteTabel_.resize(tabelSize + _size + 1);
	auto* ptr = byteTabel_.data() + tabelSize;
	
	// создание id с первыми двумя значащими битами 
	// первые два бита id, отвечают степени двойки размера лежашего далее элементарного типа
	Tackle::raiseLotId(_id, _size);
	
	// записываем id
	memcpy(ptr, &_id, 1);
	
	// обновляем значение размера 
	byteTabel_[0] = occupied();
	
	return ptr;
}

Cargo::iterator::iterator(pointer ptr) : ptr_(ptr)
{}

Cargo::iterator::pointer Cargo::iterator::getLot_ptr() {
	return ptr() + 1;
}

Cargo::iterator::pointer Cargo::iterator::getId_ptr() {
	return ptr();
}

// Prefix increment
Cargo::iterator& Cargo::iterator::operator++() {
	ptr_ = ptr_ + 1 + Tackle::getLotSize(*ptr_);
	return *this;
}

// Postfix increment
Cargo::iterator Cargo::iterator::operator++(int) {
	iterator retval = *this;
	++(*this);
	return retval;
}

Cargo::iterator Cargo::begin() {
	return iterator(byteTabel_.data() + 1);
}

Cargo::iterator Cargo::end() {
	return iterator(byteTabel_.data() + byteTabel_.size());
}