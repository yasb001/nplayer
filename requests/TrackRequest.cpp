#include "TrackRequest.h"
#include "RequestInvocationContext.h"
#include "InformationResourceRepository.h"

#include "entities/Track.h"

TrackRequest::TrackRequest()
	: m_regex("(?'artist'.+) \\- (?'title'.+)")
{

}

QString TrackRequest::trigger() const
{
	return ".track";
}

RequestResponse TrackRequest::invoke(const QStringList &arguments, const QString &who, const RequestInvocationContext &context)
{
	Q_UNUSED(who);

	const QString &id = arguments.join(QChar::Space);

	InformationResourceRepository &repository = context.informationResourceRepository();
	IdGenerator &idGenerator = context.idGenerator();

	IInformationResource *resource = repository.get(id);

	if (resource)
	{
		IIterator<Track> *iterator = resource->iterator<Track>();

		if (iterator)
		{
			Track *track = iterator->next(repository, idGenerator);

			if (track)
			{
				return RequestResponse("templates/Track.qml", track);
			}

			return RequestResponse("templates/NoMoreData.qml", nullptr);
		}
	}
	else
	{
		const QRegularExpressionMatch &match = m_regex.match(id);

		if (match.hasMatch())
		{
			RequestHandler requestHandler(repository, idGenerator);

			Track *track = requestHandler
				.get<Track>("track.getInfo"
					, as::artist = match.captured("artist")
					, as::track = match.captured("title")
					, as::username = who
				);

			if (track)
			{
				return RequestResponse("templates/Track.qml", track);
			}
		}
	}

	return RequestResponse();
}