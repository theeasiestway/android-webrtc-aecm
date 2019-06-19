#include <jni.h>
#include "AEC.h"
#include <stdlib.h> // for NULL operator
#include <assert.h>
#include "../lib/echo_control_mobile.h"
#include "../lib/echo_control_mobile.h"
#include "../log/debug_log.h"

const char* TAG = "AECM_LOG";

/**
 * This function is a wrapper which wraps the WebRtcAecm_Create function in WebRtc echo_control_mobile.c
 * Allocates the memory needed by the AECM. The memory needs to be initialized
 * separately using the WebRtcAecm_Init() function.
 *
 * Returns:
 *         -1: error
 *         other values: created AECM instance handler.
 *
 */
JNIEXPORT jlong JNICALL Java_ru_theeasiestway_libaecm_AEC_nativeCreateAecmInstance(JNIEnv *env, jclass thiz) {
	void *aecmInstHandler = NULL;
	if (WebRtcAecm_Create(&aecmInstHandler) == -1)
		return -1;
	else
		return ((long) aecmInstHandler); //returns the pointer which points to created AECM instance to JAVA layer.
}

/**
 * This is a wrapper wraps WebRtcAecm_Free function in echo_control_mobile.c
 * This function releases the memory allocated by WebRtcAecm_Create().
 *
 * Inputs:
 *         aecmHandler - handler of the AECM instance created by nativeCreateAecmInstance()
 *
 * Returns         0: OK
 *                 -1: error
 *
 */
JNIEXPORT jint JNICALL Java_ru_theeasiestway_libaecm_AEC_nativeFreeAecmInstance(JNIEnv *env, jclass thiz, jlong aecmHandler) {
	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;
	int ret = WebRtcAecm_Free(aecmInst);
	aecmInst = NULL;
	return ret;
}

/**
 * This wrapper wraps the WebRtcAecm_Init() function in WebRtc echo_control_mobile.c
 * Initializes an AECM instance.
 *
 * Inputs:
 *            aecmHandler     - Handler of AECM instance
 *            sampFreq        - Sampling frequency of data
 *
 * Return:          0: OK
 *                  -1: error
 *
 */
JNIEXPORT jint JNICALL Java_ru_theeasiestway_libaecm_AEC_nativeInitializeAecmInstance(JNIEnv *env, jclass thiz, jlong aecmHandler, jint sampFreq) {
	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;
	return WebRtcAecm_Init(aecmInst, sampFreq);
}

/**
 * This wrapper wraps the WebRtcAecm_BufferFarend function in echo_control_mobile.c
 * Inserts an 80 or 160 sample block of data into the farend buffer.
 *
 * Inputs:
 *       aecmHandler    - Handler to the AECM instance
 *       farend               - In buffer containing one frame of farend signal for L band
 *       nrOfSamples    - Number of samples in farend buffer
 *
 * Return:     0: OK
 *             -1: error
 *
 */
JNIEXPORT jint JNICALL Java_ru_theeasiestway_libaecm_AEC_nativeBufferFarend(JNIEnv *env, jclass thiz, jlong aecmHandler, jshortArray farend, jint nrOfSamples) {

    LOGD(TAG, "nativeBufferFarend() nrOfSamples: %d", nrOfSamples);

    void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL) {
        LOGD(TAG, "nativeBufferFarend() error: aecmInst == NULL");
        return -1;
    }

	int ret = -1;
	if (farend != NULL) {
		short *arrFarend = (*env)->GetShortArrayElements(env, farend, NULL);
		ret = WebRtcAecm_BufferFarend(aecmInst, arrFarend, nrOfSamples);
		(*env)->ReleaseShortArrayElements(env, farend, arrFarend, 0);
	}
	return ret;
}

