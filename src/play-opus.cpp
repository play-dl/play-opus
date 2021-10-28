#include "play-opus.h"
#include "string"

std::string errorHandler(int error)
{
    switch (error)
    {
    case OPUS_BAD_ARG:
        return "One or more invalid/out of range arguments.";
    case OPUS_BUFFER_TOO_SMALL:
        return "Not enough bytes allocated in the buffer.";
    case OPUS_INTERNAL_ERROR:
        return "An internal error was detected.";
    case OPUS_INVALID_PACKET:
        return "The compressed data passed is corrupted.";
    case OPUS_UNIMPLEMENTED:
        return "Invalid/unsupported request number.";
    case OPUS_INVALID_STATE:
        return "An encoder or decoder structure is invalid or already freed.";
    case OPUS_ALLOC_FAIL:
        return "Memory allocation has failed.";
    default:
        return "Unknown OPUS error.";
    };
};

void deleteOpus(const CallbackInfo& args)
{   
    Env env = args.Env();
    if(args.Length() != 1){
        Error::New(env, "encoder object is required as a argument.").ThrowAsJavaScriptException();
    }

    OpusHandler o1 = args[0].As<OpusHandler>();

    if (o1.encoder)
        opus_encoder_destroy(o1.encoder);
    if (o1.decoder)
        opus_decoder_destroy(o1.decoder);
    delete &o1;
}

OpusHandler::OpusHandler(const CallbackInfo& args){
    this->encoder = nullptr;
    this->decoder = nullptr;
    this->rate = args[0].ToNumber().Int32Value();
	this->channels = args[1].ToNumber().Int32Value();
    this->application = OPUS_APPLICATION_AUDIO;
};

Object Initiate(Env env, Object exports){
    HandleScope::HandleScope(env);

    Function func = DefineClass(env, "OpusHandler", {
        InstanceMethod("encode", &OpusHandler::encode),
		InstanceMethod("decode", &OpusHandler::decode),
		InstanceMethod("encode_ctl", &OpusHandler::encode_ctl),
		InstanceMethod("decode_ctl", &OpusHandler::decode_ctl),
		InstanceMethod("set_bitrate", &OpusHandler::set_bitrate),
		InstanceMethod("get_bitrate", &OpusHandler::get_bitrate),
    });

    exports.Set("OpusHandler", func);
    exports.Set("delete", Function::New(env, deleteOpus));
    return exports;
}

int EncoderCreate(OpusHandler* o1){
    if(o1->encoder) return OPUS_OK;
    else {
        int error;
        o1->encoder = opus_encoder_create(o1->rate, o1->channels, o1->application, &error);
        return error;
    }
}

int DecoderCreate(OpusHandler* o1){
    if(o1->decoder) return OPUS_OK;
    else {
        int error;
        o1->decoder = opus_decoder_create(o1->rate, o1->channels, &error);
        return error;
    }
}

Value OpusHandler::encode(const CallbackInfo& args){
    Env env = args.Env();
    
    int error = EncoderCreate(this);
    if(error != OPUS_OK){
        Error::New(env, errorHandler(error) + " [ Opus Handler -- Encoder ]").ThrowAsJavaScriptException();
    }

    if (!args[0].IsBuffer()) {
		TypeError::New(env, "Given argument is not a buffer. [ Opus Handler -- Encode ]").ThrowAsJavaScriptException();
	}

    Buffer<char> buf = args[0].As<Buffer<char>>();
    char* buf_data = buf.Data();
    opus_int16* data = reinterpret_cast<opus_int16*>(buf_data);
    int frameSize = buf.Length() / 2 / this->channels;

    unsigned char output[MAX_PACKET_SIZE];

    int finallength = opus_encode(this->encoder, data, frameSize, &output[0], MAX_PACKET_SIZE);

    if(finallength < 0){
        Error::New(env, errorHandler(finallength) + " [ Opus Handler -- Encode ]").ThrowAsJavaScriptException();
    }

    Buffer<char> result = Buffer<char>::Copy(env, reinterpret_cast<char*>(output), finallength);

    if(!result.IsEmpty()) return result;
}

