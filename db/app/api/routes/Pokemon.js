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
});

router.post('/', (req, res) => {
	let newPokemon = new PokemonModel({
		Species: req.body.Species,
		Type: {type0: req.body.Type0, type1: req.body.Type1}, 
		HP:  req.body.HP,
		ATK: req.body.ATK,
		DEF: req.body.DEF,
		SPA: req.body.SPA,
		SPD: req.body.SPD,
		SPE: req.body.SPE 
	});
	console.log(newPokemon);
	newPokemon.save();
	res.send(newPokemon);
});

router.delete('/', (req, res) => {
	console.log('delete recieved:', req.body);
	PokemonModel.deleteOne({_id: req.body._id}, function (err) {
		if (err) return handleError(err);
		// deleted at most one pokemon document
	});
	res.send({_id: req.body._id});
});

module.exports = router;
