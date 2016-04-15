http = require('http');
fs = require('fs');

// Socket-Server Config 
var socket_port = 3001;	//edit!


// Stats Post-Server 
// Post-Server Config 
var port = 3011;		//edit!
var host = '127.0.0.1';   //edit!


server = http.createServer( function(req, res) {
 
    if (req.method == 'POST') {
        //console.log("Handling POST request...");
        res.writeHead(200, {'Content-Type': 'text/html'});
 
        var body = '';
        req.on('data', function (data) {
            body += data;
        });
        req.on('end', function () {
           //console.log("POST payload: " + body);
			body = JSON.parse(body);
			if (body.player.activity == 'playing') {
				// Game has start, body contains live game datas
				parseData(body);
			}
        	res.end( '' );
        });
    }
    else
    {
        console.log("Not expecting other request types...");
        res.writeHead(200, {'Content-Type': 'text/html'});
		var html = '<html><body>HTTP Server at http://' + host + ':' + port + '</body></html>';
        res.end(html);
    }
 
});
 
server.listen(port, host);
console.log('Listening at http://' + host + ':' + port);


					  
/* eigene Variablen */
var curRound;
var curDeaths;
var curKills;
var state = 5;
var newState = false; // for test sonst false
var parsedMatchStats = { 
						"player" : {},
						"round" : 0
					};

var living = {
				team : 5,
				enemy : 5
			};
					
var playerStats = [];

var socket_clients = [];


/* eigene Funktionen */

function parseData(data) {
	parsedMatchStats.round = data.map.round;
	parsedMatchStats.player = data.player;
	
	refreshStats(data.player);
	checkRoundReset();
	
	console.log(JSON.stringify(living, null, 2));
	console.log(JSON.stringify(playerStats, null, 2));
	
	if (newState) {
		//processState_v1();
		processState();
	}
}



function checkRoundReset() {
	var rounds = parsedMatchStats.round;
	console.log("ROUND: " + rounds);
	if (rounds != curRound) {
		// new round begins, do reset
		state = 5; 
		living = {
				team : 5,
				enemy : 5
			}
		newState = true;
		if (rounds < curRound) {
			// new game has started, delete all matchstats
			console.log("New game started...");
			playerStats = [];
		}
		curRound = rounds;
	}
}


function refreshStats(player) {
	var found = false;
	for (var i = 0; i < playerStats.length; i++) {
		if (playerStats[i].steamid == player.steamid) {
			// player already inserted and found
			var diff = player.match_stats.kills - playerStats[i].kills;
			if (diff != 0) {
				// new kill from teammate
				state = state + diff
				newState = true;
				living.enemy = living.enemy - diff;
				playerStats[i].kills = player.match_stats.kills;
			}
			if (playerStats[i].deaths < player.match_stats.deaths) {
				// new deaths from teammate
				state = state - 1;
				newState = true;
				living.team = living.team - 1;
				console.log("new death from teammate");
				playerStats[i].deaths = player.match_stats.deaths;
			}
			found = true;
		}
	}
	if (!found) {
		// add player to datas
		// maybe ToDo if first kill/death triggers before player is in array
		playerStats.push({
							"steamid" : player.steamid,
							"kills"	: player.match_stats.kills,
							"deaths" : player.match_stats.deaths
		});
	}
}

function isComplete() {
	if (playerStats.length == 5) return true;
	return false;
}


function processState() {
	// talk to usb....
	var res = (2 * ((living.team - living.enemy) + living.enemy)- 2 * (living.enemy - living.team)) * 0.5;
		
	console.log("STATUS: " + res);
	
	writeToAllSocketClients("S" + res);
	
	newState = false; 
}


function processState_old() {
	// talk to usb....
	console.log(JSON.stringify(playerStats, null, 2));
	console.log("Send STATUS: " + state);
	
	writeToAllSocketClients("S" + state);
	
	newState = false; 
}


/* socket server */
const net = require('net');
var socket_client;

var server = net.createServer((c) => {
	// 'connection' listener
	console.log('CONNECTED: ' + c.remoteAddress +':'+ c.remotePort);
	if (!registerNewSocketClient(c)) {
		console.log("CONNECTED, but already known on another port" + c.remoteAddress +':'+ c.remotePort)
	};
	
	c.on('end', () => {
		console.log('client disconnected');
	});
	
	var incData = '';
	c.on('data', (data) => {
		incData += data; 
		console.log(incData);
	});
	
	// Errorhandling
	c.on('close', function(data) {
        console.log('CLOSED: ' + c.remoteAddress +' '+ c.remotePort);
		forgetSocketClient(c);
    });
	
	c.on('error', function(err) {
        console.log('ERROR: ' + err + " client: " + c.remoteAddress +' '+ c.remotePort);
		//forgetSocketClient(c);	// not needed
    });
		
});

server.on('error', (err) => {
	throw err;
});

server.listen(socket_port, () => {
	console.log('server bound');
});


function registerNewSocketClient(client) {
	if (getIndexOfSocketClient(client.remoteAddress) < 0) {
		//new ip-Adress
		socket_clients.push(client);
		console.log("New Client added: " + client.remoteAddress + ":" + client.remotePort);
		return true;
	}
	else {
		console.log("Client already known: " + client.remoteAddress);	
		return false;
	}	
}

function forgetSocketClient(client) {
	var index = getIndexOfSocketClient(client.remoteAddress, client.remotePort);
	if (index >= 0) {
		var deleted = socket_clients.splice(index,1);
		console.log("Client deleted: " + deleted[0].remoteAddress + ":" + deleted[0].remotePort);
	} else {
		console.log("Client not known: " + client.remoteAddress + ":" + client.remotePort);	
	}	
}

function getIndexOfSocketClient(clientip, clientport) {
	for (var i=0; i < socket_clients.length; i++) {
		if (socket_clients[i].remoteAddress == clientip) {
			if (clientport) {
				if (socket_clients[i].remotePort == clientport) {
					return i;
				}
			} else {
				return i;
			}
		}
	}
	return -1; // not found
}


function writeToAllSocketClients(data) {
	for (var i = 0; i < socket_clients.length; i++) {
		socket_clients[i].write(data);
	}
}
