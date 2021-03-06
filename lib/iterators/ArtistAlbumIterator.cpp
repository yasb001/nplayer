#include "ArtistAlbumIterator.h"
#include "RequestHandler.h"

#include "entities/Artist.h"
#include "entities/AlbumsEnvelope.h"

QList<Album *> ArtistAlbumIterator::fetchMore(Artist *resource, InformationResourceRepository &informationResourceRepository, IdGenerator &idGenerator, int page)
{
	RequestHandler requestHandler(informationResourceRepository, idGenerator);
	AlbumsEnvelope *envelope = requestHandler
		.get<AlbumsEnvelope>("artist.getTopAlbums"
			, as::artist = resource->name()
			, as::limit = 10
			, as::page = page
		);

	if (envelope)
	{
		return envelope->albums();
	}

	return QList<Album *>();
}
