#include "stdafx.h"
#include "natant.h"
#include <QIODevice>

Natant::Natant()
{}

Natant::~Natant()
{}

void Natant::hookDeserialization() {
	startPos_ = pFlow_->pos();

	// читаем размер cargo 
	uchar size;
	*pFlow_->dataStream() >> size;
	// переходим в начало 
	pFlow_->seek(startPos_);
	QByteArray buffer(size, 0);
	pFlow_->dataStream()->readRawData(buffer.data(), size);
	cargo_.fill(buffer);
}

void Natant::hookSerialization() {
	startPos_ = pFlow_->pos();
}

void Natant::bookCargo() {
	//char blank{ 0 };
	//for (auto ind = 0; ind < cargo_.occupied(); ind++)
	//	*pFlow_->dataStream() << blank;
	pFlow_->dataStream()->writeRawData(cargo_.get().constData(), cargo_.occupied());
//	pFlow_->dataStream()->skipRawData(cargo_.occupied());
}

void Natant::setCargo() {
	pFlow_->seek(startPos_);
	pFlow_->dataStream()->writeRawData(cargo_.get().constData(), cargo_.occupied());
	cargo_.clear();
}

void Natant::unhook() {

}

//void Natant::seek(qint64 _pos) {
//	pFlow_->device()->seek(_pos);
//}
//
//qint64 Natant::pos() {
//	return pFlow_->device()->pos();
//}


void Natant::wrightToFile(Flow* _out) {
	pFlow_ = _out;

	hookSerialization();
	//  Do reservation
	pFlow_->beginReserveCargo();
	serialize();
	//  Reserv Cargo 
	bookCargo();
	//  Save Objects
	pFlow_->beginSaveObjects();
	serialize();
	//  Set Cargo
	pFlow_->beginSaveCargo();
	setCargo();
}

void Natant::readFromFile(Flow* _in) {
	pFlow_ = _in;
	hookDeserialization();
	// Read


	deserialize();

}

Flow& operator<<(Flow& _out, Natant& _nObject) {
	_nObject.wrightToFile(&_out);
	return _out;
}

Flow& operator>>(Flow& _in, Natant& _nObject) {
	_nObject.readFromFile(&_in);
	return _in;
}
