export class OpusHandler {
    constructor(rate : number, channel : number);
    encode(buffer : Buffer): Buffer;
    decode(buffer : Buffer): Buffer;
    encode_ctl(ctl : number, value : number): void;
    decode_ctl(ctl : number, value : number): void;
    set_bitrate(value : number): void;
    get_bitrate(): number;
    delete(): void;
}