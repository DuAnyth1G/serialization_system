#pragma once
#include <QDataStream>
#include <QString>

enum class FlowMode{
	FileRead,
	FileWrite
};

class Natant;
class Flow 
{
	enum Iteration
	{
		notSet= - 1,
		reseveCargo,
		saveObjects,
		saveCargo,
		Read
	};
	std::unique_ptr<QDataStream> out_ = nullptr;
	Iteration iteration_ = Iteration::notSet;
	QFile file_;
	QString filePath_;
	friend Natant;

public:
	Flow();
	~Flow();
	void open(FlowMode);
	void setFileName(const QString);

public:
	void seek(qint64);
	qint64 pos();

private:
	void beginReserveCargo();
	void beginSaveObjects();
	void beginSaveCargo();
	QDataStream* dataStream() const;
	Iteration iteration() const;
	QFileDevice::FileError openFileWrite(const QString& path);
	QFileDevice::FileError openFileRead(const QString& path);
};
