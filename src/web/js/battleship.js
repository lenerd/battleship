"use strict";

var StateEnum = {
    INITIAL: 0,
    PLACEMENT_PHASE: 1,
    QUERY_PHASE: 2,
    ANSWER_PHASE: 3,
    END: 4,
    WAITING: 5,
};

var BS = {
    state: StateEnum.INITIAL,
};

init();


// var connection = new WebSocket('ws://localhost:1338');
//


function init()
{
    BS.ws = new WebSocket('ws://localhost:' + location.search.slice(1));
    BS.ws.onmessage = ws_dispatcher;
    BS.ws.onopen = function(evt) { console.log("websocket opened"); };
    BS.ws.onclose = function(evt) { console.log("websocket closed"); };
    BS.ws.onerror = function(evt) { console.log("websocket error: " + evt.data); };
    // open websocket, connect to application
    // setup some handlers
    // set state to placement phase

    create_board("board-local");
    create_board("board-remote");

    BS.state = StateEnum.PLACEMENT_PHASE;
    placement_phase();
}

function ws_dispatcher(evt)
{
    console.log("received message from websocket:\n" + evt.data);
    try
    {
        var message = JSON.parse(evt.data);
        console.log(message);
    }
    catch (e)
    {
        console.log(e);
    }
    if (message.type == "message")
    {
        post_message(message);
    }
    else if (message.type == "update_position")
    {
        update_position(message);
    }
    else if (message.type == "switch_state")
    {
        switch_state(message);
    }
    else
    {
        console.log("don't understand message: " + message);
    }
}

function switch_state(data)
{
    if (data.state == "query_phase")
    {
        BS.state = StateEnum.QUERY_PHASE;
    }
    else if (data.state == "answer_phase")
    {
        BS.state = StateEnum.ANSWER_PHASE;
    }
    else
    {
        console.log("unknown state: " + data.state);
    }
}

function post_message(data)
{
    console.log("posting message: " + data.message);
    var msg_box = document.getElementById("msg_box");
    var p = document.createElement("p");
    p.appendChild(document.createTextNode(data.message));
    msg_box.appendChild(p);
}

function get_char(ship, queried)
{
    if (ship)
    {
        if (queried)
            return "X";
        else
            return "O";
    }
    else
    {
        if (queried)
            return "~";
        else
            return "";
    }
}

function update_position(data)
{
    var ship = data.ship;
    var queried = data.queried;
    var row = data.row;
    var col = data.col;
    var board = data.board;
    var cell = document.querySelectorAll("div#board-" + board + " > table > tr > td[data-row='" + row.toString() + "'][data-col='" + col.toString() + "']")[0];
    cell.innerHTML = get_char(ship, queried);
}


function placement_phase()
{
    // install onclick handler for local board
    // install handler to submission button
    
    var button = document.getElementById("submission-button");
    button.onclick = placement_submission_handler;
    console.log("installed placement_submission_handler");
}

function placement_cell_handler()
{
    // toggle cell states
    // synchronize ui with some bitset
}

function placement_submission_handler()
{
    console.log("executing placement_submission_handler");
    var data = {
        type: "submit_board",
    };
    BS.ws.send(JSON.stringify(data));

    var button = document.getElementById("submission-button");
    button.onclick = null;
    console.log("uninstalled placement_submission_handler");

    BS.state = StateEnum.WAITING;
}

function select_position(row, col)
{
    var data = {
        type: "query",
        row: row,
        col: col,
    };
    console.log("select_position: sending " + JSON.stringify(data));
    BS.ws.send(JSON.stringify(data));
    BS.state = StateEnum.ANSWER_PHASE;
}

function toggle_position(row, col)
{
    var data = {
        type: "toggle_position",
        row: row,
        col: col,
    };
    console.log("toggle_position: sending " + JSON.stringify(data));
    BS.ws.send(JSON.stringify(data));
}

function onclick_cell_dispatcher(board_id, row, col)
{
    console.log("executing onclick_cell_dispatcher for " + board_id + " on (" + row.toString() + ", " + col.toString() + ")");
    if (BS.state === StateEnum.PLACEMENT_PHASE && board_id === "board-local")
    {
        toggle_position(row, col);
    }
    else if (BS.state === StateEnum.QUERY_PHASE && board_id === "board-remote")
    {
        select_position(row, col);
    }
}

function create_board(board_id)
{
    var board = document.getElementById(board_id);

    var table = document.createElement("table")
    table.setAttribute("class", "board");
    var tr = document.createElement("tr");
    table.appendChild(tr);
    var td = document.createElement("td");
    tr.appendChild(td);
    for (var col = 0; col < 10; ++col)
    {
        var td = document.createElement("td");
        td.setAttribute("class", "board-col-index");
        var text = document.createTextNode(col.toString());
        td.appendChild(text);
        tr.appendChild(td);
    }
    for (var row = 0; row < 10; ++row)
    {
        var tr = document.createElement("tr");
        table.appendChild(tr);
        var td = document.createElement("td");
        td.setAttribute("class", "board-row-index");
        var text = document.createTextNode(String.fromCharCode(0x41 + row));
        td.appendChild(text);
        tr.appendChild(td);
        for (var col = 0; col < 10; ++col)
        {
            var td = document.createElement("td");
            td.setAttribute("class", "board-cell");
            td.setAttribute("data-row", row.toString());
            td.setAttribute("data-col", col.toString());
            tr.appendChild(td);
            td.onclick = function(row, col)
            {
                return function() { onclick_cell_dispatcher(board_id, row, col); };
            }(row, col);
        }
    }
    board.appendChild(table);
}

