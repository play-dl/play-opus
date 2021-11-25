// async function main() {
//     /* using 20 to make the progress bar length 20 charactes, multiplying by 5 below to arrive to 100 */
  
//     for (let i = 0; i <= 20; i++) {
//       const dots = ".".repeat(i)
//       const left = 20 - i
//       const empty = " ".repeat(left)
  
//       /* need to use  `process.stdout.write` becuase console.log print a newline character */
//       /* \r clear the current line and then print the other characters making it looks like it refresh*/
//       process.stdout.write('\x1B[?25l')
//       process.stdout.write(`\r[${dots}${empty}] ${i * 5}%`)
//       await wait(80)
//     }
//     process.stdout.write('\x1B[?25h')
//     console.log(process.stdout.columns)
//   }
  
//   main()
  
//   function wait(ms : number) {
//     return new Promise(res => setTimeout(res, ms))
//   }

// import { spawn } from 'child_process'

// const x = spawn('node-gyp', ['build'], { shell : true })

// let count = 0

// x.stdout.on('data', (x) => console.log(++count))

// x.stderr.on('data', (y) => console.log(y.toString('utf-8')))

// x.on('error', (s) => console.log('ERROR :' + s))

// Configure = 12
// Build = 168
// Creating new dir = 1
// Moving = 1

import { ProgressBar } from './progressbar'

const x = new ProgressBar('Title' , ['T 1', 'T 22222222'], 10)
x.cursor.unhide()