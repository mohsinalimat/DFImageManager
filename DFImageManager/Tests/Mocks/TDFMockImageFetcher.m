//
//  TDFImageFetcher.m
//  DFImageManager
//
//  Created by Alexander Grebenyuk on 2/28/15.
//  Copyright (c) 2015 Alexander Grebenyuk. All rights reserved.
//

#import "TDFMockFetchOperation.h"
#import "TDFMockImageFetcher.h"
#import "TDFMockResource.h"
#import "TDFTesting.h"


NSString *TDFMockImageFetcherDidStartOperationNotification = @"TDFMockImageFetcherDidStartOperationNotification";
NSString *TDFMockImageFetcherRequestKey = @"TDFMockImageFetcherRequestKey";
NSString *TDFMockImageFetcherOperationKey = @"TDFMockImageFetcherOperationKey";


@implementation TDFMockImageFetcher

- (instancetype)init {
    if (self = [super init]) {
        _queue = [NSOperationQueue new];
        _image = [TDFTesting testImage];
    }
    return self;
}

- (BOOL)canHandleRequest:(DFImageRequest *)request {
    return [request.resource isKindOfClass:[TDFMockResource class]];
}

- (BOOL)isRequestFetchEquivalent:(DFImageRequest *)request1 toRequest:(DFImageRequest *)request2 {
    return [request1.resource isEqual:request2.resource];
}

- (BOOL)isRequestCacheEquivalent:(DFImageRequest *)request1 toRequest:(DFImageRequest *)request2 {
    return [request1.resource isEqual:request2.resource];
}

- (nonnull NSOperation *)startOperationWithRequest:(nonnull DFImageRequest *)request progressHandler:(nullable DFImageFetchingProgressHandler)progressHandler completion:(nullable DFImageFetchingCompletionHandler)completion {
    _createdOperationCount++;
    TDFMockFetchOperation *operation = [TDFMockFetchOperation blockOperationWithBlock:^{
        dispatch_async(dispatch_get_main_queue(), ^{
            if (progressHandler) {
                progressHandler(50, 100);
            }
        });
        dispatch_async(dispatch_get_main_queue(), ^{
            if (progressHandler) {
                progressHandler(100, 100);
            }
        });
        dispatch_async(dispatch_get_main_queue(), ^{
            if (completion) {
                completion(self.image, self.info, self.error);
            }
        });
    }];
    [_queue addOperation:operation];
    [[NSNotificationCenter defaultCenter] postNotificationName:TDFMockImageFetcherDidStartOperationNotification object:self userInfo:@{ TDFMockImageFetcherRequestKey : request, TDFMockImageFetcherOperationKey : operation }];
    return operation;
}

@end
