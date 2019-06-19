//
// Created by User on 11.06.2019.
//

#ifndef AECM_AEC_H
#define AECM_AEC_H

#include <jni.h>
    JNIEXPORT jlong Java_ru_theeasiestway_libaecm_AEC_nativeCreateAecmInstance(JNIEnv *env, jclass thiz);
    JNIEXPORT jint Java_ru_theeasiestway_libaecm_AEC_nativeFreeAecmInstance(JNIEnv *env, jclass thiz, jlong aecmHandler);
    JNIEXPORT jint Java_ru_theeasiestway_libaecm_AEC_nativeInitializeAecmInstance(JNIEnv *env, jclass thiz, jlong aecmHandler, jint sampFreq);
    JNIEXPORT jint Java_ru_theeasiestway_libaecm_AEC_nativeBufferFarend(JNIEnv *env, jclass thiz, jlong aecmHandler, jshortArray farend, jint nrOfSamples);
    JNIEXPORT jshortArray Java_ru_theeasiestway_libaecm_AEC_nativeAecmProcess(JNIEnv *env, jclass thiz, jlong aecmHandler, const jshortArray nearendNoisy, const jshortArray nearendClean, jshort nrOfSamples, jshort msInSndCardBuf);
    JNIEXPORT jint Java_ru_theeasiestway_libaecm_AEC_nativeSetConfig(JNIEnv *env, jclass thiz, jlong aecmHandler, jobject aecmConfig);
#endif //AECM_AEC_H