const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors')
const morgan = require('morgan');

//Define app
var app = express();

// set our application port
app.set('port', 3001);

//To have better logs
app.use(morgan('dev'));

//Enable all requests
app.use(cors());

//To parse URL encoded data
app.use(bodyParser.urlencoded({ extended: true }))

//To parse json data
app.use(bodyParser.json())


// ******** Define API routes ********

// Test API connection
var testAPIRouter = require('./routes/TestAPIConnect');
app.use('/api/testAPI', testAPIRouter); 

// Test DB connection
var testDBRouter = require('./routes/TestDBConnect');
app.use('/api/testDB', testDBRouter);

// Pokemon api
var pokemonrouter = require('./routes/Pokemon');
app.use('/api/pokemon', pokemonrouter);

// Moves api
var movesrouter = require('./routes/Moves');
app.use('/api/moves', movesrouter);

// other routes here
app.get('*', function(req, res){
   res.send('Sorry, this is an invalid URL.');
});


// run app on specified port
app.listen(app.get('port'), () => console.log(`App started on port ${app.get('port')}`));
