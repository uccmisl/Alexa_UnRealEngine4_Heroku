// A class to store the différent sockets with their session ID and alexa ID to be able to process alexa's requests with the right socket
module.exports = class SocketList{

    constructor() {
        // The list sockets with their session ID and alexa ID
        this.sockets = []
    }

    // Get the first session id available
    getNextId(){
        var id = 0
        var exist = true
        while(exist){
            id++
            exist = false
            for (var element of this.sockets){
                if(element[0]==id) exist = true
            }
        }
        return id
    }

    // Add a socket to the list and give it an unuse session ID
    addSocket(socket){
        var id = this.getNextId()
        this.sockets.push([id, null, socket])
        return id
    }

    // Add the alexa id to the corresponding entry of the list based on the session id
    connectAlexa(id, alexaId){
        if(this.getLineByAlexaId(alexaId) == null) {
            var elem = this.getLineById(id);
            if (elem != null) {
                if (elem[1] == null) {
                    elem[1] = alexaId
                    return elem
                }
            }
        }
        return null
    }

    // Remove the alexaId from the list of sockets (it don't remove the entry, it just set the id to null)
    disconnectAlexa(alexaId){
        var elem = this.getLineByAlexaId(alexaId);
        if(elem!=null){
            elem[1] = null
            return elem
        }
        return null
    }

    // Remove the entry of the corresponding socket
    removeBySocketRef(socket) {
        this.sockets.splice(this.sockets.indexOf(this.getLineBySocketRef(socket)), 1);
    }

    // Get an entry (id, alexaId and socket) by his id
    getLineById(id) {
        for (var element of this.sockets){
            if(element[0]==id)
                return element
        }
        return null
    }

    // Get an entry (id, alexaId and socket) by his alexaid
    getLineByAlexaId(id) {
        for (var element of this.sockets){
            if(element[1]==id)
                return element
        }
        return null
    }

    // Get an entry (id, alexaId and socket) by his socket reference
    getLineBySocketRef(socket) {
        for (var element of this.sockets){
            if(element[2]==socket)
                return element
        }
        return null
    }
}