
arrayMove = []
boardMap = []
timeElapsed = []

function updateGraph() {
    const div = document.getElementById('chartdiv');
    const ctx = document.getElementById('myChart');
    div.removeChild(ctx);
    const chart = document.createElement('canvas');
    chart.id = 'myChart';
    div.appendChild(chart);
    const labels = [];
    for (let i = 0; i < arrayMove.length / 2; i++) {
        labels.push(i * 2 + 1);
    }
    let totaltime = 0;
    for (let i = 0; i < arrayMove.length; i++) {
        if (arrayMove[i].type === 'enemy') {
            totaltime += arrayMove[i].timeelapsed;
        }
    }
    if (arrayMove[arrayMove.length - 1].type === 'enemy') {
        timeElapsed.push(totaltime);
    }
    console.log(timeElapsed);
    let graph = new Chart(chart, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'Plays indexed by time',
                data: timeElapsed,
                fill: false,
                borderColor: 'rgb(75, 192, 192)',
                tension: 0.1
            }]
        },
    });
}

function createGomokuBoard() {
    const board = document.getElementsByClassName('game-board-background')[0];
    const boardSize = 21;
    
    for (let i = 0; i < boardSize; i++) {
        const row = document.createElement('tr');
        row.classList.add('row');
        boardMap.push([]);
        for (let j = 0; j < boardSize; j++) {
            const cell = document.createElement('td');
            cell.classList.add('cell');
            row.appendChild(cell);
            boardMap[i].push(0);
        }
        board.appendChild(row);
    }
}

function createGomokuBoardClickable() {
    const board = document.getElementsByClassName('game-board-clickable')[0];
    const boardSize = 21;

    for (let i = 0; i < boardSize; i++) {
        const row = document.createElement('tr');
        row.classList.add('row-clickable');
        row.classList.add('row-clickable-' + i);
        for (let j = 0; j < boardSize; j++) {
            const cell = document.createElement('td');
            cell.classList.add('cell-clickable');
            cell.classList.add('cell-clickable-' + i  + '-' + j);
            row.appendChild(cell);
        }
        board.appendChild(row);
    }
}

function createPoint(type) {
    const point = document.createElement('div');
    point.classList.add('point');
    point.classList.add(type);
    return point;
}

async function sayWinnerToServer(winner) {
    return await fetch("http://127.0.0.1:3000/win", {
        method: "POST",
        mode: 'cors',
        redirect: 'manual',
        body: JSON.stringify({"winner": winner}),
        headers: {
            "Content-Type": "application/json"
        },
    });
        
}

function placePoint(x, y, type) {
    const cell = document.getElementsByClassName('cell-clickable-' + y + '-' + x)[0];
    cell.appendChild(createPoint(type));
    cell.classList.add('occupied');
    if (type == 'player') {
        boardMap[x][y] = 1;
    } else {
        boardMap[x][y] = 2;
    }
    const nbmoves = document.getElementById('nbmoves')
    const nb = parseInt(nbmoves.innerText.replace("Nbmoves = ", ""))
    nbmoves.innerHTML = "Nbmoves = " + (nb + 1).toString();
    registerMove(x, y, type)
    updateGraph();
}

function registerMove(x, y, type) {
    let timeElapsed = 0;
    const now = new Date();
    if (arrayMove.length == 0) {
        arrayMove.push({x: parseInt(x) + 1, y: parseInt(y) + 1, type: type, timeelapsed: timeElapsed, time: now});
    } else {
        const lastMove = arrayMove[arrayMove.length - 1];
        timeElapsed = now - lastMove.time;
        timeElapsed /= 1000;
        var seconds = Math.round(timeElapsed);
        arrayMove.push({x: parseInt(x) + 1 , y: parseInt(y) + 1, type: type, timeelapsed: seconds, time: now});
        document.getElementById('moves').innerHTML = "";
    }
    for (let i = 0; i < arrayMove.length; i++) {
        const move = arrayMove[i];
        if (move.type == 'player') {
            document.getElementById('moves').innerHTML += "The Player played at (" + move.x + ", " + move.y + ") in " + move.timeelapsed + " seconds.<br>";
        } else {
            document.getElementById('moves').innerHTML += "The AI played at (" + move.x + ", " + move.y + ") in " + move.timeelapsed + " seconds.<br>";
        }
    }
}

