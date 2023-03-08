const express = require('express');
const app = express();
const { spawn } = require('child_process');
const PlayerBrain = require('./Player');
let cors = require('cors')

app.use(express.json())
app.use(cors())

const board = [];

function createBoard() {

    for (let i = 0; i < 20; i++) {
        const row = [];
        for (let j = 0; j < 20; j++) {
            row.push(0);
        }
        board.push(row);
    }
    console.log(board)
}

function setup_player(path_player) {
    let player_brain = null;
    player_brain = new PlayerBrain(path_player);
    return player_brain;
}

let pX = -1;
let pY = -1;
let turn = 0;
let AI = null;

createBoard();

app.post('/initAI', async (req, res) => {

    console.log(req.body)
    let move = null;

    if (AI != null) {
        AI.kill();
        turn = 0;
    }
    // Make so the player can't refresh and spam AI initialization.
    if (turn === 0) {
        // Initialize AI according to what the front sent me.
        if (process.platform  === 'win32') {
            AI = setup_player("../../" + req.body.ai + ".exe");
        } else {
            AI = setup_player("../../" + req.body.ai);
        }

        // As the manager send START 20 to the AI.
        answer = await AI.start();
        console.log("START ANSWER ->", answer)

        // Sent BEGIN so the AI can send us her first move
        answer = await AI.begin(5000);
        console.log("BEGIN ANSWER ->", answer)

        // Split the answer to get the moves
        move = answer.split(',').map((data) => parseInt(data));
        console.log("MOVE ->", move)

        turn++
        res.send({x: move[0], y: move[1]})
    }
    if (move)
        board[move[1]][move[0]] = 1;
})

app.post('/play', async (request,response) => {

    console.log(request.body);

    if (!(request.body))
        return response.sendStatus(400);

    let x = request.body.x
    let y = request.body.y
    board[y][x] = 2

    let answer = await AI.turn(x, y, 5000);
    console.log("ANSWER ->", answer)
    let move = answer.split(',').map((data) => parseInt(data));
    console.log("MOVE ->", move)
    if (move)
        board[move[1]][move[0]] = 1;

    console.log("PLAY MOVE ->", move)
    console.log("received play");
    for (let i = 0; i < 20; i++) {
        for (let j = 0; j < 20; j++) {
            process.stdout.write(board[i][j].toString());
        }
        process.stdout.write("\n");
    }
    response.send({x: move[0], y: move[1]});
});

app.post('/win', async (request,response) => {
    console.log("received winner");
    console.log(request.body);
    if (!(request.body))
        return response.sendStatus(400);
    let winner = request.body.winner
    console.log("winner is " + winner);
    AI.kill();
    response.send("ok");
});

app.listen(3000, () => {
    console.log('Example app listening on port 3000!');
    }
);
