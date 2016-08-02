#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QJsonSerializer>

#include "IInformationResource.h"
#include "RequestHandler.h"

class Artist;
class Track;
class InformationResourceRepository;
class IdGenerator;

template<class T>
using Callback = T (*)(Track &);

class Track : public InformationResource<Track>
{
	Q_OBJECT

	Q_PROPERTY(bool nowPlaying READ nowPlaying WRITE setNowPlaying)
	Q_PROPERTY(bool userLoved READ userLoved WRITE setUserLoved NOTIFY userLovedChanged)
	Q_PROPERTY(bool hasTags READ hasTags NOTIFY hasTagsChanged)

	Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
	Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)

	Q_PROPERTY(Array<QString> tags READ tags WRITE setTags NOTIFY tagsChanged)

	Q_PROPERTY(int userPlayCount READ userPlayCount WRITE setUserPlayCount NOTIFY userPlayCountChanged)

	public:
		Track(InformationResourceRepository &repository, IdGenerator &idGenerator);

		QString id() const override;

		bool nowPlaying() const;
		void setNowPlaying(bool nowPlaying);

		bool userLoved();
		void setUserLoved(bool userLoved);

		bool hasTags();

		QString artist() const;
		void setArtist(const QString &artist);

		QString name() const;
		void setName(const QString &name);

		QString album();
		void setAlbum(const QString &album);

		QString user() const;
		void setUser(const QString &user);

		Array<QString> tags();
		void setTags(const Array<QString> &tags);

		int userPlayCount();
		void setUserPlayCount(int userPlayCount);

		Q_INVOKABLE QString formattedTitle() const;

	private:
		void fetchExtendedInfo() override;

		RequestHandler m_requestHandler;

		bool m_nowPlaying;
		bool m_fetched;
		bool m_userLoved;

		QString m_artist;
		QString m_name;
		QString m_album;
		QString m_user;
		QString m_id;

		Array<QString> m_tags;

		int m_userPlayCount;

	signals:
		void userLovedChanged();
		void hasTagsChanged();
		void artistChanged();
		void nameChanged();
		void albumChanged();
		void userChanged();
		void tagsChanged();
		void userPlayCountChanged();
};

Q_DECLARE_METATYPE(Array<Track *>)

#endif // TRACK_H
