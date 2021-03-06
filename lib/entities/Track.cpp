#include "Track.h"
#include "InformationResourceRepository.h"
#include "IdGenerator.h"
#include "QStringEx.h"

#include "iterators/TrackAlbumIterator.h"
#include "iterators/TrackArtistIterator.h"
#include "iterators/TrackTrackIterator.h"

#include "providers/LastFmUrlProvider.h"
#include "providers/TrackSpotifyUrlProvider.h"
#include "providers/YouTubeUrlProvider.h"

Track::Track(InformationResourceRepository &repository, IdGenerator &idGenerator)
	: m_requestHandler(repository, idGenerator)
	, m_repository(repository)
	, m_nowPlaying(false)
	, m_fetched(false)
	, m_userLoved(false)
	, m_id(idGenerator)
	, m_userPlayCount(0)
{
	m_repository.add(this);

	registerIterator<TrackAlbumIterator>();
	registerIterator<TrackArtistIterator>();
	registerIterator<TrackTrackIterator>();

	registerUrlProvider<LastFmUrlProvider>();
	registerUrlProvider<TrackSpotifyUrlProvider>();
	registerUrlProvider<YouTubeUrlProvider>();
}

Track::~Track()
{
	m_repository.remove(this);
}

QString Track::id() const
{
	return m_id;
}

QString Track::key() const
{
	return QStringEx::format("%1%2", m_artist, m_name);
}

bool Track::nowPlaying() const
{
	return m_nowPlaying;
}

void Track::setNowPlaying(bool nowPlaying)
{
	m_nowPlaying = nowPlaying;
}

bool Track::userLoved()
{
	return property(&Track::m_userLoved);
}

void Track::setUserLoved(bool userLoved)
{
	m_userLoved = userLoved;
}

QString Track::artist() const
{
	return m_artist;
}

void Track::setArtist(const QString &artist)
{
	m_artist = artist;
}

QString Track::name() const
{
	return m_name;
}

void Track::setName(const QString &name)
{
	m_name = name;
}

QString Track::album()
{
	return property(&Track::m_album);
}

void Track::setAlbum(const QString &album)
{
	m_album = album;
}

QString Track::user() const
{
	return m_user;
}

void Track::setUser(const QString &user)
{
	m_user = user;
}

QString Track::nick() const
{
	return m_nick;
}

void Track::setNick(const QString &nick)
{
	m_nick = nick;
}

QString Track::url() const
{
	return m_url;
}

void Track::setUrl(const QString &url)
{
	m_url = url;
}

QDateTime Track::date() const
{
	return m_date;
}

void Track::setDate(const QDateTime &date)
{
	m_date = date;
}

Array<QString> Track::tags()
{
	return property(&Track::m_tags);
}

void Track::setTags(const Array<QString> &tags)
{
	m_tags = tags;
}

int Track::userPlayCount()
{
	return property(&Track::m_userPlayCount);
}

void Track::setUserPlayCount(int userPlayCount)
{
	m_userPlayCount = userPlayCount;
}

QString Track::formattedTitle() const
{
	return QStringEx::format("%1 - %2", m_artist, m_name);
}

void Track::fetchExtendedInfo()
{
	m_requestHandler
		.get(this, "track.getInfo"
			, as::artist = m_artist
			, as::track = m_name
			, as::username = m_user
		);
}
