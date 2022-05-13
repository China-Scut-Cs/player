#ifndef FFMPEG_H
#define FFMPEG_H

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
    #include <libswresample/swresample.h>
    #include <SDL2/SDL.h>
    #include <stdio.h>
}

#endif // FFMPEG_H
