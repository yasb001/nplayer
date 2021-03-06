#include "ArtistsEnvelope.h"

Array<Artist *> ArtistsEnvelope::artists() const
{
	return m_artists;
}

void ArtistsEnvelope::setArtists(const Array<Artist *> &artists)
{
	m_artists = artists;
}

bool ArtistsEnvelope::isValid() const
{
	return !m_artists.isEmpty();
}
