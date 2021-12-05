#pragma once

#include "Mailing.h"

#include "Includes.h"

void CurrencyThread(void* Param);

class Currency : public Mailing
{
	friend void CurrencyThread(void* Param);
public:
	Currency(int time = 25000);

	virtual void SendMailThread() override;
};

