export class ProgressBar {
    cursor : Cursor;
    completed : string[];
    queue : string[];
    running : string;
    process : NodeJS.WriteStream;
    total : number;
    current : number;

    constructor( title : string, tasks : string[], size : number ){
        this.cursor = new Cursor(process.stdout)
        this.completed = []
        this.queue = tasks
        this.running = ''
        this.process = process.stdout
        this.total = size
        this.current = 0
        this.cursor.hide()
        this.process.write( "\r" + title + '\n' )
        this.process.write( "\r" + this.bar + '\n' )
        for (const task of this.queue){
            console.log(task)
            this.process.write( "\r" + task + " " + ".".repeat(20 - task.length) + "Queued" + '\n' )
        }
        this.cursor.up(this.queue.length)
    }

    get percentage(){
        return parseFloat((this.current/this.total).toFixed(2))
    }

    get spaces(){
        return " ".repeat(30 - Math.floor(this.percentage * 30))
    }

    get dots(){
        return ".".repeat(Math.floor(this.percentage * 30))
    }

    get bar(){
        return `[${this.dots}${this.spaces}]  ${this.percentage}%`
    }

    update(){
        this.process.write(`\r${this.bar}`)
    }
}


class Cursor{

    private process : NodeJS.WriteStream

    constructor( process : NodeJS.WriteStream ){
        this.process = process
    }

    unhide(){
        this.process.write('\x1B[?25h')
    }

    hide(){
        this.process.write('\x1B[?25l')
    }

    cursorTo( x: number, y?: number ){
        this.process.cursorTo(x , y)
    }

    erase(){
        this.process.write('\x1B[K')
    }

    save(){
        this.process.write('\x1B[s')
    }

    restore(){
        this.process.write('\x1B[u')
    }

    clear(){
        this.process.write('\x1B[2J')
    }

    up( num? : number ){
        if( !num || num <= 0) num = 1;
        this.process.write(`\x1B[${num}A`)
    }

    down( num? : number ){
        if( !num || num <= 0) num = 1;
        this.process.write(`\x1B[${num}B`)
    }

    forward( num? : number ){
        if( !num || num <= 0) num = 1;
        this.process.write(`\x1B[${num}C`)
    }

    backward( num? : number ){
        if( !num || num <= 0) num = 1;
        this.process.write(`\x1B[${num}D`)
    }
}