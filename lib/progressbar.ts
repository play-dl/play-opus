export class ProgressBar {
    done : number
    tasks : string[]
    title : string
    timer : NodeJS.Timer
    process : NodeJS.WriteStream

    constructor( title : string, tasks : string[] ){
        this.title = title
        this.done = 0
        this.tasks = tasks
        this.process = process.stdout
        this.timer = setInterval(() => this.update(), 1000)
    }

    update(){
        this.process.cursorTo(0, 2)
        const count = Math.floor((this.done/100) * 30)
        const dots = ".".repeat(count)
        const empty = " ".repeat(30 - count)
        const bar = `[${dots}${empty}] ${this.done}%`
        this.write('\r' + bar + '\n')
    }

    init(){
        process.once('exit', () => this.clear())
        process.once('SIGINT', () => this.clear())
        this.clear()
        this.process.cursorTo(0, 0)
        this.process.write('\x1B[?25l')
        this.write(this.title + '\n')
        const empty = " ".repeat(30)
        this.write(`\r[${empty}] ${this.done}%` + '\n')
        for (const task of this.tasks){
            const space = " ".repeat(40 - task.length)
            this.write(`\r${task}${space}Queued\n`)
        }
    }

    task_complete(num : number){
        if(num > this.tasks.length) return;
        this.process.cursorTo(0, num + 2)
        const space = " ".repeat(40 - this.tasks[num - 1].length)
        this.write(`\r${this.tasks[num - 1]}${space}Completed\n`)
    }

    task_running(num : number){
        if(num > this.tasks.length) return;
        this.process.cursorTo(0, num + 2)
        const space = " ".repeat(40 - this.tasks[num - 1].length)
        this.write(`\r${this.tasks[num - 1]}${space}Running\n`)
    }

    task_error(num : number, error : string){
        if(num > this.tasks.length) return;
        this.process.cursorTo(0, num + 2)
        const space = " ".repeat(40 - this.tasks[num - 1].length)
        this.write(`\r${this.tasks[num - 1]}${space}Error\n`)
        this.process.cursorTo(0, this.tasks.length + 3)
        this.write('\r Error : ' + error)
    }

    add(num : number){
        if(num <= 0) num = 1
        if(num > 100 || (this.done + num) > 100) this.done = 100
        else this.done += num
    }

    write(str : string){
        const empty = " ".repeat(this.process.columns)
        this.process.write(empty)
        this.process.write(str)
    }

    clear(){
        var lines = process.stdout.rows;
        for(var i = 0; i < lines; i++) {
            console.log('\n')
        }
    }
}