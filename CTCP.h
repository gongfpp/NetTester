#pragma once

#include <QObject>

class CTCP : public QObject
{
	Q_OBJECT

public:
	CTCP(QObject *parent);
	~CTCP();
};
