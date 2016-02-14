#include "FFmpegMedia.h"
#include "../../GlobalFuncs.h"
#include <math.h>

void FFmpegMedia::init() {
	vidStreamIdx = audStreamIdx = -1;
	_videoAvailable = _audioAvailable = false;
	fmtCtx = NULL;
	//rawBitmapBuffer = NULL;
	//rawBufferFrameNum = 0;
}

FFmpegMedia::FFmpegMedia() {
	init();
}
FFmpegMedia::FFmpegMedia(std::wstring filepath):FFmpegMedia() {

	if (filepath.length() < 1)return;
	int ret = 0;

	/* open file */
	ret = avformat_open_input(
				&fmtCtx,					//get format context
				WStr2Str(filepath).c_str(), //filepath(cast from wstring to char)
				NULL,						//nothing to specify about input format
				NULL);						//no options

	if (ret != 0)return;	//return when failed to open

	/* set stream info */
	ret = avformat_find_stream_info(fmtCtx, NULL);

	if (ret < 0)return;		//return when no stream found
	
	/* find most appropriate video stream */
	AVCodec *vidCodec,*audCodec;				//get codec info in this
	vidCodec = audCodec = NULL;
	vidStreamIdx = audStreamIdx = -1;

	vidStreamIdx = av_find_best_stream(
					fmtCtx,					//file info
					AVMEDIA_TYPE_VIDEO,		//media type we want
					-1,						//no stream specified
					-1,						//no related stream spiecified
					&vidCodec,				//get codec(decoder) info
					0);						//(no flags available)

	audStreamIdx = av_find_best_stream(		//same as video
					fmtCtx,					
					AVMEDIA_TYPE_AUDIO,		
					-1,					
					-1,						
					&audCodec,				
					0);

	if (vidStreamIdx >= 0 && vidCodec != NULL) { // has video stream
		/* init codec */
		vidCodecCtx = fmtCtx->streams[vidStreamIdx]->codec;
		_videoAvailable = (avcodec_open2(vidCodecCtx, vidCodec, NULL) >=0);
		
	}

	if (audStreamIdx >= 0 && audCodec != NULL) { 
		audCodecCtx = fmtCtx->streams[audStreamIdx]->codec;
		_audioAvailable = (avcodec_open2(audCodecCtx, audCodec, NULL) >= 0);
	}

end:
	return;
}

bool FFmpegMedia::isVideoAvailable() const {
	return _videoAvailable;
}

bool FFmpegMedia::isAudioAvailable() const {
	return _audioAvailable;
}

int FFmpegMedia::width() const {
	return isVideoAvailable()?vidCodecCtx->width:-1;
}

int FFmpegMedia::height() const {
	return isVideoAvailable() ? vidCodecCtx->height : -1;
}

double FFmpegMedia::duration() const {
	return max(
		isVideoAvailable() ? fmtCtx->streams[vidStreamIdx]->duration : -1,
		isAudioAvailable() ? fmtCtx->streams[audStreamIdx]->duration : -1
		);
}
/*
void FFmpegMedia::rawBufferAlloc(int size, int frameNum, uint8_t** buf) {
	buf = new uint8_t*[frameNum];
	rawBitmapBuffer[0] = (uint8_t*)malloc(frameNum*size);
	for (int i = 1; i < frameNum; i++) {
		rawBitmapBuffer[i] = ((uint8_t*)rawBitmapBuffer[i - 1]) + size;
	}
}

void FFmpegMedia::rawBufferFree(uint8_t** buf) {
	free(buf[0]);
}
*/