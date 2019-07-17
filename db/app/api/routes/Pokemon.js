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
	let newPokemon = new PokemonModel(req.body);
	console.log(newPokemon);
	newPokemon.save();
	res.send(newPokemon);
});

router.delete('/', (req, res) => {
	console.log('delete recieved:', req.body);
	PokemonModel.deleteOne({_id: req.body._id}, function (err) {
		if (err) console.log(err);
	});
	res.send(req.body);
});

router.put('/', (req, res) => {
	PokemonModel.updateOne({_id: req.body._id}, req.body, function (err) {
		if (err) console.log(err);
	});
	res.send(req.body);
	console.log('recieved update:', req.body);
});

module.exports = router;
