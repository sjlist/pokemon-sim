var express = require('Express');
var bodyParser = require('body-parser');
var cors = require('cors')
var morgan = require('morgan');


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


// Define paths
// test DB connection
var testDBRouter = require('./routes/testDBConnect');
app.use('/testDB', testDBRouter);

// Pokemon api
var pokemon = require('./routes/Pokemon');
app.use('/pokemonapi', pokemon);

// other routes here
app.get('*', function(req, res){
   res.send('Sorry, this is an invalid URL.');
});



// run app on specified port
app.listen(app.get('port'), () => console.log(`App started on port ${app.get('port')}`));
