/// файл с примером переопредедения метотода serialize, deserialize для пользователя программиста
/// порядок сериализации членов класса не важен 
/// код приведен метообразно

#pragma once

#include <QString>
#include "foundation/common/calib/i_calib.h"
#include "foundation/harbor/natant.h"



class TOFCalib;
typedef std::shared_ptr<TOFCalib> TOFCalibPointer;

class TOFCalib : public ICalib, public Natant
{
    //Interface Natant
	enum Id : uchar {
		_TOFCalibType = 16,
		_Notation,

		_PointCount,
		_Masses,
		_Times,
	
		_CoefCount,
		_Coefs,
		_CoefNotations
	};

    Type type_;
	QString name_;
	
	int pointCount_ = 0;
	double* masses_ = nullptr;
	double* times_ = nullptr;
	
	int coefCount_ = 0;
	double* coefs_ = nullptr;
	QStringList coefNotations_;

public:
    // enum
    .....
	// };


	TOFCalib(const TOFCalib& calib);
	~TOFCalib();

	// void setName(const QString& name);
	// ........
	// bool operator !=(const TOFCalib& rCalib) const;
	// TOFCalib& operator=(const TOFCalib& rCalib);

	//Interface ICalib
	// virtual double calibrate(double fVal) override final;
	// virtual double decalibrate(double fVal) override final;


public:
	//Interface Natant
	virtual int deserialize();  
	virtual int serialize();   
};


int TOFCalib::deserialize() {

	getValue(_TOFCalibType, type_);
	int oldPointCount = pointCount_;
	getValue(_PointCount, pointCount_);
	getValue(_CoefCount, coefCount_);

	getObject(_Notation, name_);
	if (oldPointCount && oldPointCount != pointCount_) {
		delete masses_;
		masses_ = nullptr;
	}

	if (!masses_) masses_ = new double[pointCount_];
	getArray(_Masses, masses_, pointCount_);
	if (!times_) times_ = new double[pointCount_];
	getArray(_Times, times_, pointCount_);
	if (!coefs_) coefs_ = new double[coefCount_];
	getArray(_Coefs, coefs_, coefCount_);

	getObject(_CoefNotations, coefNotations_);
		
	return 0;
}

int TOFCalib::serialize() {
	setValue(_TOFCalibType, type_);
	setValue(_CoefCount, coefCount_);
	setValue(_PointCount, pointCount_);

	setObject(_Notation, name_);
	
	setArray(_Masses, masses_, pointCount_);
	setArray(_Times, times_, pointCount_);
	setArray(_Coefs, coefs_, coefCount_);

	setObject(_CoefNotations, coefNotations_);

	return 0;
}

/// порядок сериализации членов класса не важен 