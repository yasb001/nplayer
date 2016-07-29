#include "NowPlayingRequest.h"
#include "RequestInvocationContext.h"
#include "RequestHandler.h"
#include "IOutputHandler.h"

#include "entities/RecentTracks.h"
#include "entities/NotPlaying.h"

#include "components/ITemplateComponent.h"

NowPlayingRequest::NowPlayingRequest()
{
	qRegisterMetaType<RecentTracks *>();
}

QString NowPlayingRequest::trigger() const
{
	return ".np";
}

RequestResponse NowPlayingRequest::invoke(const QStringList &arguments, const QString &who, const RequestInvocationContext &context)
{
	Q_UNUSED(arguments);

	InformationResourceRepository &repository = context.informationResourceRepository();
	IdGenerator &idGenerator = context.idGenerator();

	Track *nowPlaying = getNowPlaying(who, repository, idGenerator);

	if (nowPlaying)
	{
		return RequestResponse("templates/NowPlaying.qml", nowPlaying);
	}

	NotPlaying *notPlaying = new NotPlaying();
	notPlaying->setUser(who);

	return RequestResponse("templates/NotPlaying.qml", notPlaying);
}

Track *NowPlayingRequest::getNowPlaying(const QString &user, InformationResourceRepository &repository, IdGenerator &idGenerator) const
{
	const RequestHandler requestHandler(repository, idGenerator);
	const RecentTracks *recentTracks = requestHandler
		.get<RecentTracks>("user.getRecentTracks"
			, as::limit = 1
			, as::user = user
		);

	if (recentTracks)
	{
		for (Track *track : recentTracks->tracks())
		{
			if (track->nowPlaying())
			{
				return track;
			}
		}
	}

	return nullptr;
}
