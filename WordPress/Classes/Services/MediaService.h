#import <Foundation/Foundation.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <Photos/Photos.h>
#import "LocalCoreDataService.h"

@class Media;
@class Blog;

@interface MediaService : LocalCoreDataService

/**
 Create a media object using the url provided as the source of media.

 @param url a file url pointing to a file with the media data
 @param postObjectID the post object ID to associate the media
 @param thumbnailCallback a block that will be invoked when the thumbail for the media object is ready
 @param completion a block that will be invoked when the media is created, on success it will return a valid Media object, on failure it will return a nil Media and an error object with the details.
 */
- (void)createMediaWithURL:(NSURL *)url
           forPostObjectID:(NSManagedObjectID *)postObjectID
         thumbnailCallback:(void (^)(NSURL *thumbnailURL))thumbnailCallback
                completion:(void (^)(Media *media, NSError *error))completion;

/**
 Create a Media object using the asset as the source and making it a child of the post with postObjectId.
 
 @param asset
 @param postObjectID
 @param thumbnailCallback a block that will be invoked when the thumbail for the media object is ready
 @param completion a block that will be invoked when the media is created, on success it will return a valid Media object, on failure it will return a nil Media and an error object with the details.
 */
- (void)createMediaWithPHAsset:(PHAsset *)asset
             forPostObjectID:(NSManagedObjectID *)postObjectID
           thumbnailCallback:(void (^)(NSURL *thumbnailURL))thumbnailCallback
                  completion:(void (^)(Media *media, NSError *error))completion;

- (void)createMediaWithImage:(UIImage *)image
                 withMediaID:(NSString *)mediaID
             forPostObjectID:(NSManagedObjectID *)postObjectID
           thumbnailCallback:(void (^)(NSURL *thumbnailURL))thumbnailCallback
                  completion:(void (^)(Media *media, NSError *error))completion;

/**
 Get the Media object from the server using the blog and the mediaID as the identifier of the resource
 
 @param mediaID
 @param blog
 @param success a block that will be invoked when the media is retrieved
 @param failure a block that will be invoked if an error happens returnin the associated error object with the details.
 */
- (void)getMediaWithID:(NSNumber *)mediaID
                inBlog:(Blog *)blog
           withSuccess:(void (^)(Media *media))success
               failure:(void (^)(NSError *error))failure;

/**
 Uploads the Media object to the server.
 
 @param media object to upload to the server.
 @param progress a NSProgress that tracks the upload progress to the server.
 @param success a block that will be invoked when the media upload finished with success
 @param failure a block that will be invoked when there is upload error.
 */
- (void)uploadMedia:(Media *)media
           progress:(NSProgress **) progress
            success:(void (^)())success
            failure:(void (^)(NSError *error))failure;


/**
 Updates the media object details to the server. This method doesn't allow you to update media file,
 because that always stays static after the initial upload, it only allows to change details like,
 caption, alternative text, etc...

 @param media object to upload to the server.
 @success a block that will be invoked when the media upload finished with success
 @failure a block that will be invoked when there is upload error.
 */
- (void)updateMedia:(Media *)media
            success:(void (^)())success
            failure:(void (^)(NSError *error))failure;

/**
 Updates multiple media objects similar to -updateMedia:success:failure: but batches them
 together. The success handler is only called when all updates succeed. Failure is called
 if the entire process fails in some catostrophic way.
 
 @param mediaObjects An array of media objects to update
 @param success
 */
- (void)updateMultipleMedia:(NSArray<Media *> *)mediaObjects
             overallSuccess:(void (^)())overallSuccess
                    failure:(void (^)(NSError *error))failure;

/**
 Find the media object in the local database.
 
 @param mediaID
 @param blog
 
 @return the Media object with the mediaID that belongs to the blog. Nil if not found.
 */
- (Media *)findMediaWithID:(NSNumber *)mediaID inBlog:(Blog *)blog;

/**
 *  Obtains the  video url and poster image url for the video with the videoPressID
 *
 *  @param videoPressID ID of video in VideoPress
 *  @param blog         blog to use to access video references
 *  @param success      return block if videopress info is found
 *  @param failure      return block if not information found.
 */
- (void)getMediaURLFromVideoPressID:(NSString *)videoPressID
                             inBlog:(Blog *)blog
                            success:(void (^)(NSString *videoURL, NSString *posterURL))success
                            failure:(void (^)(NSError *error))failure;
/**
 * Sync all Media objects from the server to local database
 
 * @param blog
 * @param success a block that will be invoked when the sync succeeds
 * @param failure a block that will be invoked when the sync fails
 */
- (void)syncMediaLibraryForBlog:(Blog *)blog
                        success:(void (^)())success
                        failure:(void (^)(NSError *error))failure;

/**
 + Get a thumbnail image for a Media by downloading its image or using the local cache
 +
 + @param media
 + @param success a block that will be invoked when the media is retrieved
 + @failure a block that will be invoked if an error happens returnin the associated error object with the details.
 + */
- (void)thumbnailForMedia:(Media *)media
                     size:(CGSize)size
                  success:(void (^)(UIImage *image))success
                  failure:(void (^)(NSError *error))failure;
/**
 *  Get the number of items in a blog media library that are of a certain type.
 *
 *  @param blog from what blog to count the media items.
 *  @param mediaTypes set of media type values to be considered in the counting.
 */
- (NSInteger)getMediaLibraryCountForBlog:(Blog *)blog
                           forMediaTypes:(NSSet *)mediaTypes;

#pragma mark - Media cleanup

/**
 Returns the url for for the media cache directory

 @return an url for the media cache directory.
 */
+ (NSURL *)urlForMediaDirectory;

/**
 *  @brief      Removes all unused media files from the media directories
 *  
 *  @discussion This method looks for any media files that stored inside the media folder that aren't
 * linked to any valid media object and remove them. These files can show up because of the app being killed
 * while a media object was being created or when a CoreData migration fails and the database is recreated.
 */
+ (void)cleanUnusedMediaFilesFromMediaCacheFolder;

/**
 Cleans all files that have a remote copy from the media cache folder.
 */
+ (void)cleanMediaCacheFolder;

@end