Value OpusHandler::decode(const CallbackInfo& args){
    Env env = args.Env();

    if (!args[0].IsBuffer()) {
		TypeError::New(env, "Given argument is not a buffer. [ Opus Handler -- Encode ]").ThrowAsJavaScriptException();
	}

    Buffer<unsigned char> buf = args[0].As<Buffer<unsigned char>>();
    unsigned char* opusData = buf.Data();
    size_t opusLength = buf.Length();

    int error = DecoderCreate(this);

    if(error != OPUS_OK){
        Error::New(env, errorHandler(error) + " [ Opus Handler -- Decoder ]").ThrowAsJavaScriptException();
    }

    opus_int16* pcmout;
    int decodedCount = opus_decode(this->decoder, opusData, opusLength, &pcmout[0], MAX_FRAME_SIZE, 0);

    if(decodedCount < 0){
        Error::New(env, errorHandler(decodedCount) + " [ Opus Handler -- Decode ]").ThrowAsJavaScriptException();
    }

    int decodedLength = decodedCount * 2 * this->channels;

    Buffer<char> result = Buffer<char>::Copy(env, reinterpret_cast<char*>(pcmout), decodedLength);

    if(!result.IsEmpty()) return result;
}

void OpusHandler::encode_ctl(const CallbackInfo& args){
    Env env = args.Env();

    if(args.Length() != 2){
        Error::New(env, "Wrong no of arguements provided.").ThrowAsJavaScriptException();
    }

    int ctl = args[0].ToNumber().Int32Value();
    int value = args[1].ToNumber().Int32Value();

    int error = EncoderCreate(this);
    if(error != OPUS_OK){
        Error::New(env, errorHandler(error) + " [ Opus Handler -- Encoder ]").ThrowAsJavaScriptException();
    }

    if(opus_encoder_ctl(this->encoder, ctl, value) != OPUS_OK){
        TypeError::New(env, "Invalid ctl / value.").ThrowAsJavaScriptException();
    }
}

void OpusHandler::decode_ctl(const CallbackInfo& args){
    Env env = args.Env();

    if(args.Length() != 2){
        TypeError::New(env, "Wrong no of arguements provided.").ThrowAsJavaScriptException();
    }

    int ctl = args[0].ToNumber().Int32Value();
    int value = args[1].ToNumber().Int32Value();

    int error = DecoderCreate(this);

    if(error != OPUS_OK){
        Error::New(env, errorHandler(error) + " [ Opus Handler -- Decoder ]").ThrowAsJavaScriptException();
    }

    if(opus_decoder_ctl(this->decoder, ctl, value) != OPUS_OK){
        TypeError::New(env, "Invalid ctl / value.").ThrowAsJavaScriptException();
    }
}

void OpusHandler::set_bitrate(const CallbackInfo& args){
    Env env = args.Env();

    if(args.Length() != 1){
        TypeError::New(env, "Wrong no of arguements provided.").ThrowAsJavaScriptException();
    }

    int bitrate = args[0].ToNumber().Int32Value();

    int error = EncoderCreate(this);

    if(error != OPUS_OK){
        Error::New(env, errorHandler(error) + " [ Opus Handler -- Encoder ]").ThrowAsJavaScriptException();
    }

    if(opus_encoder_ctl(this->encoder, OPUS_SET_BITRATE(bitrate)) != OPUS_OK){
        TypeError::New(env, "Invalid bitrate").ThrowAsJavaScriptException();
    }
}

Value OpusHandler::get_bitrate(const CallbackInfo& args){
    Env env = args.Env();

    int error = EncoderCreate(this);

    if(error != OPUS_OK){
        Error::New(env, errorHandler(error) + " [ Opus Handler -- Encoder ]").ThrowAsJavaScriptException();
    }

    opus_int32 bitrate;
    opus_encoder_ctl(this->encoder, OPUS_GET_BITRATE(&bitrate));
    return Number::New(env, bitrate);
}

NODE_API_MODULE(addon, Initiate);