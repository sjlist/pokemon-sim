var express = require('express');
var router = express.Router();

// Load pokemon model (template)
// var Pokemon = require('../models/PokemonModel');

router.route('/')
	.get((req, res) => { 
		console.log('get recieved @ pokemon');
		res.send({ pokemonapi: true });
	})
	.post((req, res) => {  
		// let poke = new Pokemon({  
		// 	species: 'Pikachu',
		// 	type: { type0: 'WATER', type1: 'DRAGON' }, 
		// 	HP:  12,
		// 	ATK: 20,
		// 	DEF: 13,
		// 	SPA: 30,
		// 	SPD: 12,
		// 	SPE: 40
		// });
		// poke.save();
		// res.status(201).send(poke)
		console.log('post recieced @ pokemon');
		res.send({ pokemonapi: true });
	});


module.exports = router;
