const http = require('http')
// Route the incoming request based on type (LaunchRequest, IntentRequest,
// etc.) The JSON body of the request is provided in the event parameter.
exports.handler = function (event, context) {
    try {
        console.log("event.session.application.applicationId=" + event.session.application.applicationId);

        /**
         * Uncomment this if statement and populate with your skill's application ID to
         * prevent someone else from configuring a skill that sends requests to this function.
         */

    // if (event.session.application.applicationId !== "") {
    //     context.fail("Invalid Application ID");
    //  }

        if (event.session.new) {
            onSessionStarted({requestId: event.request.requestId}, event.session);
        }

        if (event.request.type === "LaunchRequest") {
            onLaunch(event.request,
                event.session,
                function callback(sessionAttributes, speechletResponse) {
                    context.succeed(buildResponse(sessionAttributes, speechletResponse));
                });
        } else if (event.request.type === "IntentRequest") {
            onIntent(event.request,
                event.session,
                function callback(sessionAttributes, speechletResponse) {
                    context.succeed(buildResponse(sessionAttributes, speechletResponse));
                });
        } else if (event.request.type === "SessionEndedRequest") {
            onSessionEnded(event.request, event.session);
            context.succeed();
        }
    } catch (e) {
        context.fail("Exception: " + e);
    }
};

/**
 * Called when the session starts.
 */
function onSessionStarted(sessionStartedRequest, session) {
    // add any session init logic here
}

/**
 * Called when the user invokes the skill without specifying what they want.
 */
function onLaunch(launchRequest, session, callback) {
    getWelcomeResponse(callback)
}

/**
 * Called when the user specifies an intent for this skill.
 */
function onIntent(intentRequest, session, callback) {

    var intent = intentRequest.intent
    var intentName = intentRequest.intent.name;

    // dispatch custom intents to handlers here
    if (intentName === "CastAbilityIntent") {
        handleCastAbilityRequest(intent, session, callback)
    } else if(intentName === "OpenInventoryIntent"){
        handleOpenInventoryRequest(intent, session, callback)
    } else if(intentName === "ConnectToGameIntent"){
        handleConnectRequest(intent, session, callback)
    } else if(intentName === "DisconnectFromGameIntent"){
        handleDisconnectRequest(intent, session, callback)
    } else if(intentName === "AMAZON.HelpIntent"){
        handleHelpRequest(intent, session, callback)
    } else if(intentName === "AMAZON.StopIntent"){
        handleFinishSessionRequest(intent, session, callback) 
    } else {
         throw "Invalid intent"
    }
}

/**
 * Called when the user ends the session.
 * Is not called when the skill returns shouldEndSession=true.
 */
function onSessionEnded(sessionEndedRequest, session) {
    const postData = JSON.stringify({"user": session.user.userId})
    const options = {
        path: '/alexa/disconnect',
        method: 'POST'
    }

    const req = http.request(url(), options, (res) => {})
    req.write(postData)
    req.end()
}

// ------- Skill specific logic -------

function getWelcomeResponse(callback) {
    var speechOutput = "Welcome to the Alexa role play game! You can connect to a game by aking me with your session ID. Then I can open your inventory or cast one of your spell if you ask for it."

    var reprompt = speechOutput

    var header = "Alexa role play game"

    var shouldEndSession = false

    var sessionAttributes = {
        "speechOutput" : speechOutput,
        "repromptText" : reprompt
    }

    callback(sessionAttributes, buildSpeechletResponse(header, speechOutput, reprompt, shouldEndSession))

}

// Return the url of the server
function url(){
    return 'http://alexaunrealserver.herokuapp.com'
}

//Send A request to the server and check if you are connect to a game session
function SendRequest(intent, session, callback, requestType, requestData) {
    
    const postData = JSON.stringify({"user": session.user.userId, "type": requestType, "data": requestData})

    const options = {
        path: '/alexa/request',
        method: 'POST'
    }

    
    const req = http.request(url(), options, (res) => {

        let data = ''

        res.on('data', (chunk) => {
            data += chunk
        })

        res.on('end', () => {
            var speechOutput
            if(JSON.parse(data).isConnected){
                speechOutput = 'request received'
            }
            else{
                speechOutput = 'You are not connected to a game'
            }
            callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
        })

    }).on("error", (err) => {
        var speechOutput = "Cannot connect to the server."
        callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
    })

    req.write(postData)

    req.end()
}