function checkwin2(x, y, type) {
    let count = 0;
    let i = x;
    let j = y;
    while (i >= 0 && boardMap[i][j] == type) {
        count++;
        i--;
    }
    i = x + 1;
    while (i < 21 && boardMap[i][j] == type) {
        count++;
        i++;
    }
    if (count >= 5) {
        return true;
    }
    count = 0;
    i = x;
    j = y;
    while (j >= 0 && boardMap[i][j] == type) {
        count++;
        j--;
    }
    j = y + 1;
    while (j < 21 && boardMap[i][j] == type) {
        count++;
        j++;
    }
    if (count >= 5) {
        return true;
    }
    count = 0;
    i = x;
    j = y;
    while (i >= 0 && j >= 0 && boardMap[i][j] == type) {
        count++;
        i--;
        j--;
    }
    i = x + 1;
    j = y + 1;
    while (i < 21 && j < 21 && boardMap[i][j] == type) {
        count++;
        i++;
        j++;
    }
    if (count >= 5) {
        return true;
    }
    count = 0;
    i = x;
    j = y;
    while (i >= 0 && j < 21 && boardMap[i][j] == type) {
        count++;
        i--;
        j++;
    }
    i = x + 1;
    j = y - 1;
    while (i < 21 && j >= 0 && boardMap[i][j] == type) {
        count++;
        i++;
        j--;
    }
    if (count >= 5) {
        return true;
    }
    return false;
}

function checkwin() {
    for (let i = 0; i < 21; i++) {
        for (let j = 0; j < 21; j++) {
            if (boardMap[i][j] != 0) {
                if (checkwin2(i, j, boardMap[i][j])) {
                    return boardMap[i][j];
                }
            }
        }
    }
    return false;
}

function initAI() {
    fetch("http://127.0.0.1:3000/initAI", {
            method: "POST",
            mode: 'cors',
            redirect: 'manual',
            body: JSON.stringify({ai: "pbrain-gomoku-ai"}),
            headers: {
                "Content-Type": "application/json"
            },
        }).then(function (response) {
            // The API call was successful!
            if (response.ok) {
                return response.json();
            } else {
                return Promise.reject(response);
            }
        }).then(function (data) {
            // This is the JSON from our response
            placePoint(data.x, data.y, 'enemy');
            console.log("data : ", data)
        }).catch(function (err) {
            // There was an error
            console.warn('Something went wrong.', err);
        });
}

createGomokuBoard();
createGomokuBoardClickable();
initAI();


window.onclick = async function(event) {
    const domain = window.location.origin

    const element = event.target;
    if (element.classList.contains('cell-clickable')) {
        if (element.classList.contains('occupied')) {
            window.alert('This cell is occupied!');
            return;
        }
        const coordinates = element.classList[1].split('-');
        const x = coordinates[3];
        const y = coordinates[2];
        console.log('x: ' + x + ' y: ' + y, "test");
        if (x <= 19 && y <= 19) {
            placePoint(x, y, 'player');
            const win = checkwin();
            if (win == 1) {
                alert('You win!');
                await sayWinnerToServer(2);
            } else if (win == 2) {
                alert('You lose!');
                await sayWinnerToServer(1);
            }
            fetch("http://127.0.0.1:3000/play", {
                method: "POST",
                mode: 'cors',
                redirect: 'manual',
                body: JSON.stringify({x: x, y: y}),
                headers: {
                    "Content-Type": "application/json"
                },
            }).then(function (response) {
                // The API call was successful!
                if (response.ok) {
                    return response.json();
                } else {
                    return Promise.reject(response);
                }
            }).then(function (data) {
                // This is the JSON from our response
                placePoint(data.x, data.y, 'enemy');
                const win = checkwin();
                if (win == 1) {
                    alert('You win!');
                    sayWinnerToServer(2);
                } else if (win == 2) {
                    alert('You lose!');
                    sayWinnerToServer(1);
                }
                console.log("data : ", data)
            }).catch(function (err) {
                // There was an error
                console.warn('Something went wrong.', err);
            });
        }
    } else if (element.id === 'reset') {
        location.reload();
    }
}
