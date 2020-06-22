var SocketList = require('./socketlist.js')

var clientSockets = new SocketList()

//Create the server and define what he will do when an http request is received
const server = require('http').createServer((req, res) => {
    console.log('request received')

    // When a request is received, transmit it to the game that the alexa's user is connected to
    if(req.url === '/alexa/request') {
        console.log('alexa request received')

        let data = ''

        req.on('data', (chunk) => {
            data += chunk
        })

        req.on('end', () => {
            
            data = JSON.parse(data)
            var line = clientSockets.getLineByAlexaId(data.user)

            if(line != null){
                console.log('emit request to unreal')
                line[2].emit('message', JSON.stringify({"type":data.type, "data": data.data}))
                var msg = { "isConnected": true}
            }
            else {
                var msg = { "isConnected": false}
            }
            res.write(JSON.stringify(msg))

            res.end()
        })
    }
    // When an alexa connection request is received, add the alexa user's id to list with the right game socket if it exist
    else if(req.url === '/alexa/connect') {
        console.log('alexa connect received')

        let data = ''

        req.on('data', (chunk) => {
            data += chunk
        })

        req.on('end', () => {
            data = JSON.parse(data)
            var line = clientSockets.connectAlexa(data.id, data.user)

            if(line != null){
                console.log('emit connection to unreal')
                line[2].emit('alexaConnection', data.user)
                var msg = { "isConnected": true}
                console.log("connected")
            }
            else {
                console.log("notconnected")
                var msg = { "isConnected": false}
            }
            console.log(clientSockets)
            res.write(JSON.stringify(msg))

            res.end()
        })
    }
    //  remove the alexa user's id from the list of sockets
    else if(req.url === '/alexa/disconnect') {
        console.log('alexa disconnect received')

        let data = ''

        req.on('data', (chunk) => {
            data += chunk
        })

        req.on('end', () => {
            data = JSON.parse(data)
            var line = clientSockets.disconnectAlexa(data.user)
            if(line != null){
                console.log('emit disconnection to unreal')
                line[2].emit('alexaDisconnection', data.user)
                var msg = { "succeed": true}
            }
            else {
                var msg = { "succeed": false}
            }
            console.log(clientSockets)

            res.write(JSON.stringify(msg))
            res.end()
        })
    }
    // By default the server just indicate that it is the game sever
    else{
        res.write('Here is alexa unreal server')
        res.end();
    }
})

//create the server to receive connections and bind it to the server
const socket = require('socket.io')(server)

const PORT = process.env.PORT || 3000

// Add the client socket to the socket list when connected and give him his Id
socket.on('connection', function (client) {
    console.log('client connected')
    client.emit('connected', clientSockets.addSocket(client))
    console.log(clientSockets)

    //When the connection end, remove the socket from the list
    client.on('disconnect', function (){
        clientSockets.removeBySocketRef(client);
        console.log('client disconnected')
        console.log(clientSockets)
    })

})

//Start the server on the dedicate port given by heroku
server.listen(PORT, function (err) {
    if (err) throw err
    console.log('listening on port ' + PORT)
})
