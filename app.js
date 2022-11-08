const express = require('express')
const app = express()
var bodyParser = require("body-parser")
const { Server } = require("socket.io");
const http = require('http');

const server = http.createServer(app);

const port = 3000

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

const io = new Server(server);

app.post('/', (req, res, next) => {
    data = req.body

    console.log(data)
    io.emit('di', data); // This will emit the event to all connected sockets

    res.json(data)
})

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

io.on('connection', (socket) => {
    console.log('a user connected');
    socket.on('disconnect', () => {
        console.log('user disconnected');
    });
});

server.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})