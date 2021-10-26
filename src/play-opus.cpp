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

void deleteOpus(OpusHandler *o1)
{
    if (o1->encoder)
        opus_encoder_destroy(o1->encoder);
    if (o1->decoder)
        opus_decoder_destroy(o1->decoder);
    delete o1;
}