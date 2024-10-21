//
//  iOSAssetManager.cpp
//  MapCanvas
//
//  Created by Adrian Niebla on 20/10/24.
//

#include "iOSAssetManager.h"
#import <Foundation/Foundation.h>

iOSAssetStorage::iOSAssetStorage() : AssetStorage() {
    auto test = false;
}

iOSAssetStorage::~iOSAssetStorage() {
    auto test = false;
}

NSBundle* getBundle() {
    NSArray<NSBundle*>* allBundles = [NSBundle allFrameworks];
    for (NSBundle* bundle in allBundles) {
        NSString* identifier = [bundle bundleIdentifier];
        if ([identifier containsString:@"com.adrian.MapCanvas"]) {
            return bundle;
        }
    }
    return [NSBundle mainBundle];
}

std::string iOSAssetStorage::readFile(const std::string &filename) {
    NSBundle *b = getBundle();
    NSString *pathNSString = [NSString stringWithCString: filename.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString *fullPath = [NSString pathWithComponents:@[[b resourcePath], pathNSString]];
    NSError *error = nil;
    NSString *contents = [NSString stringWithContentsOfFile:fullPath encoding:NSUTF8StringEncoding error:&error];
    return std::string([contents UTF8String]);
}
