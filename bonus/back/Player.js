const fs = require('fs');
const { spawn } = require('child_process');

class Player {
    constructor(bin_path) {
        fs.accessSync(bin_path, fs.constants.F_OK /*| fs.constants.X_OK*/ | fs.constants.R_OK); // do not check if executable so bugs can run it.
        if (!fs.statSync(bin_path).isFile()) {
            throw new Error('not a file');
        }
        this.child = spawn(bin_path, { stdio: ['pipe', 'pipe', 'ignore'] });
        this.child.stdout.setEncoding('utf8');
    }

    send(command) {
        // stdin read
        return new Promise((resolve, reject) => {
            this.child.stdin.write(command + '\n');
            this.child.stdout.once('data', (data) => {
                resolve(data);
            });
        });
    }

    turn(x, y, timeout = 5000) {
        const move = '' + x + ',' + y;
        return this.send('TURN ' + move, (answer) => {
            const pos = answer.split(',').map((data) => parseInt(data));
            return (pos.length === 2 && !isNaN(pos[0]) && !isNaN(pos[1]));
        }, timeout);
    }

    start(size = 20, timeout = 5000) {
        return this.send('START ' + size, (answer) => {
            return (answer.startsWith('OK') || answer.startsWith('ERROR'));
        }, timeout);
    }

    begin(timeout = 5000) {
        return this.send('BEGIN', (answer) => {
            const pos = answer.split(',').map((data) => parseInt(data));
            return (pos.length === 2 && !isNaN(pos[0]) && !isNaN(pos[1]));
        }, timeout);
    }

    kill(signal = 'SIGKILL') {
        try {
            this.child.kill(signal);
        } catch (error) { }
    }
}

module.exports = Player;
