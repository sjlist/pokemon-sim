const mongoose = require('mongoose');
const express = require('express');
const router = express.Router();

// Load pokemon model
var PokemonModel = require('../models/PokemonModel');

// Define pokemon api
router.get('/', (req, res) => {
	PokemonModel.find({}, function(err, pokemon) {
		if (err) throw err;
		console.log(pokemon);
		res.send(pokemon);
	});

	// res.send({ pokemon: 'test get message' }); 
});

router.post('/', (req, res) => {
	let newPokemon = new PokemonModel({
		Species: req.body.Species,
		Type: {type0: req.body.Type0, Type1: req.body.Type1}, 
		HP:  req.body.HP,
		ATK: req.body.ATK,
		DEF: req.body.DEF,
		SPA: req.body.SPA,
		SPD: req.body.SPD,
		SPE: req.body.SPE 
	});
	newPokemon.save();
	res.send(newPokemon);
});

module.exports = router;
