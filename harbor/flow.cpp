#include "stdafx.h"
#include "flow.h"
#include "foundation/harbor/natant.h"

Flow::Flow()
{}

Flow::~Flow()
{
}

void Flow::open(FlowMode _openMode) {
	if (filePath_.isEmpty())
		return;
	switch (_openMode)
	{
	case FlowMode::FileRead:
		openFileRead(filePath_);
		return;
	case FlowMode::FileWrite:
		openFileWrite(filePath_);
		return;
	}
}

void Flow::setFileName(const QString _fileName) {
	filePath_ = _fileName;
}

void Flow::seek(qint64 _pos) {
	out_->device()->seek(_pos);
}

qint64 Flow::pos() {
	return out_->device()->pos();
}

QDataStream* Flow::dataStream() const {
	return out_.get();
}

void Flow::beginReserveCargo(){
	iteration_ = Iteration::reseveCargo;
}
void Flow::beginSaveObjects(){

	iteration_ = Iteration::saveObjects;
}
void Flow::beginSaveCargo(){
	iteration_ = Iteration::saveCargo;
}

Flow::Iteration Flow::iteration() const{
	return iteration_;
}
QFileDevice::FileError Flow::openFileRead(const QString& path) {
	file_.close();
	file_.setFileName(path);
	file_.open(QIODevice::ReadOnly);
	out_ = std::make_unique<QDataStream>(&file_);
	iteration_ = Iteration::Read;
	return file_.error();
}


QFileDevice::FileError Flow::openFileWrite(const QString & path)
{
	file_.close();
	file_.setFileName(path);
	file_.open(QIODevice::WriteOnly);
	out_ = std::make_unique<QDataStream>(&file_);
	iteration_ = Iteration::notSet;
	return file_.error();
}


