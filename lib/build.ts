import fs from 'fs'

if(fs.existsSync('./play-opus')) fs.rmSync('./play-opus', { recursive : true })
fs.mkdirSync('./play-opus')
if(!fs.existsSync('./build/Release')) {
    console.log('\nNo Build Files found. Report to the developer.')
    process.exit(1)
}
const opusFiles = fs.readdirSync('./build/Release').filter((x) => x.startsWith('opus'))
opusFiles.forEach((file) => {
    fs.copyFileSync(`./build/Release/${file}`, `./play-opus/${file}`)
})
fs.copyFileSync('./src/opus.d.ts', './play-opus/opus.d.ts')
fs.rmSync('./build', { recursive : true })