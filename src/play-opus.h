#include "napi.h"
#include "../deps/opus/include/opus.h"

using namespace Napi;

#define FRAME_SIZE 960
#define MAX_FRAME_SIZE 6 * 960
#define MAX_PACKET_SIZE 3 * 1276
#define BITRATE 64000

class OpusHandler {
    private :
        OpusEncoder* encoder;
        OpusDecoder* decoder;
    public :
        friend void deleteOpus(OpusHandler* o1);
        OpusHandler(const CallbackInfo& args);
        Value encode(const CallbackInfo& args);
        Value decode(const CallbackInfo& args);
        void encode_ctl(const CallbackInfo& args);
        void decode_ctl(const CallbackInfo& args);
        void set_bitrate(const CallbackInfo& args);
        Value get_bitrate(const CallbackInfo& args);
};

void deleteOpus(OpusHandler o1);