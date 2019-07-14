// From https://medium.com/free-code-camp/create-a-fullstack-react-express-mongodb-app-using-docker-c3e3e21c4074
const express = require('express');
const router = express.Router();
const mongoose = require('mongoose');

// Variable to be sent to Frontend with Database status
let databaseConnection = 'Waiting for Database response...';

router.get('/', function(req, res) {
	console.log('db test get recieved');
    res.send({ connection: databaseConnection });
});

// Connecting to MongoDB
mongoose.connect('mongodb://localhost:27017/pokemon', {useNewUrlParser: true});

// If there is a connection error send an error message
mongoose.connection.on('error', error => {
    console.log('Database connection error:', error);
    databaseConnection = 'Error connecting to Database';
});

// If connected to MongoDB send a success message
mongoose.connection.once('open', () => {
    console.log('Connected to Database!');
    databaseConnection = 'Connected to Database';
});

module.exports = router;
