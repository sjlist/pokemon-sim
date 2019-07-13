var express = require('express');
var router = express.Router();

// Load pokemon model (template)
var Pokemon = require('../models/PokemonModel');

router.route('/')
    .post((req, res) => {  
        console.log(req);
    })


module.exports = router;
