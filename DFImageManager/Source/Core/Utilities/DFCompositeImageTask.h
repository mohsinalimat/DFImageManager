// The MIT License (MIT)
//
// Copyright (c) 2015 Alexander Grebenyuk (github.com/kean).
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "DFImageManagerDefines.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class DFImageRequest;
@class DFImageTask;
@protocol DFImageManaging;

NS_ASSUME_NONNULL_BEGIN

@class DFCompositeImageTask;

typedef void (^DFCompositeImageTaskImageHandler)(UIImage *__nullable image, NSDictionary *info, DFImageRequest *request, DFCompositeImageTask *task);

/*! The DFCompositeImageTask manages execution of one or many image tasks and provides a single completion block that gets called multiple times. All requests are executed concurrently.
 @note By default, DFImageFetchTask does not call its completion handler for each of the completed requests. It treats the array of the requests as if the last request was the final image that you would want to display, while the others were the placeholders. The completion handler is guaranteed to be called at least once, even if all of the requests have failed. It also automatically cancels obsolete requests.
 @warning This class is not thread safe and is designed to be used on the main thread.
 */
@interface DFCompositeImageTask : NSObject

/*! Initializes composite task with an array of image tasks and a completion handler. After you create the task, you must start it by calling start method.
 @param tasks Array of image tasks. Must contain at least one task.
 @param imageHandler Gets called each time the image is fetched. Doesn't get called for obsolete requests.
 */
- (instancetype)initWithImageTasks:(NSArray /* DFImageTask */ *)tasks imageHandler:(nullable DFCompositeImageTaskImageHandler)imageHandler NS_DESIGNATED_INITIALIZER;

/*! Creates and starts task with an array of image requests.
 @param requests Array of requests. Must contain at least one request.
 @param imageHandler Gets called each time the image is fetched. Doesn't get called for obsolete requests.
 */
+ (DFCompositeImageTask *)compositeImageTaskWithRequests:(NSArray *)requests imageHandler:(DFCompositeImageTaskImageHandler)imageHandler;

/*! Image handler. Gets called each time the image is fetched. Doesn't get called for obsolete requests.
 */
@property (nullable, nonatomic, copy) void (^imageHandler)(UIImage *__nullable image, NSDictionary *info, DFImageRequest *request, DFCompositeImageTask *task);

/*! Set to YES to enable special handling of obsolete requests. Default value is YES. For more info see class notes.
 */
@property (nonatomic) BOOL allowsObsoleteRequests;

/*! Array of image tasks that the receiver was initialized with.
 */
@property (nonatomic, copy, readonly) NSArray /* DFImageTask */ *imageTasks;

/*! Returns image requests from the image tasks that the receiver was initialized with.
 */
@property (nonatomic, copy, readonly) NSArray /* DFImageRequest */ *imageRequests;

/*! Returns YES if all the requests have completed.
 */
@property (nonatomic, readonly) BOOL isFinished;

/*! Resumes the task.
 */
- (void)resume;

/*! Cancels all the requests registered with the receiver.
 */
- (void)cancel;

/*! Sets the priority for all the requests registered with a receiver.
 */
- (void)setPriority:(DFImageRequestPriority)priority;

@end

NS_ASSUME_NONNULL_END
