class Room {
  constructor(roomname) {
    this._first = null;
    this._second = null;
    this._name = roomname;

    console.log("Room created "+this.name);
  }
  // set name(name) {
  //   this._name = name.charAt(0).toUpperCase() + name.slice(1);
  // }
  get name() {
    return this._name;
  }

  get first() {
    return this._first;
  }

  get second() {
    return this._second;
  }

  set first(connectionPtr) {
    this._first = connectionPtr;
    console.log("first set " + this._first);
  }

  set second(connectionPtr) {
    this._second = connectionPtr;
    console.log("second set " + this._second);
  }

  get isWaiting() {
    return this._first == null || this._second == null;
  }

  isMemberOfThisRoom(remoteAddress) {
    console.log("search for "+remoteAddress);
    // console.log("first "+this._first.remoteAddress);
    // console.log("second "+this._second.remoteAddress);

    if (this._first !=null && this._first.remoteAddress == remoteAddress) {
      return 1;
    }
    if (this._second !=null && this._second.remoteAddress == remoteAddress) {
      return 2;
    }
    return 0;
  }

  startGame() {
    console.log("startGame for room "+this.name);
    this._first.send("first");
    this._second.send("second");
  }

  stopGame(connection) {
    console.log("stopGame for room "+this.name);
    if (connection != this._first) {
      this._first.send("stopgame");
      this._first.close();
    } else {
      this._second.send("stopgame");
      this._second.close();
    }
  }
  // sayHello() {
  //   console.log('Hello, my name is ' + this.name + ', I have ID: ' + this.id);
  // }
}

// var justAGuy = new Person();
// justAGuy.name = 'martin'; // The setter will be used automatically here.
// justAGuy.sayHello(); // Will output 'Hello, my name is Martin, I have ID: id_1'

var rooms = [];

var WebSocketServer = require('websocket').server;
var http = require('http');

var server = http.createServer(function(request, response) {
  // process HTTP request. Since we're writing just WebSockets
  // server we don't have to implement anything.
});
server.listen(3000, function() { });

// create the server
wsServer = new WebSocketServer({
  httpServer: server
});

// WebSocket server
wsServer.on('request', function(request) {
  var connection = request.accept(null, request.origin);
  console.log("new connection "+connection.remoteAddress);

  // check for any unfilled rooms
  var r = waitingRoom();
  if (r) {
    console.log("some one waiting ... ");
    r.second = connection;
    r.startGame();
  } else {
    var newRoom = new Room("room"+rooms.length);
    newRoom.first = connection;
    rooms.push(newRoom);
  }

  // This is the most important callback for us, we'll handle
  // all messages from users here.
  connection.on('message', function(message) {
    if (message.type === 'utf8') {
      // process WebSocket message
      console.log(message.utf8Data);
      console.log("connection msg "+connection.remoteAddress);
      messagePass(connection, message.utf8Data);
    }
  });

  connection.on('close', function(socket) {
    // close user connection
    console.log("connection closed "+connection.remoteAddress);
    stopGame(connection);
  });
});

function waitingRoom() {
  for (i=0;i<rooms.length;i++) {
    if (rooms[i].isWaiting) {
      return rooms[i];
    }
  }
  return null;
}

function stopGame(connection) {
  var index = -1;
  for (i=0;i<rooms.length;i++) {
    console.log("searching room "+rooms[i].name);
    if (rooms[i].isMemberOfThisRoom(connection.remoteAddress) > 0) {
      rooms[i].stopGame(connection);
      index = i;
      break;
    }
  }

  if (index>-1) {
    rooms.splice(index, 1);
    console.log("Room removed "+rooms.length);
  }
}

function messagePass(connection, msg) {
  for (i=0;i<rooms.length;i++) {
    console.log("searching room "+rooms[i].name);
    var playerID = rooms[i].isMemberOfThisRoom(connection.remoteAddress)
    if (playerID > 0) {
      if (playerID == 1) {
        if (rooms[i].second) {
          rooms[i].second.send(msg);
        }
      } else if (playerID == 2) {
        if (rooms[i].first) {
          rooms[i].first.send(msg);
        }
      }
      break;
    }
  }
}