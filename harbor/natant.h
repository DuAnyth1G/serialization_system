#pragma once

#include <QDataStream>
#include "foundation/harbor/cargo.h"
#include "foundation/harbor/flow.h"
class Natant /*vessel, ship, boat, craft, bedpan, ark*/
{
protected:
	Cargo cargo_;
	Flow* pFlow_ = nullptr;
	int startPos_ = 0;
public:
	Natant();
	~Natant();
	
	// for deserialize
	void hookDeserialization();  
	void hookSerialization();  
	
	void bookCargo();
	void setCargo();
	
	void unhook();
/*	void seek(qint64);
	qint64 pos()*/;
	virtual int voyage() = 0;
	virtual int deserialize() = 0;   //deserialize
	virtual int serialize() = 0;     //serialize
	template<typename Type>
	void getValue(uchar _id, Type& _value);
	template<typename Type>
	void setValue(uchar _id, Type& _value);
	template<typename Object>
	void getObject(uchar _id, Object& _object);
	template<typename Object>
	void setObject(uchar _id, Object& _object);
	template<typename ArrayElement>
	void getArray(uchar _id, ArrayElement* _pointer, qint64 size);
	template<typename ArrayElement>
	void setArray(uchar _id, ArrayElement* _pointer, qint64 size);
	
	friend Flow& operator<<(Flow&, Natant&);
	friend Flow& operator>>(Flow&, Natant&);
private:
	void wrightToFile(Flow*);
	void readFromFile(Flow*);
};

template<typename Type>
inline void Natant::getValue(uchar _id, Type& _value) {
	for (auto it = cargo_.begin(), end = cargo_.end(); it != end; ++it)
		if (_id == Tackle::lotIdValidator(*it)) {
			_value = *reinterpret_cast<Type*>(it.getLot_ptr());
			return;
		}
	_value = Type();
}

template<typename Type>
inline void Natant::setValue(uchar _id, Type& _value) {
	switch (pFlow_->iteration())
	{
	case Flow::Iteration::reseveCargo:
		cargo_.checkIn(_id, _value);
		return;
	}
}

template<typename Object>
inline void Natant::getObject(uchar _id, Object& _object) {
	pFlow_->seek(*reinterpret_cast<qint64*>(cargo_.findLot(_id)));
	*pFlow_->dataStream() >> _object;
}

template<typename Object>
inline void Natant::setObject(uchar _id, Object& _object) {
	switch (pFlow_->iteration())
	{
	case Flow::Iteration::reseveCargo:
		cargo_.book(_id, sizeof(qint64));
		return;
	case Flow::Iteration::saveObjects:
		cargo_.setLot(_id, pFlow_->pos());
		*pFlow_->dataStream() << _object;
		return;
	}
	
}

template<typename ArrayElement>
inline void Natant::getArray(uchar _id, ArrayElement* _pointer, qint64 size) {
	pFlow_->seek(*reinterpret_cast<qint64*>(cargo_.findLot(_id)));
	pFlow_->dataStream()->readRawData(reinterpret_cast<char*>(_pointer), sizeof(ArrayElement) * size);
}

template<typename ArrayElement>
inline void Natant::setArray(uchar _id, ArrayElement* _pointer, qint64 size) {
	switch (pFlow_->iteration())
	{
	case Flow::Iteration::reseveCargo:
		cargo_.book(_id, sizeof(qint64));
		return;
	case Flow::Iteration::saveObjects:
		cargo_.setLot(_id, pFlow_->pos());
		pFlow_->dataStream()->writeRawData(reinterpret_cast<char*>(_pointer), sizeof(ArrayElement) * size);
		return;
	}
}
