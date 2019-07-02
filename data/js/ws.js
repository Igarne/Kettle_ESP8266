let socket = new WebSocket("ws://192.168.1.202:81");


document.getElementById('led_on').onclick = function() {
    var outgoingMessage = "led_on";
    socket.send(outgoingMessage);
    return false;
};

document.getElementById('led_off').onclick = function() {
    var outgoingMessage = "led_off";
    socket.send(outgoingMessage);
    return false;
};

socket.onmessage = function(event) {
    var incomingMessage = event.data;
    showMessage(incomingMessage, "free_mem");
    detect_blank(incomingMessage, "free_mem");
};

function showMessage(message, id) {
    document.getElementById(id).innerText = message;
}

function detect_blank(message, id) {
    console.log(message);
    if (message === 'No data') {
        document.getElementById(id).classList.add("btn-danger");
    }
    else {
        document.getElementById(id).classList.remove("btn-danger");
    }
}
