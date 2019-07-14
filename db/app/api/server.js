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


// app.all("*", function (req, resp, next) {
//    console.log(req); // do anything you want here
//    next();
// });

// Confirm server connection
app.get('/express_backend', (req, res, next) => {
  console.log('express backend recieved GET');
  res.send({ express: true }); 
});

// Define paths
// test DB connection
// var testDBRouter = require('./routes/testDBConnect');
// app.use('/api/testDB', testDBRouter);

// Pokemon api
var pokemonrouter = require('./routes/Pokemon');
app.use('/pokemonapi', pokemonrouter);



// other routes here
app.get('*', function(req, res){
   res.send('Sorry, this is an invalid URL.');
});



// run app on specified port
app.listen(app.get('port'), () => console.log(`App started on port ${app.get('port')}`));