/**
 * This wrapper wraps the WebRtcAecm_Process in echo_control_mobile.c
 * Runs the AECM on an 80 or 160 sample blocks of data.
 *
 * Inputs:
 *         aecmHandler           - Handler to the AECM handler
 *         nearendNoisy          - In buffer containing one frame of reference nearend+echo signal.
 *                                             If noise reduction is active, provide the noisy signal here.
 *         nearendClean          -  In buffer containing one frame of nearend+echo signal.
 *                                             If noise reduction is active, provide the clean signal here.
 *                                             Otherwise pass a NULL pointer.
 *         nrOfSamples           - Number of samples in nearend buffer
 *         msInSndCardBuf    	 - Delay estimate for sound card and system buffers
 * Outputs:
 *         out    - Out buffer, one frame of processed nearend.
 * Return:     0: OK
 *             -1: error
 *
 */

JNIEXPORT jshortArray JNICALL Java_ru_theeasiestway_libaecm_AEC_nativeAecmProcess(JNIEnv *env, jclass thiz, jlong aecmHandler, const jshortArray nearendNoisy, const jshortArray nearendClean, jshort nrOfSamples, jshort msInSndCardBuf) {

    LOGD(TAG, "nativeAecmProcess() nrOfSamples: %d; delay: %d", nrOfSamples, msInSndCardBuf);

	int16_t *arrNearendNoisy = NULL;
	int16_t *arrNearendClean = NULL;
	int16_t *arrOut = NULL;

	void *aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return NULL;

	int ret = -1;

	//nearendNoisy and out must not be NULL, otherwise process can not be run, return -1 for error.
	if (nearendNoisy == NULL)
		return NULL;

	//create out array
	jsize outSize = (*env)->GetArrayLength(env, nearendNoisy);
    jshortArray out = (*env)->NewShortArray(env, outSize);

    //get data from java side.
	arrNearendNoisy = (*env)->GetShortArrayElements(env, nearendNoisy, NULL);
	arrOut = (*env)->GetShortArrayElements(env, out, NULL);

	if (nearendClean != NULL)
		arrNearendClean = (*env)->GetShortArrayElements(env, nearendClean, NULL);

	ret = WebRtcAecm_Process(aecmInst, arrNearendNoisy, arrNearendClean, arrOut,
			nrOfSamples, msInSndCardBuf);

	if (ret != 0) {
	    LOGD(TAG, "nativeAecmProcess() error ret: %d", ret);
        return NULL;
	}

	//release and send the changes back to java side.
	(*env)->ReleaseShortArrayElements(env, nearendNoisy, arrNearendNoisy, 0);
	(*env)->ReleaseShortArrayElements(env, out, arrOut, 0);

	if (nearendClean != NULL)
		(*env)->ReleaseShortArrayElements(env, nearendClean, arrNearendClean, 0);

	return out;
}

/**
 * This wrapper wraps the WebRtcAecm_set_config function in echo_control_mobile.c
 * Enables the user to set certain parameters on-the-fly.
 *
 * Inputs:
 *        aecHandler - Handler to the AEC instance.
 *        aecConfig - the new configuration of AEC instance to set.
 * Outputs:
 *         NONE
 * Return:     0: OK
 *             -1: error
 *
 */
JNIEXPORT jint JNICALL Java_ru_theeasiestway_libaecm_AEC_nativeSetConfig(JNIEnv *env, jclass thiz, jlong aecmHandler, jobject aecmConfig) {

	void * aecmInst = (void *) aecmHandler;
	if (aecmInst == NULL)
		return -1;

	//get reference of AecmConfig class  from java side.
	jclass JavaAecmConfig = (*env)->GetObjectClass(env, aecmConfig);

	//assertion that class not be NULL
	assert(JavaAecmConfig != NULL);

	//get configuration field IDs from java side.
	jfieldID mAecmModeID = (*env)->GetFieldID(env, JavaAecmConfig, "mAecmMode", "S");
	jfieldID mCngModeID = (*env)->GetFieldID(env, JavaAecmConfig, "mCngMode", "S");

	//if any ID is NULL, return -1 for error.
	if (mAecmModeID == NULL || mCngModeID == NULL)
		return -1;

	//get values of fields
	short echoMode = (*env)->GetShortField(env, aecmConfig, mAecmModeID);
	short cngMode = (*env)->GetShortField(env, aecmConfig, mCngModeID);

	//set new configuration to AECM instance.
	AecmConfig config;
	config.echoMode = echoMode;
	config.cngMode = cngMode;

	return WebRtcAecm_set_config(aecmInst, config);
}
