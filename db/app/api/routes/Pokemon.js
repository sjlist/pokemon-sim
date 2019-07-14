const express = require('express');
const router = express.Router();

// Load pokemon model
// var PokemonModel = require('../models/PokemonModel');
// var Pokemon = mongoose.model('pokemon', PokemonModel);

router.get('/', (req, res) => {
  console.log('pokemon test GET recieved');
  res.send({ pokemon: true }); 
});

router.post('/', (req, res) => {
	console.log('pokemon test POST recieved');
	res.send({ pokemon: req.body });
});

module.exports = router;
