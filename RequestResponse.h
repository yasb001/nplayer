#ifndef REQUESTRESPONSE_H
#define REQUESTRESPONSE_H

#include <QObject>

#include "IInformationResource.h"

class RequestResponse
{
	public:
		RequestResponse();
		RequestResponse(const QString &templateName, QObject *dataContext);
		~RequestResponse();

		QString templateName() const;
		QObject *dataContext() const;

		bool isValid() const;

	private:
		QString m_templateName;
		QObject *m_dataContext;
};

#endif // REQUESTRESPONSE_H