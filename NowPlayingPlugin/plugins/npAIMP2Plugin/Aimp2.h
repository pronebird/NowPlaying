#define	AIMP2_RemoteClass L"AIMP2_RemoteInfo"
#define AIMP2_RemoteFileSize 2048
#define AIMP2_MainClassName L"TAIMP2Main"

#pragma pack(push)
#pragma pack(1) 
typedef struct {
    ULONG cbSizeOF;

    BOOL  Active;
	ULONG nBitRate;
    ULONG nChannels;
	ULONG nDuration;
	INT64 nFileSize ;
    ULONG nRating;
    ULONG nSampleRate;
    ULONG nTrackID;

    ULONG nAlbumLen;
    ULONG nArtistLen;
    ULONG nDateLen;
    ULONG nFileNameLen;
    ULONG nGenreLen;
    ULONG nTitleLen;
    
	PWCHAR sAlbum;
    PWCHAR sArtist;
    PWCHAR sDate;
	PWCHAR sFileName;
	PWCHAR sGenre;
    PWCHAR sTitle;

}TAIMP2FileInfo,*PAIMP2FileInfo;

#pragma pack(pop)