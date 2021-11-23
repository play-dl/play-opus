export class ProgressBar {
    task_completed : number
    running : boolean
    tasks : number
    current_task : string
    timer : NodeJS.Timer | null
    process : NodeJS.WriteStream
    current : number;
    total : number;
    constructor(title : string, num : number){
        this.tasks = num
        this.task_completed = 0
        this.process = process.stdout
        this.process.write('\x1B[?25l')
        this.process.write(title + '\n')
        this.timer = null
        this.current_task = ''
        this.current = 0
        this.total = 0
        this.running = false
    }  

    destroy(){
        clearInterval(this.timer as NodeJS.Timer)
        this.unhide()
        this.current_task = ''
        this.current = 0
        this.total = 0
        this.tasks = 0
        this.task_completed = 0
    }
    
    update(){
        if(this.running === false) return;
        const count = Math.floor((this.current/this.total) * 30)
        const space = " ".repeat(30 - count)
        const dots = ".".repeat(count)
        this.process.write(`\r` + this.current_task + `[${this.task_completed + 1}/${this.tasks}] : ` + `[${dots}${space}] ${((this.current/this.total) * 100).toFixed(1)}%`)
    }

    init(task : string, total : number){
        this.timer = setInterval(() => {this.update()}, 1000)
        this.running = true
        this.current_task = task
        this.total = total
        const count = Math.floor((this.current/this.total) * 30)
        const space = " ".repeat(30 - count)
        const dots = ".".repeat(count)
        this.process.write(`\r` + this.current_task + `[${this.task_completed + 1}/${this.tasks}] : ` + `[${dots}${space}] ${((this.current/this.total) * 100).toFixed(1)}%`)
    }

    complete(){
        if(this.running === false) return;
        this.running = false
        this.task_completed++
        const count = Math.floor((this.current/this.total) * 30)
        const space = " ".repeat(30 - count)
        const dots = ".".repeat(count)
        this.process.write(`\r` + this.current_task + `[${this.task_completed}/${this.tasks}] : ` + `[${dots}${space}] ${((this.current/this.total) * 100).toFixed(1)}% Finished`)
        this.destroy()
    }

    add(num? : number){
        if(!num || num <= 0 ) num = 1
        if((this.current + num) > this.total) this.current = this.total
        else this.current += num
        if(this.current === this.total) this.complete()
    }

    unhide(){
        this.process.write('\x1B[?25h')
    }
}