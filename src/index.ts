import * as opus from '../build/Release/opus'

let x = new opus.OpusHandler(48000, 2)

console.log(x.encode.toString())