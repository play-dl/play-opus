#include "napi.h"
#include "../deps/opus/include/opus.h"

using namespace Napi;

#define FRAME_SIZE 960
#define MAX_FRAME_SIZE 6 * 960
#define MAX_PACKET_SIZE 3 * 1276
#define BITRATE 64000

class OpusHandler : public ObjectWrap<OpusHandler> {
    private :
        OpusEncoder* encoder;

        OpusDecoder* decoder;

        opus_int32 rate;

        int channels;
        
        int application;

        
    public :
        OpusHandler(const CallbackInfo& args);

        static Object Init(Napi::Env env, Object exports);

        ~OpusHandler();

        int create_encoder();

        int create_decoder();

        friend void deleteOpus(const CallbackInfo& args);

        Napi::Value encode(const CallbackInfo& args);

        Napi::Value decode(const CallbackInfo& args);

        void encode_ctl(const CallbackInfo& args);

        void decode_ctl(const CallbackInfo& args);

        void set_bitrate(const CallbackInfo& args);

        Napi::Value get_bitrate(const CallbackInfo& args);

        void deletehandler(const CallbackInfo& args);
};