// Send an abilty request with the name of the ability
function handleCastAbilityRequest(intent, session, callback){
    SendRequest(intent, session, callback, "Cast", intent.slots.ability.value)
}

// Send an Inventory request
function handleOpenInventoryRequest(intent, session, callback){
    SendRequest(intent, session, callback, "Inventory", "")
    
}

// Send a connection request to connect to a game session
function handleConnectRequest(intent, session, callback){

    const postData = JSON.stringify({"user": session.user.userId, "id": intent.slots.gameID.value})

    const options = {
        path: '/alexa/connect',
        method: 'POST'
    }

    const req = http.request(url(), options, (res) => {

        let data = ''

        res.on('data', (chunk) => {
            data += chunk
        })

        res.on('end', () => {
            var speechOutput
            if(JSON.parse(data).isConnected){
                speechOutput = "You have been successfully connected."
            }
            else{
                speechOutput = "The game you are trying to connect to does not exist or someone is already connected or you are already connected to another game."
            }
            callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
        })

    }).on("error", (err) => {
        var speechOutput = "Cannot connect to the server"
        callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
    })

    req.write(postData)

    req.end()
}

// Disconnect from the actual game session
function handleDisconnectRequest(intent, session, callback){

    const postData = JSON.stringify({"user": session.user.userId})
    const options = {
        path: '/alexa/disconnect',
        method: 'POST'
    }

    const req = http.request(url(), options, (res) => {

        let data = ''

        res.on('data', (chunk) => {
            data += chunk
        })
        res.on('end', () => {
            var speechOutput
            if(JSON.parse(data).succeed){
                speechOutput = 'You have been disconnected.'
            }
            else{
                speechOutput = "An error happen during deconnection."
            }
            callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
        })

    }).on("error", (err) => {
        var speechOutput = "An error happen during deconnection."
        callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
    })

    req.write(postData)

    req.end()
}

function handleHelpRequest(intent, session, callback){
    var speechOutput = "You can connect to a game by aking me with your session ID. Then I can open your inventory or cast one of your spell if you ask for it.You can connect to a game by aking me with your session ID. Then I can open your inventory or cast one of your spell if you ask for it."

    callback(session.attributes, buildSpeechletResponseWithoutCard(speechOutput, "", false))
}

function handleFinishSessionRequest(intent, session, callback) {
    // End the session with a "Good bye!" if the user wants to quit the game
    const postData = JSON.stringify({"user": session.user.userId})
    const options = {
        path: '/alexa/disconnect',
        method: 'POST'
    }

    const req = http.request(url(), options, (res) => {})
    req.write(postData)
    req.end()
    callback(session.attributes,
        buildSpeechletResponseWithoutCard("Good bye! Thank you for playing!", "", true));
}





// ------- Helper functions to build responses for Alexa -------


function buildSpeechletResponse(title, output, repromptText, shouldEndSession) {
    return {
        outputSpeech: {
            type: "PlainText",
            text: output
        },
        card: {
            type: "Simple",
            title: title,
            content: output
        },
        reprompt: {
            outputSpeech: {
                type: "PlainText",
                text: repromptText
            }
        },
        shouldEndSession: shouldEndSession
    };
}

function buildSpeechletResponseWithoutCard(output, repromptText, shouldEndSession) {
    return {
        outputSpeech: {
            type: "PlainText",
            text: output
        },
        reprompt: {
            outputSpeech: {
                type: "PlainText",
                text: repromptText
            }
        },
        shouldEndSession: shouldEndSession
    };
}

function buildResponse(sessionAttributes, speechletResponse) {
    return {
        version: "1.0",
        sessionAttributes: sessionAttributes,
        response: speechletResponse
    };
}