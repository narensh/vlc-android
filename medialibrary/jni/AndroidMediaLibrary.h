#ifndef ANDROIDMEDIALIBRARY_H
#define ANDROIDMEDIALIBRARY_H

#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>
#include <jni.h>
#include <pthread.h>

#include "AndroidDeviceLister.h"
#include "utils.h"


#include <medialibrary/IAlbum.h>
#include <medialibrary/IArtist.h>
#include <medialibrary/IGenre.h>
#include <medialibrary/IPlaylist.h>
#include <medialibrary/Types.h>
#include <medialibrary/IDeviceLister.h>
#include <medialibrary/IMedia.h>
#include <medialibrary/IMediaLibrary.h>

class AndroidMediaLibrary : public medialibrary::IMediaLibraryCb
{
public:
    AndroidMediaLibrary(JavaVM *vm, fields *ref_fields, jobject thiz);
    ~AndroidMediaLibrary();

    void initML(const std::string& dbPath, const std::string& thumbsPath);
    void addDevice(std::string uuid, std::string path, bool removable);
    bool removeDevice(std::string uuid);
    void banFolder(const std::string& path);
    void discover(const std::string&);
    bool isWorking();
    void setMediaUpdatedCbFlag(int flags);
    void setMediaAddedCbFlag(int flags);
    void pauseBackgroundOperations();
    void resumeBackgroundOperations();
    void reload();
    void reload( const std::string& entryPoint );
    bool increasePlayCount(int64_t mediaId);
    bool updateProgress(int64_t mediaId, int64_t time);
    std::vector<medialibrary::MediaPtr> lastMediaPlayed();
    medialibrary::SearchAggregate search(const std::string& query);
    medialibrary::MediaPtr media(long id);
    medialibrary::MediaPtr media(const std::string& mrl);
    std::vector<medialibrary::MediaPtr> videoFiles( medialibrary::SortingCriteria sort = medialibrary::SortingCriteria::Default, bool desc = false );
    std::vector<medialibrary::MediaPtr> audioFiles( medialibrary::SortingCriteria sort = medialibrary::SortingCriteria::Default, bool desc = false );
    std::vector<medialibrary::AlbumPtr> albums();
    medialibrary::AlbumPtr album(int64_t albumId);
    std::vector<medialibrary::ArtistPtr> artists();
    medialibrary::ArtistPtr artist(int64_t artistId);
    std::vector<medialibrary::GenrePtr> genres();
    medialibrary::GenrePtr genre(int64_t genreId);
    std::vector<medialibrary::PlaylistPtr> playlists();
    medialibrary::PlaylistPtr playlist( int64_t playlistId );
    medialibrary::PlaylistPtr PlaylistCreate( const std::string &name );
    std::vector<medialibrary::MediaPtr> tracksFromAlbum( int64_t albumId );
    std::vector<medialibrary::MediaPtr> mediaFromArtist( int64_t artistId );
    std::vector<medialibrary::AlbumPtr> albumsFromArtist( int64_t artistId );
    std::vector<medialibrary::MediaPtr> mediaFromGenre( int64_t genreId );
    std::vector<medialibrary::AlbumPtr> albumsFromGenre( int64_t genreId );
    std::vector<medialibrary::ArtistPtr> artistsFromGenre( int64_t genreId );
    std::vector<medialibrary::MediaPtr> mediaFromPlaylist( int64_t playlistId );
    bool playlistAppend(int64_t playlistId, int64_t mediaId);
    bool playlistAdd(int64_t playlistId, int64_t mediaId, unsigned int position);
    bool playlistMove(int64_t playlistId, int64_t mediaId, unsigned int position);
    bool playlistRemove(int64_t playlistId, int64_t mediaId);
    bool PlaylistDelete( int64_t playlistId );


    void onMediaAdded( std::vector<medialibrary::MediaPtr> media );
    void onMediaUpdated( std::vector<medialibrary::MediaPtr> media ) ;
    void onMediaDeleted( std::vector<int64_t> ids ) ;

    void onArtistsAdded( std::vector<medialibrary::ArtistPtr> artists ) ;
    void onArtistsModified( std::vector<medialibrary::ArtistPtr> artist );
    void onArtistsDeleted( std::vector<int64_t> ids );

    void onAlbumsAdded( std::vector<medialibrary::AlbumPtr> albums );
    void onAlbumsModified( std::vector<medialibrary::AlbumPtr> albums );
    void onAlbumsDeleted( std::vector<int64_t> ids );
    void onTracksAdded( std::vector<medialibrary::AlbumTrackPtr> tracks );
    void onTracksDeleted( std::vector<int64_t> trackIds );
    void onDiscoveryStarted( const std::string& entryPoint );
    void onDiscoveryProgress( const std::string& entryPoint );
    void onDiscoveryCompleted( const std::string& entryPoint );
    void onParsingStatsUpdated( uint32_t percent);

private:
    void jni_detach_thread(void *data);
    jobject getWeakReference(JNIEnv *env);
    JNIEnv *getEnv();
    void detachCurrentThread();

    pthread_once_t key_once = PTHREAD_ONCE_INIT;
    jweak weak_thiz, weak_compat;
    fields *p_fields;
    medialibrary::IMediaLibrary* p_ml;
    std::shared_ptr<AndroidDeviceLister> p_lister;
    medialibrary::IDeviceListerCb* p_DeviceListerCb;
    std::string mainStorage = "";
    bool discoveryEnded = false;
    uint32_t m_nbDiscovery = 0, m_progress = 0, m_mediaAddedType = 0, m_mediaUpdatedType = 0;
};
#endif // ANDROIDMEDIALIBRARY_H
